/*
 * SkyExecutor.cpp
 *
 *  Created on: Sep 29, 2017
 *      Author: starchmd
 */
#include <string.h>
#include <Arduino.h> //Required for snprintf

#include "executor.h"

using namespace Sediq;

Executor::Executor(const char* name)
{
    //Must copy name in to protect ownership
    strncpy(this->name, name, SKY_EXECUTOR_MAX_NAME_SIZE);
}
Executor::~Executor() {}
int Executor::error(int status, uint8_t* buffer, size_t& size)
{
    size = snprintf((char*)buffer, size, "[ERROR] Executor, %s, errored with status: %d\n", this->name, status);
    return 0;
}
