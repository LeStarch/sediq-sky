/*
 * sky-gps.cpp
 *
 *  Created on: Apr 28, 2017
 *      Author: starchmd
 */
#include <string.h>
#include <stdio.h>
#include "sky-gps.h"

using namespace SkySensor;

int SkyGPS::GPSPacket::serialize(unsigned char* buffer, size_t size) {
    Serial.println(this->altitude);
    *(((float*)buffer) + 0) = this->altitude;
    Serial.println(this->latitude);
    *(((float*)buffer) + 1) = this->latitude;
    Serial.println(this->longitude);
    *(((float*)buffer) + 2) = this->longitude;
    return 0;
}

int SkyGPS::GPSPacket::deserialize(unsigned char* buffer) {
    this->altitude = *(((float*)buffer) + 0);
    this->latitude = *(((float*)buffer) + 1);
    this->longitude= *(((float*)buffer) + 2);
    return 0;
}

SkyGPS::SkyGPS(HardwareSerial* serial) {
    this->serial = serial;
}
SkyGPS::~SkyGPS() {}

int SkyGPS::detect(uint32_t timeout, size_t& size) {
    this->gpsMachine(timeout);
    size = this->packetCount;
    return 0;
}
size_t SkyGPS::getPacket(uint8_t* buffer, size_t size) {
    if (size < sizeof(GPSPacket)) {
        return 1;
    }
    this->packetCount--;
    Serial.println(this->packets[this->packetCount].latitude);
    this->packets[this->packetCount].serialize(buffer, size);
    return 0;
}


int SkyGPS::gpsMachine(uint32_t timeout)
{
    int ret = 0;
    unsigned char current = 0;
    //Simplicity of reading one byte at a time outweighs the loss in efficiency
    uint32_t stop = micros() + timeout*1000;
    while ((this->serial->available() > 0) && (micros() < stop))
    {
        current = (unsigned char)this->serial->read();
        //Life is a switch statement
        switch (this->state)
        {
            //Searching for start of GPS PACKET
            case SEARCHING:
                //Found start of packet, detect tag
                if (current == '$')
                {
                    this->state = DETECTING;
                    this->tagIndex = 0;
                }
                break;
            //Reading command tag
            case DETECTING:
                this->tag[this->tagIndex++] = current;
                //Read tag, move to read packet
                if (this->tagIndex >= GPS_TAG_SIZE)
                {
                    this->tag[GPS_TAG_SIZE] = '\0';
                    this->state = READING;
                    this->index = 0;
                }
                //Botched data, found start again
                else if (current == '$')
                {
                    this->state = DETECTING;
                    this->tagIndex = 0;
                }
                break;
            //Reading a packet, looking for complete data
            case READING:
                this->buffer[this->index] = current;
                //Buffer read, complete
                if (this->index >= 2 && this->buffer[this->index-2] == '*')
                {
                    this->state = SEARCHING;
                    ret = this->decodeGPSBuffer(this->buffer);
                    if (ret != 0) {
                        handleGPSError(ret);
                    }
                }
                //Botched data, found start again
                else if (current == '$')
                {
                    this->state = DETECTING;
                    this->tagIndex = 0;
                }
                this->index++;
                break;
            //Bogus state, return error and reset
            default:
                this->state = SEARCHING;
                //TODO: Log an error here
                handleGPSError(ER_GPS_BAD_STATE);
        }
    }
    return 0;
}
void SkyGPS::handleGPSError(int status) {
    if (status == 5)
        return;
    //TODO: handle an error here with logging etc
    Serial.print("\nError: ");
    Serial.println(status);
}
/**
 * Decodes the GPS buffer and fills the tagged packet for return
 * char* tag - tag of the GPS packet
 * TaggedPacket* packet - (output) packet to fill with GPS data or error
 * unsigned char* buffer - buffer holding GPS data
 * return 0 on success, negative on error, 1 on no packet
 */
