/*
 * sky-comm.h
 *
 *  Created on: Jun 17, 2017
 *      Author: starchmd
 */
#include <string.h>
#include <stdint.h>
#ifndef LIB_SKY_COMM_SKY_COMM_H_
#define LIB_SKY_COMM_SKY_COMM_H_
    /**
     * A namespace for Sediq
     */
    namespace Sediq {
        /**
         * A base class for communication channels in Sediq
         */
        class Comm {
            public:
                /**
                 * Initializes this communication channel
                 * \return: status of initialization
                 */
                virtual int init();
                /**
                 * Put this communication device into low-power state
                 * \return: status
                 */
                virtual int sleep();
                /**
                 * Put this communication device into ready state state
                 * \return: status
                 */
                virtual int resume();
                /**
                 * Is the device ready to transmit or receive
                 */
                virtual bool ready() = 0;
                /**
                 * Send a set of data of a given size
                 * \param data: [in] data to write out
                 * \param size: [in-out] size of bytes to send and actually sent
                 * \return: status of data returned
                 */
                virtual int send(uint8_t* data, size_t& size) = 0;
                /**
                 * Receive a set of data and report size received
                 * Note: this is not intended to block
                 * \param data: [out] data received
                 * \param size: [in-out] maximum size to receive and size actually received
                 * \return: status of data returned
                 */
                virtual int recv(uint8_t* data, size_t& size) = 0;
                /**
                 * Destruct this class
                 */
                virtual ~Comm() = 0;
        };
    };
#endif /* LIB_SKY_COMM_SKY_COMM_H_ */
