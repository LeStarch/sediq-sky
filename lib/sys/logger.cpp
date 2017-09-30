/*
 * sky-logger.cpp
 *
 *  Created on: Sep 29, 2017
 *      Author: starchmd
 */

#include <Arduino.h>
#include <string.h>

#include "logger.h"

using namespace Sediq;

Logger::Logger(Level level)
{
    this->level = level;
    Serial.begin(115200);
}

Logger::~Logger()
{
    Serial.end();
}

void Logger::log(Level level, const char* message, ...) {
    va_list args;
    //Ignore messages at lower levels
    if (this->level > level)
    {
        return;
    }
    //Figure out prefix
    const char* prefix = NULL;
    switch (level)
    {
        case DEBUG:
            prefix = "[DEBUG]";
            break;
        case INFO:
            prefix = "[INFO]";
            break;
        case WARNING:
            prefix = "[WARNING]";
            break;
        case FATAL:
            prefix = "[FATAL]";
            break;
        default:
            prefix = "[UNKNOWN]";
            break;
    }
    //Possibly evil
    va_start(args, message);
    vsnprintf(this->buffer, SKY_LOGGER_BUFFER_MAX_LEN, message, args);
    va_end(args);
    Serial.write(prefix, strnlen(prefix, SKY_LOGGER_BUFFER_MAX_LEN));
    Serial.write(' ');
    Serial.write(this->buffer, strnlen(message, SKY_LOGGER_BUFFER_MAX_LEN));
    Serial.write('\n');
    Serial.flush();
}

Logger* Logger::getLogger(Level level)
{
    static Logger log(level);
    return &log;
}
