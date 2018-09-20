/*
 * main.cpp
 *
 *  Created on: Oct 15, 2016
 *      Author: starchmd
 */
#include <Arduino.h>

#include "logger.h"
#include "gps.h"
#include "radio.h"
#include "sky-main.h"
#include "sensor-executor.h"

Sediq::GPS sky(&Serial3);
Sediq::SensorExecutor executor("GPS Executor", sky);
Sediq::Radio comm;

//Unused looping function
void loop()
{}
/**
 * Sets up the SediqMain program and runns it
 */
void setup() {
    Sediq::Logger* log = Sediq::Logger::getLogger(Sediq::Logger::DEBUG);
    Serial3.begin(9600);
    delay(500);
    Sediq::SkyMain main;
    log->log(Sediq::Logger::INFO, "Beginning main program");
    main.main(comm, &executor, 1);
}
