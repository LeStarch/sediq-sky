/*
 * sensor-executor.cpp
 *
 *  Created on: Sep 29, 2017
 *      Author: starchmd
 */

#include "sensor-executor.h"

using namespace Sediq;
SensorExecutor::SensorExecutor(const char* name, Sensor& sensor)
    : Executor(name)
{
    this->sensor = &sensor;
}

SensorExecutor::~SensorExecutor()
{}

int SensorExecutor::execute(uint8_t* buffer, size_t& size)
{
    int status = 0;
    unsigned int i = 0;
    size_t num_packet = 0;
    size_t total = 0;
    size_t pack_size;

    status = this->sensor->detect(num_packet);
    if (status != 0) {
        size = 0;
        return status;
    }
    for (i = 0; i < num_packet && (total + this->sensor->getMaxSize()) < size; i++)
    {
        pack_size = size - total;
        status = this->sensor->getPacket(buffer + total, pack_size);
        if (status != 0)
        {
            size = total;
            return status;
        }
        total = total + pack_size;
    }
    size = total;
    return 0;
}
