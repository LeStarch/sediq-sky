/*
 * main.cpp
 *
 *  Created on: Oct 15, 2016
 *      Author: starchmd
 */
#include <Arduino.h>
#include "string.h"
#include "logger.h"
#include "radio.h"

#define BUFFER_SIZE 1024
#define POWER_LED 13

Sediq::Radio radio;
Sediq::Logger* LOG;

/**
 * A basic embedded ground loop, reads messages
 * and writes out all of the messages to the log
 * using the INFO level
 */
void loop()
{
    size_t size = BUFFER_SIZE;
    uint8_t buffer[BUFFER_SIZE];
    memset(buffer, 0, size);
    radio.recv(buffer, size);
    LOG->log(Sediq::Logger::INFO, "(%d):%s", size, buffer);
}
/**
 * Setup the system, and turn on the radio
 */
void setup() {
    LOG = Sediq::Logger::getLogger(Sediq::Logger::DEBUG);
    delay(500);
    LOG->log(Sediq::Logger::DEBUG, "Beginning main program");
    radio.resume();
    digitalWrite(POWER_LED, HIGH);
}
