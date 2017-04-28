/*
 * sky-sensor.h
 *
 *  Created on: Apr 28, 2017
 *      Author: starchmd
 */
#include <stdint.h>
#include <stdlib.h>

#ifndef LIB_SKY_SENSOR_SKY_SENSOR_H_
#define LIB_SKY_SENSOR_SKY_SENSOR_H_
    /**
     * Namespace for SkySensor implementors and related software
     */
    namespace SkySensor {
        /**
         * A virtual class used to represent a sensor for the sediq-sky package.
         * This enables the system to handle packets in a generic way, and allow
         * users to simply wrap a sensor in a child-class for use in this system.
         *
         * @author starchmd
         */
        class SkySensor {
            public:
                //Default timeout is 50ms
                const uint32_t DEFAULT_TIMEOUT = 50;
                /**
                 * Run this sensor and capture available packets returning the
                 * number of available packets that can be read. A default
                 * timeout will be used.
                 * \param size: [out] number of packets available
                 * \return: 0 on success or something else on error
                 */
                int detect(size_t& size);
                /**
                 * Run the sensor and capture packets returning the number of
                 * packets available for a call to getPacket. Note: this
                 * function should return after timeout milliseconds even if
                 * it means no packets are available.
                 * \param timeout: timeout in milliseconds
                 * \param size: [out] number of packets available
                 * \return: 0 on success or something else on error
                 */
                virtual int detect(uint32_t timeout, size_t& size);
                /**
                 * Get a packet from this sensor written into the buffer
                 * provided. If the packet is larger than the size provided the
                 * sensor will return no data with a size of zero. Note: this
                 * function will not read the sensor, but only fill a packet
                 * \param buffer: buffer to fill
                 * \param size: [in/out] maximum packet size/size actually written
                 * \return: 0 on success or something else on error
                 */
                virtual size_t getPacket(uint8_t* buffer, size_t size);
                /**
                 * C++ style note: virtual classes must define a virtual destructor
                 * to ensure base-references properly destroy child classes
                 */
                virtual ~SkySensor();
        };
    }; //End namespace
#endif /* LIB_SKY_SENSOR_SKY_SENSOR_H_ */
