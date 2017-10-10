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
    mode = 0;
    pinMode(RADIO_SELECT_PIN, OUTPUT);
    pinMode(RADIO_POWER_PIN, OUTPUT);
    pinMode(RADIO_FIFO_NOT_EMPTY_PIN, INPUT);
    pinMode(RADIO_FIFO_NOT_FULL_PIN, INPUT);
    //Disable slave select for initialization
    digitalWrite(RADIO_SELECT_PIN, HIGH);
    SPI.begin();
    this->switchMode(mode);

}
int Radio::switchMode(int mode) {
    return 1;
}
int Radio::send(uint8_t* data, size_t& size)
{
    int status = 0;
    bool (*func)() = &Radio::fifoNotFull;
    if (mode == 0) {
        status = this->switchMode(1);
        if (status != 0) {
            return status;
        }
    }
    //TODO: set "go"?
    //TODO: send errors?
    return this->spi(SKY_RADIO_FIFO_REG, data, size, 1, func);
}
int Radio::recv(uint8_t* data, size_t& size)
{
    int status = 0;
    bool (*func)() = &Radio::fifoNotEmpty;
    if (mode == 1) {
        status = this->switchMode(1);
        if (status != 0) {
            return status;
        }
    }
    //TODO: recv errors?
    return this->spi(SKY_RADIO_FIFO_REG, data, size, 0, func);
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
bool Radio::fifoNotFull() {
    return (bool)digitalRead(RADIO_FIFO_NOT_FULL_PIN);
}
bool Radio::fifoNotEmpty() {
    return (bool)digitalRead(RADIO_FIFO_NOT_EMPTY_PIN);
}
int Radio::writeReg(uint8_t reg, uint8_t& inout) {
    size_t size = 1;
    return spi(reg, &inout, size, 1);
}
int Radio::readReg(uint8_t reg, uint8_t& inout) {
    size_t size = 1;
    return spi(reg, &inout, size, 0);
}
int Radio::spi(uint8_t reg, uint8_t* data, size_t& size, bool write) {
    return this->spi(reg, data, size, write, NULL);
}
int Radio::spi(uint8_t reg, uint8_t* data, size_t& size, bool write, bool (*cont)())
{
    size_t i = 0;
    //Setup the SPI transaction
    SPISettings settings(RADIO_SPI_FREQ, MSBFIRST, SPI_MODE0);
    SPI.beginTransaction(settings);
    //Set the pin low send address, and then data
    digitalWrite(RADIO_SELECT_PIN, LOW);
    //Write addresses have leading 1
    reg = reg | (write<<7);
    SPI.transfer(reg);
    for (i = 0; i < size; i++) {
        SPI.transfer(data + i, 1);
        //Check if we need to stop the transfer
        if (cont != NULL && !cont()) {
            break;
        }
    }
    size = i;
    digitalWrite(RADIO_SELECT_PIN, HIGH);
    //End the transaction
    SPI.endTransaction();
    return 0;
}
