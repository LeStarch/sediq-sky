/*
 * sky-logger.h
 *
 *  Created on: Sep 29, 2017
 *      Author: starchmd
 */

#ifndef LIB_SKY_SYS_SKY_LOGGER_H_
#define LIB_SKY_SYS_SKY_LOGGER_H_

#define SKY_LOGGER_BUFFER_MAX_LEN 1024

namespace Sediq
{
    /**
     * A logger class that enables bare-bones logging to the Arduino
     * Serial port for testing purposes.
     * TODO: abstract this to support multiple logging vectors (comm, serial, disk)
     */
    class Logger
    {
        public:
            /**
             * An enumeration of levels for this particular logger
             */
            enum Level {
                DEBUG = 0,
                INFO = 1,
                WARNING = 2,
                FATAL = 3,
                OFF = 4
            };
        private:
            Level level;
            char buffer[SKY_LOGGER_BUFFER_MAX_LEN];
        public:
            /**
             * Constructor of this logger
             * @param Level level - [in] level below which messages will be ignored
             */
            Logger(Level level);
            /**
             * Destructor
             */
            virtual ~Logger();
            /**
             * Log a message to this logger
             * @param Level level - [in] level of this message
             * @param char* message - [in] message to log
             */
            void log(Level level, const char* message, ...);
            /**
             * Gets a logger instance allowing for just one logger to be active
             * @return - logger to use
             */
            static Logger* getLogger(Level level);
    };
}
#endif /* LIB_SKY_SYS_SKY_LOGGER_H_ */
