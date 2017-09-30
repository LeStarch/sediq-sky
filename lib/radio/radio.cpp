/*
 * radio.cpp
 *
 *  Created on: Oct 15, 2016
 *      Author: starchmd
 */
#include "logger.h"
#include <radio.h>
#include <SPI.h>
#include "Arduino.h"

using namespace Sediq;

Radio::Radio()
{
    pinMode(RADIO_SELECT_PIN, OUTPUT);
    pinMode(RADIO_POWER_PIN, OUTPUT);
    //Disable slave select for initialization
    digitalWrite(RADIO_SELECT_PIN, HIGH);
    SPI.begin();

}
int Radio::send(uint8_t* data, size_t& size)
{
    Logger::getLogger(Logger::DEBUG)->log(Logger::INFO, (char*)data);
    return 0;
}
int Radio::recv(uint8_t* data, size_t& size)
{
    return 0;
}
int Radio::resume()
{
    uint8_t mode = SKY_RADIO_STANDBY << SKY_RADIO_SHIFT;
    this->writeReg(SKY_RADIO_MODE_REG, mode);
    return 0;
}

int Radio::sleep()
{
    //Switch this to sleep the radio
    digitalWrite(RADIO_POWER_PIN, LOW);
    return 0;
}
bool Radio::ready()
{
    return 0;
}
int Radio::writeReg(uint8_t reg, uint8_t& inout) {
    return spi(reg, &inout, 1, 1);
}
int Radio::readReg(uint8_t reg, uint8_t& inout) {
    return spi(reg, &inout, 1, 0);
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
