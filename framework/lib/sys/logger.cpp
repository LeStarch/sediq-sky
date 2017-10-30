/*
 * sky-logger.cpp
 *
 *  Created on: Sep 29, 2017
 *      Author: starchmd
 */

#include <Arduino.h>
#include <stdarg.h>
#include <stdio.h>
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
    bool proc = 0;
    const char* prefix = NULL;
    long lValue;
    int iValue;
    double dValue;
    //Ignore messages at lower levels
    if (this->level > level)
    {
        return;
    }
    //Figure out prefix
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
    Serial.write(prefix, strnlen(prefix, SKY_LOGGER_BUFFER_MAX_LEN));
    Serial.write(' ');
    va_start(args, message);
    prefix = message;
    while (*prefix != '\0' && prefix < message + SKY_LOGGER_BUFFER_MAX_LEN)
    {
        //Process
        if (proc && *prefix == 'd') {
            iValue = va_arg(args, int);
            Serial.print(iValue);
        } else if (proc && *prefix == 'f') {
            dValue = va_arg(args, double);
            Serial.print(dValue, 6);
        } else if (proc && *prefix == 'l') {
            lValue = va_arg(args, long);
            Serial.print(lValue);
        } else if (proc || *prefix != '%') {
            Serial.write(prefix, 1);
        }
        proc = !proc && *prefix == '%';
        prefix = prefix + 1;
    }
    va_end(args);
    Serial.write('\n');
    Serial.flush();
}

Logger* Logger::getLogger(Level level)
{
    static Logger log(level);
    return &log;
}
