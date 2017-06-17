/*
 * main.cpp
 *
 *  Created on: Oct 15, 2016
 *      Author: starchmd
 */
#include <Arduino.h>
#include <SPI.h>

#include "../lib/sky-radio/sky-radio.h"
#include "sky-gps.h"

SkySensor::SkyGPS sky(&Serial2);

void loop()
{
    unsigned char buffer[1024];
    SkySensor::SkyGPS::GPSPacket packet;
    size_t available;
    sky.detect(100, available);
    if (available > 0) {
        sky.getPacket(buffer, sizeof(packet));
        packet.deserialize(buffer);
        Serial.print("Latitude:");
        Serial.println(packet.latitude);
        Serial.print("Longitude:");
        Serial.println(packet.longitude);
        Serial.print("Altitude:");
        Serial.println(packet.altitude);
        Serial.print("Satellite count:");
        Serial.println(packet.satellites);
        Serial.print("Time:");
        for (int i = 0; i < 9; i++) {
            Serial.print(packet.time[i]);
        }
        Serial.println();
    }
}

void setup() {
    Serial.begin(9600);
    Serial2.begin(9600);
    delay(500);

}
