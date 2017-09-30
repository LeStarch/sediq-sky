/*
 * sky-gps.h
 *
 *  Created on: Apr 28, 2017
 *      Author: starchmd
 */
#include <sensor.h>
#include <Arduino.h>
#ifndef LIB_SKY_GPS_SKY_GPS_H_
#define LIB_SKY_GPS_SKY_GPS_H_
    #define GPS_BUFFER_SIZE 256
    #define GPS_TAG_SIZE 5

    #define GPS_TIME_SIZE 9
    //Maximum length of DegHHMM field
    #define GPS_MAXIMUM_DEGHHMM 11
    #define GPS_MAXIMUM_HDOP 6

    #define PACKET_BUFFER_SIZE 100

    #define ER_GPS_BAD_DATA_ERROR 1
    #define ER_GPD_BAD_CHECKSUM 2
    #define ER_GPS_NOT_LOCKED 4
    #define ER_GPS_BAD_STATE 3
    #define ER_GPS_UNIMPLEMENTED_PACKET 5

    namespace Sediq {

        class GPS : public Sensor {
            public:
                /**
                 * In C++ structs are classes with all public members, here
                 * we are using it as a data packet
                 */
                struct GPSPacket {
                    unsigned long  deviceTime;
                    float latitude;
                    float longitude;
                    float altitude;
                    unsigned char time[GPS_TIME_SIZE];
                    unsigned char satellites;

                    int serialize(unsigned char* buffer, size_t size);

                    int deserialize(unsigned char* buffer);
                };
            private:
                /**
                 * State machine for reading GPS data from UART
                 */
                enum GPSMachineState
                {
                    SEARCHING,
                    DETECTING,
                    READING,
                    COMPLETE
                };

                GPSMachineState state = SEARCHING;
                HardwareSerial* serial;
                GPSPacket packets[PACKET_BUFFER_SIZE];

                uint32_t index = 0, tagIndex = 0, packetCount = 0;
                uint8_t buffer[GPS_BUFFER_SIZE];
                char tag[GPS_TAG_SIZE+1];
                /**
                 * Run the GPS state machine
                 * \param timeout: maximum time to read in ms
                 * \return: 0 on success or something else on error
                 */
                int gpsMachine(uint32_t timeout);

                int decodeGPSBuffer(unsigned char* buffer);

                static char* decodeDegHHMM(char* buffer, float& value);
                void handleGPSError(int status);
            public:
                /**
                 * Build the GPS around a serial port
                 */
                GPS(HardwareSerial* serial);
                /**
                 * Run the GPS sensor and capture packets returning the number
                 * of packets available for a call to getPacket.
                 * \param timeout: timeout in milliseconds
                 * \param size: [out] number of packets available
                 * \return: 0 on success or something else on error
                 */
                int detect(uint32_t timeout, size_t& size);
                /**
                 * Get a packet from the GPS sensor written into the buffer
                 * provided. If the packet is larger than the size provided the
                 * sensor will return no data with a size of zero. Note: this
                 * function will not read the sensor, but only fill a packet
                 * \param buffer: buffer to fill
                 * \param size: [in/out] maximum packet size/size actually written
                 * \return: 0 on success or something else on error
                 */
                size_t getPacket(uint8_t* buffer, size_t size);
                /**
                 * Get the maximum size of this packet
                 * @return: size of GPS packet
                 */
                size_t getMaxSize();
                 /**
                  * Destruct
                  */
                ~GPS();
        };
    }; //End sky sensor namespace
#endif /* LIB_SKY_GPS_SKY_GPS_H_ */
