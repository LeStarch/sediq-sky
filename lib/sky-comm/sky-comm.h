/*
 * sky-comm.h
 *
 *  Created on: Jun 17, 2017
 *      Author: starchmd
 */

#ifndef LIB_SKY_COMM_SKY_COMM_H_
#define LIB_SKY_COMM_SKY_COMM_H_
    /**
     * A namespace for SkyComms
     */
    namespace SkyComm {
        /**
         * A base class for communication channels in Sediq
         */
        class SkyComm {
            public:
                /**
                 * Initializes this communication channel
                 * \return: status of initialization
                 */
                virtual int init();
                /**
                 * Send a set of data of a given size
                 * \param data: [in] data to write out
                 * \param size: [in-out] size of bytes to send and actually sent
                 * \return: status of data returned
                 */
                virtual int send(char* data, size_t& size) = 0;
                /**
                 * Receive a set of data and report size received
                 * Note: this is not intended to block
                 * \param data: [out] data received
                 * \param size: [in-out] maximum size to receive and size actually received
                 * \return: status of data returned
                 */
                virtual int recv(char* data, size_t& size) = 0;
                /**
                 * Destruct this class
                 */
                virtual ~SkyComm() = 0;
        };
    };
#endif /* LIB_SKY_COMM_SKY_COMM_H_ */