int SkyGPS::decodeGPSBuffer(unsigned char* buffer)
{
    int i = 0;
    unsigned char checksum = 0;
    //Currently we are only decoding the GPGSA tag
    if (strcmp(this->tag,"GPGGA") != 0)
    {
        return ER_GPS_UNIMPLEMENTED_PACKET;
    }
    Serial.print(this->tag);
    for (int j = 0; j < 50; j++) {
        Serial.print((char) buffer[j]);
    }
    Serial.println();
    //Check checksum
    while (i < GPS_BUFFER_SIZE && buffer[i] != '*')
    {
        checksum ^= buffer[i];
        i++;
    }
    if (i == GPS_BUFFER_SIZE)
    {
        //TODO: log error
        return ER_GPS_NOT_LOCKED;
    }
    checksum = checksum ^ 'G'^'P'^'G'^'G'^'A';
    checksum -= (buffer[i+1] - (buffer[i+1] >= 65 ? 55 : 48)) << 4;
    checksum -= buffer[i+2] - (buffer[i+2] >= 65 ? 55 : 48);
    if (checksum != 0)
    {
        return ER_GPD_BAD_CHECKSUM;
    }
    //Buffer should start with comma, then 10 digits of time
    buffer++;
    if (*(buffer) == ',')
    {
        return ER_GPS_NOT_LOCKED;
    }
    memcpy((void *)this->packets[this->packetCount].time,buffer,GPS_TIME_SIZE);
    buffer += GPS_TIME_SIZE;
    buffer++;
    //Read lat and long
    buffer = (unsigned char*)SkyGPS::decodeDegHHMM((char*)buffer,this->packets[this->packetCount].latitude);
    if (buffer == NULL)
    {
        return ER_GPS_BAD_DATA_ERROR;
    }
    buffer = (unsigned char*)SkyGPS::decodeDegHHMM((char*)buffer,this->packets[this->packetCount].longitude);
    if (buffer == NULL)
    {
        return ER_GPS_BAD_DATA_ERROR;
    }
    //Ignore lock flag, read satellites count
    buffer += 2;
    this->packets[this->packetCount].satellites = 0;
    this->packets[this->packetCount].satellites = (*buffer -48)*10 + (*(buffer+1) - 48);
    buffer += 3;
    //Look for buffer after comma to read MSL altitude
    i = 0;
    while (buffer[i++] != ',' && i < GPS_MAXIMUM_HDOP);
    this->packets[this->packetCount].altitude = (float)atof((char*)buffer+i); //TODO: replace w/ length-safe version
    this->packetCount++;
    return 0;
}
/**
 * Decodes the Degrees Hours Minutes portion of the buffer and
 * return the address after this section is consumed
 * unsigned char* buffer - buffer to decode from
 * float value - (output) value to fill with decoded value
 * return - NULL on error, or address after the DegHHMM portion
 */
char* SkyGPS::decodeDegHHMM(char* buffer, float& value)
{
    int i = 0,tmp = 0;
    //Safely find '.' or error
    while (buffer[i++] != '.' && i < GPS_MAXIMUM_DEGHHMM);
    if (i >= GPS_MAXIMUM_DEGHHMM || i < 2)
    {
        return NULL;
    }
    //Convert minutes, and fractional minutes to degrees
    value =(float) atof(buffer + i-2)/60.0f; //TODO: replace w/ length-safe version
    //Calculate integral degrees and add them to fractional
    i -= 3;
    while (i > 0)
    {
        tmp *= 10;
        tmp += (*buffer - 48);
        buffer++; i--;
    }
    value += tmp;
    //Safely find  "," or error
    i = 0;
    while (buffer[i++] != ',' && i < GPS_MAXIMUM_DEGHHMM+3);
    if (i >= GPS_MAXIMUM_DEGHHMM+3)
    {
        return NULL;
    }
    //Negate value based on hemisphere
    value *= (buffer[i] == 'S' || buffer[i] == 'W')? -1 : 1;
    //Return buffer after trailing comma
    buffer += i+2;
    return buffer;
}
