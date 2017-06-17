/*
 * sky-sensor.cpp
 *
 *  Created on: Apr 28, 2017
 *      Author: starchmd
 */
#include <sky-sensor.h>

using namespace SkySensor;

int SkySensor::SkySensor::detect(size_t& size) {
    return this->detect(SkySensor::DEFAULT_TIMEOUT, size);
}
//Destructor is required even when pure virtual
SkySensor::SkySensor::~SkySensor() {}
