/*
 * radio.h
 *
 *  Created on: Oct 15, 2016
 *      Author: starchmd
 */
#include <stdint.h>
#include "comm.h"
#include "radio-register-map.h"
#ifndef RADIO_H_
#define RADIO_H_
    #define RADIO_SPI_FREQ 5000000
    #define RADIO_SELECT_PIN 10
    #define RADIO_POWER_PIN 3

    #define SKY_RADIO_STANDBY 0x01
    #define SKY_RADIO_TRANSMIT 0x03
    #define SKY_RADIO_RECEIVE 0x4
    #define SKY_RADIO_SHIFT 2

    namespace Sediq {
        /**
         * A class handling the communications with the radio component of Sediq-Sky. The
         * radio is an SPI controlled radio capable of +20Db transmissions at 440MHz,
         * although other models at different frequencies are avilable.
         *
         * @author lestarch
         */
        class Radio : public Comm
        {
            public:
                /**
                 * Build and initialize this radio, by setting the pin modes of
                 * relevant pins and setting the SS pin high
                 */
                Radio();
                /**
                 * Power the radio on
                 */
                int resume();
                /**
                 * Check the status of the radio
                 */
                bool ready();
                /**
                 * Turn the radio back off
                 */
                int sleep();

                int send(uint8_t* data, size_t& size);
                int recv(uint8_t* data, size_t& size);
            private:
                /**
                 * Write the radio's SPI register
                 * \param reg: register address to write to
                 * \param inout: dat to read and result of data
                 * \return: status
                 */
                int writeReg(uint8_t reg, uint8_t& inout);
                /**
                 * Read the radio's SPI register
                 * \param reg: register address to write to
                 * \param inout: dat to read and result of data
                 * \return: status
                 */
                int readReg(uint8_t reg, uint8_t& inout);
                /**
                 * Send or recieve data from the SPI using burst-mode transmission
                 * where the address is written one time, and then a block of data is
                 * written or read.
                 * \param reg: register address to read/write to
                 * \param data: data buffer to send from AND receive from.
                 *              Note: this data will be destroyed
                 * \param size: size of data to read/write
                 * \param bool: write 1 if this is a write call, and 0 if a read
                 *              Note: when writing, the previous value is "read"
                 */
                int spi(uint8_t reg, uint8_t* data, size_t size, bool write);
        };
    };
#endif /* RADIO_H_ */
