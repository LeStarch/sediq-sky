/*
 * sky-comm.h
 *
 *  Created on: Jun 17, 2017
 *      Author: starchmd
 */
#include <string.h>
#include <stdint.h>


#include "logger.h"
#include "comm.h"
#ifndef TEST_SKY_GPS_TEST_COMM_H_
#define TEST_SKY_GPS_TEST_COMM_H_
    /**
     * A namespace for Sediq
     */
    namespace Sediq {
        namespace Test {
            /**
             * A base class for communication channels in Sediq
             */
            class GPSTestComm : public Comm {
                private:
                    Logger* LOGGER;
                public:
                    /**
                     * Initializes the member variables
                     */
                    GPSTestComm();
                    /**
                     * Initializes this communication channel
                     * \return: status of initialization
                     */
                    int init();
                    /**
                     * Put this communication device into low-power state
                     * \return: status
                     */
                    int sleep();
                    /**
                     * Put this communication device into ready state state
                     * \return: status
                     */
                    int resume();
                    /**
                     * Is the device ready to transmit or receive
                     */
                    bool ready();
                    /**
                     * Send a set of data of a given size
                     * \param data: [in] data to write out
                     * \param size: [in-out] size of bytes to send and actually sent
                     * \return: status of data returned
                     */
                    int send(uint8_t* data, size_t& size);
                    /**
                     * Receive a set of data and report size received
                     * Note: this is not intended to block
                     * \param data: [out] data received
                     * \param size: [in-out] maximum size to receive and size actually received
                     * \return: status of data returned
                     */
                    int recv(uint8_t* data, size_t& size);
                    /**
                     * Destruct this class
                     */
                    ~GPSTestComm();
            };
        };
    };
#endif /* LIB_SKY_COMM_SKY_COMM_H_ */
