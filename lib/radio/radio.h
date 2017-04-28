/*
 * radio.h
 *
 *  Created on: Oct 15, 2016
 *      Author: starchmd
 */

#ifndef RADIO_H_
#define RADIO_H_
    #define RADIO_SPI_FREQ 5000000
    #define RADIO_SELECT_PIN 10
    #define RADIO_POWER_PIN 3
    /**
     * A class handling the communications with the radio component of Sediq-Sky. The
     * radio is an SPI controlled radio capable of +20Db transmissions at 440MHz,
     * although other models at different frequencies are avilable.
     *
     * @author lestarch
     */
    class Radio
    {
        public:
            /**
             * State of the radio
             */
            enum Status {
                OFF, //Radio is off
                BOOT, //Radio is turning on
                ON //Radio is on
            };
            /**
             * Build and initialize this radio, by setting the pin modes of
             * relevant pins and setting the SS pin high
             */
            Radio();
            /**
             * Power the radio on
             */
            int powerOn();
            /**
             * Check the status of the radio
             */
            Status status();
            /**
             * Turn the radio back off
             */
            int powerOff();
 //       private:
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

#endif /* RADIO_H_ */
