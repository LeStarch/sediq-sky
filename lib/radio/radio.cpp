/*
 * radio.cpp
 *
 *  Created on: Oct 15, 2016
 *      Author: starchmd
 */
#include <SPI.h>
#include "Arduino.h"
#include "radio.h"

Radio::Radio()
{
    pinMode(RADIO_SELECT_PIN, OUTPUT);
    pinMode(RADIO_POWER_PIN, OUTPUT);
    //Disable slave select for initialization
    digitalWrite(RADIO_SELECT_PIN, HIGH);
    SPI.begin();

}

int Radio::powerOn()
{
    digitalWrite(RADIO_POWER_PIN, HIGH);
    return 0;
}

int Radio::powerOff()
{
    digitalWrite(RADIO_POWER_PIN, LOW);
    return 0;
}

Radio::Status status()
{
    return Radio::OFF;
}


int Radio::spi(uint8_t reg, uint8_t* data, size_t size, bool write)
{
    //Setup the SPI transaction
    SPISettings settings(RADIO_SPI_FREQ, MSBFIRST, SPI_MODE0);
    SPI.beginTransaction(settings);
    //Set the pin low send address, and then data
    digitalWrite(RADIO_SELECT_PIN, LOW);
    //Write addresses have leading 1
    reg = reg | (write<<7);
    SPI.transfer(reg);
    SPI.transfer(data, size);
    digitalWrite(RADIO_SELECT_PIN, HIGH);
    //End the transaction
    SPI.endTransaction();
    return 0;
}
