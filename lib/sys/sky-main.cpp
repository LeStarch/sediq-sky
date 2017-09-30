/*
 * SkyMain.cpp
 *
 *  Created on: Sep 29, 2017
 *      Author: starchmd
 */

#include "sky-main.h"
#include "logger.h"

using namespace Sediq;

SkyMain::SkyMain()
{
    LOGGER = Logger::getLogger(Logger::DEBUG);
}

SkyMain::~SkyMain()
{}

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
    }
}

int SkyMain::execute(Comm& comm, Executor& executor)
{
    size_t size = this->bufferSize;
    int status = executor.execute(this->buffer, size);
    if (status != 0) {
        size = this->bufferSize;
        executor.error(status, this->buffer, size);
    }
    status = comm.send(this->buffer, size);
    return status;
}
