/*
 * gps.h
 *
 * Holds definitions for reading from GPS UART device.
 *
 *  Created on: Mar 18, 2016
 *      Author: starchmd
 */

#ifndef GPS_H_
#define GPS_H_
	#include <packet.h>
	//Size of GPS machine buffer
	#define GPS_BUFFER_SIZE 256
	//GPS Tag size
	#define GPS_TAG_SIZE 5
	//Serial device connected to GPS
	//#define GPS_SERIAL Serial2
	//Maximum length of DegHHMM field
	#define GPS_MAXIMUM_DEGHHMM 11
	#define GPS_MAXIMUM_HDOP 6

	/**
	 * State machine for reading GPS data from UART
	 */
	typedef enum
	{
		SEARCHING,
		DETECTING,
		READING,
		COMPLETE
	} GPSMachineState;

	/**
	 * Structure to hold the memory composing the
	 * GPS state machine
	 */
	typedef struct
	{
		GPSMachineState state;
		unsigned int index, tagIndex;
		unsigned char buffer[GPS_BUFFER_SIZE];
		char tag[GPS_TAG_SIZE+1];
	} GPSMachine;

	/**
	 * Reads a packet from the GPS UART unit
	 * TaggedPacket* packet - (output) pointer to packet to fill
	 * return - 0 on success, 1 if no packet ready
	 */
	int readGPSPacket(TaggedPacket* packet);
	/**
	 * Runs the GPS State machine and return any GPS packets found
	 * GPSPacket gps - (output) pointer to GPS packet to fill
	 * return - 0 on successful packet read, 1 if no packets found, negative on error
	 */
	int gpsMachine(TaggedPacket* gps);
	/**
	 * Decodes the GPS buffer and fills the tagged packet for return
	 * char* tag - tag of the GPS packet
	 * unsigned char* buffer - buffer holding GPS data
	 * TaggedPacket* packet - (output) packet to fill with GPS data or error
	 * return 0 on success, negative on error, 1 on no packet
	 */
	int decodeGPSBuffer(char* tag, unsigned char* buffer, TaggedPacket* packet);
	/**
	 * Decodes the Degrees Hours Minutes portion of the buffer and
	 * return the address after this section is consumed
	 * unsigned char* buffer - buffer to decode from
	 * float* value - (output) value to fill with decoded value
	 * return - NULL on error, or address after the DegHHMM portion
	 */
	unsigned char* decodeDegHHMM(unsigned char* buffer, float* value);
#endif /* GPS_H_ */
