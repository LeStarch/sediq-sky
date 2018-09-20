/*
 * SkyMain.cpp
 *
 *  Created on: Sep 29, 2017
 *      Author: starchmd
 */

#include <Arduino.h>
#include "sky-main.h"
#include "logger.h"

using namespace Sediq;

SkyMain::SkyMain()
{
    LOGGER = Logger::getLogger(Logger::DEBUG);
}

SkyMain::~SkyMain()
{}
char* TEST_MESSAGE = "Hello World, Bro!";
/**
 * Main program consisting of a program loop, and downlinking capability
 */
void SkyMain::main(Comm& comm, Executor* executors, size_t count)
{
    int status = 0;
    size_t i = 0;
    while(1)
    {
        LOGGER->log(Logger::INFO, "Executing %d executors", count);
        for (i = 0; i < count; i++)
        {
            status = this->execute(comm, executors[i]);
            if (status != 0) {
                LOGGER->log(Logger::WARNING, "Communications failure: %d", status);
            }
        }
        delay(1000);
    }
}

int SkyMain::execute(Comm& comm, Executor& executor)
{
    size_t size = SKY_MAIN_BUFFER_SIZE;
    int status = 0; /* = executor.execute(this->buffer, size);
    if (status != 0) {
        size = this->bufferSize;
        executor.error(status, this->buffer, size);
    }*/
    memcpy(this->buffer, TEST_MESSAGE, strlen(TEST_MESSAGE) + 1);
    size = strlen(TEST_MESSAGE) + 1;
    if (size > 0) {
        LOGGER->log(Logger::INFO, "Sending message of size: %d", size);
        status = comm.send(this->buffer, size);
        LOGGER->log(Logger::INFO, "Comm failure: %d", status);
    }
    return status;
}
