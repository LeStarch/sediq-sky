/*
 * main.cpp
 *
 *  Created on: Oct 15, 2016
 *      Author: starchmd
 */
#include <Arduino.h>
#include <SPI.h>

#include "radio.h"



void loop()
{

}

void setup() {
    uint8_t buffer[1024];
    buffer[0] = 0xB;
    Serial.begin(9600);
    Serial.println("Starting radio test program");
    Radio radio;
    delay(1000);
    Serial.println("Powering on radio");
    radio.powerOn();
    delay(1000);
    Serial.print("Reading radio version: <");
    //radio.spi(0x3a, buffer, 1, 1);
    radio.spi(0x3a, buffer, 1, 0);
    Serial.print(buffer[0]);
    Serial.println(">");
}
