/*
 * sky-gps.h
 *
 *  Created on: Apr 28, 2017
 *      Author: starchmd
 */
#include <sky-sensor.h>
#ifndef LIB_SKY_GPS_SKY_GPS_H_
#define LIB_SKY_GPS_SKY_GPS_H_
    namespace SkySensor {
        class SkyGPS : SkySensor {
            public:
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
                  * Destruct
                  */
                ~SkyGPS();
        };
    }; //End sky sensor namespace
#endif /* LIB_SKY_GPS_SKY_GPS_H_ */
