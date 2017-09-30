/*
 * main.cpp
 *
 *  Created on: Oct 15, 2016
 *      Author: starchmd
 */
#include <Arduino.h>
#include <SPI.h>

//#include "../lib/sky-radio/sky-radio.h"
#include "logger.h"
#include "gps.h"
#include "radio.h"
#include "sky-main.h"
#include "sensor-executor.h"


Sediq::GPS sky(&Serial2);
Sediq::SensorExecutor executor("GPS Executor", sky);
Sediq::Radio radio;
void loop()
{

}

void setup() {
    Sediq::Logger log(Sediq::Logger::DEBUG);
    //Serial.begin(9600);
    Serial2.begin(9600);
    delay(500);
    Sediq::SkyMain main;
    main.main(radio, &executor, 1);
}
