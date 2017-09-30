/*
 * sky-sensor.cpp
 *
 *  Created on: Apr 28, 2017
 *      Author: starchmd
 */
#include <sensor.h>

using namespace Sediq;

int Sensor::detect(size_t& size) {
    return this->detect(Sensor::DEFAULT_TIMEOUT, size);
}
//Destructor is required even when pure virtual
Sensor::~Sensor() {}
