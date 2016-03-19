/*
 * gps.c
 *
 *  Holds implementation for reading from GPS UART device
 *
 *  Created on: Mar 18, 2016
 *      Author: starchmd
 */
#include <stdlib.h>
#include <string.h>

#include "gps.h"
#include "serror.h"

#include <testing.h>

/**
 * Reads a packet from the GPS UART unit
 * TaggedPacket* packet - (output) pointer to packet to fill
* return - 0 on success, 1 if no packet ready
*/
int readGPSPacket(TaggedPacket* packet)
{
	return gpsMachine(packet);
}

/**
 * Runs the GPS State machine and return any GPS packets found
 * GPSPacket gps - (output) pointer to GPS packet to fill
 * return - 0 on successful packet read, 1 if no packets found, negative on error
 */
int gpsMachine(TaggedPacket* packet)
{
	static GPSMachine machine = {.index = 0, .state = SEARCHING, .tagIndex = 0};

	int ret = 1;
	unsigned char current = 0;
	//Simplicity of reading one byte at a time outweighs the loss in efficiency
	while (GPS_SERIAL.read(&current,1) == 1)
	{
		//Life is a switch statement
		switch (machine.state)
		{
			//Searching for start of GPS PACKET
			case SEARCHING:
				//Found start of packet, detect tag
				if (current == '$')
				{
					machine.state = DETECTING;
					machine.tagIndex = 0;
				}
				break;
			//Reading command tag
			case DETECTING:
				machine.tag[machine.tagIndex++] = current;
				//Read tag, move to read packet
				if (machine.tagIndex >= GPS_TAG_SIZE)
				{
					machine.tag[GPS_TAG_SIZE] = '\0';
					machine.state = READING;
					machine.index = 0;
				}
				//Botched data, found start again
				else if (current == '$')
				{
					machine.state = DETECTING;
					machine.tagIndex = 0;
				}
				break;
			//Reading a packet, looking for complete data
			case READING:
				machine.buffer[machine.index] = current;
				//Buffer read, complete
				if (machine.index >= 2 && machine.buffer[machine.index-2] == '*')
				{
					machine.state = SEARCHING;
					ret = decodeGPSBuffer(machine.tag,machine.buffer,packet);
				}
				//Botched data, found start again
				else if (current == '$')
				{
					machine.state = DETECTING;
					machine.tagIndex = 0;
				}
				machine.index++;
				break;
			//Bogus state, return error and reset
			default:
				machine.state = SEARCHING;
				ret = ER_GPS_BAD_STATE;
				fillErrorPacket(ret,ER_GPS_BAD_STATE_MSG,packet);
		}
		//Emit packet is applicable
		if (ret <= 0)
		{
			return ret;
		}
	}
	return ret;
}
/**
 * Decodes the GPS buffer and fills the tagged packet for return
 * char* tag - tag of the GPS packet
 * TaggedPacket* packet - (output) packet to fill with GPS data or error
 * unsigned char* buffer - buffer holding GPS data
 * return 0 on success, negative on error, 1 on no packet
 */
int decodeGPSBuffer(char* tag, unsigned char* buffer, TaggedPacket* packet)
{
	int i = 0;
	float latitude, longitude, altitude;
	unsigned char time[10], satellites, checksum = 0;
	//Currently we are only decoding the GPGSA tag
	if (strcmp(tag,"GPGGA") != 0)
	{
		return 1;
	}
	//Check checksum
	while (i < GPS_BUFFER_SIZE && buffer[i] != '*')
	{
		checksum += buffer[i];
		i++;
	}
	if (i == GPS_BUFFER_SIZE)
	{
		fillErrorPacket(ER_GPS_MALFORMED_DATA,ER_GPS_MALFORMED_DATA_MSG,packet);
		return ER_GPS_MALFORMED_DATA;
	}
	checksum += '$'+'G'+'P'+'G'+'G'+'A';
	checksum -= (buffer[i+1] - (buffer[i+1] >= 65 ? 55 : 48)) << 4 + buffer[i+2] - (buffer[i+2] >= 65 ? 55 : 48);
	if (checksum != 0)
	{
		fillErrorPacket(ER_GPS_NOT_CHECKSUM,ER_GPS_NOT_CHECKSUM_MSG,packet);
		return ER_GPS_NOT_CHECKSUM;
	}
	fillErrorPacket(ER_GPS_MALFORMED_DATA,ER_GPS_MALFORMED_DATA_MSG,packet);
	return ER_GPS_MALFORMED_DATA;
	//Buffer should start with comma, then 10 digits of time
	buffer++;
	if (*buffer == ',')
	{
		fillErrorPacket(ER_GPS_NOT_LOCKED,ER_GPS_NOT_LOCKED_MSG,packet);
		return ER_GPS_NOT_LOCKED;
	}
	memcpy(packet->packet.gps.time,buffer,sizeof(packet->packet.gps.time));
	buffer += sizeof(packet->packet.gps.time);
	buffer++;
	//Read lat and long
	buffer = decodeDegHHMM(buffer,&packet->packet.gps.latitude);
	if (buffer == NULL)
	{
		fillErrorPacket(ER_GPS_MALFORMED_DATA,ER_GPS_MALFORMED_DATA_MSG,packet);
		return ER_GPS_MALFORMED_DATA;
	}
	buffer = decodeDegHHMM(buffer,&packet->packet.gps.longitude);
	if (buffer == NULL)
	{
		fillErrorPacket(ER_GPS_MALFORMED_DATA,ER_GPS_MALFORMED_DATA_MSG,packet);
		return ER_GPS_MALFORMED_DATA;
	}
	//Ignore lock flag, read satellites count
	buffer += 2;
	packet->packet.gps.satellites = 0;
	packet->packet.gps.satellites = (*buffer -48)*10 + (*(buffer+1) - 48);
	buffer += 3;
	//Look for buffer after comma to read MSL altitude
	i = 0;
	while (buffer[i++] != ',' && i < GPS_MAXIMUM_HDOP);
	packet->packet.gps.altitude = atof(buffer+i); //TODO: replace w/ length-safe version
	packet->type = GPSPacketType;
	packet->length = sizeof(GPSPacket);
	return 0;
}
/**
 * Decodes the Degrees Hours Minutes portion of the buffer and
 * return the address after this section is consumed
 * unsigned char* buffer - buffer to decode from
 * float* value - (output) value to fill with decoded value
 * return - NULL on error, or address after the DegHHMM portion
 */
unsigned char* decodeDegHHMM(unsigned char* buffer, float* value)
{
	int i = 0,tmp = 0;
	//Safely find '.' or error
	while (buffer[i++] != '.' && i < GPS_MAXIMUM_DEGHHMM);
	if (i >= GPS_MAXIMUM_DEGHHMM || i < 2)
	{
		return NULL;
	}
	//Convert minutes, and fractional minutes to degrees
	*value = atof(buffer + i-2)/60.0f; //TODO: replace w/ length-safe version
	//Calculate integral degrees and add them to fractional
	i -= 3;
	while (i > 0)
	{
		tmp *= 10;
		tmp += (*buffer - 48);
		buffer++; i--;
	}
	*value += tmp;
	//Safely find  "," or error
	i = 0;
	while (buffer[i++] != ',' && i < GPS_MAXIMUM_DEGHHMM+3);
	if (i >= GPS_MAXIMUM_DEGHHMM+3)
	{
		return NULL;
	}
	//Negate value based on hemisphere
	*value *= (buffer[i] == 'S' || buffer[i] == 'W')? -1 : 1;
	//Return buffer after trailing comma
	buffer += i+2;
	return buffer;
}
