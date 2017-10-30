/*
 * sky-executor.h
 *
 *  Created on: Sep 29, 2017
 *      Author: lestarch
 */
#include <stdint.h>
#ifndef LIB_SKY_SYS_SKY_EXECUTOR_H_
#define LIB_SKY_SYS_SKY_EXECUTOR_H_

#define SKY_EXECUTOR_MAX_NAME_SIZE 128

namespace Sediq
{
    /**
     * An executor class designed to wrap a execute-code-produce-data block
     * enabling a generic program loop.
     *
     * @author starchmd
     */
    class Executor
    {
        private:
            char buffer[];
            char name[SKY_EXECUTOR_MAX_NAME_SIZE];
        public:
            Executor(const char* name);
            virtual ~Executor() = 0;
            /**
             * Executes this executor and fills the ourput into the buffer
             * @param char buffer - [in/out] buffer to write to
             * @param size_t size - [in/out] maximum size to write, size actually written
             * @return 0 on success, anything else on error
             */
            virtual int execute(uint8_t* buffer, size_t& size) = 0;
            /**
             * Generates an error from the status returned by the "execute" method
             * @param char buffer[] - [in/out] buffer to write to
             * @param size_t size - [in/out] maximum size to write, size actually written
             * @return 0 on success, anything else on error...although this will likely be
             * logged and nothing more.
             */
            virtual int error(int status, uint8_t* buffer, size_t& size);
    };
};
#endif /* LIB_SKY_SYS_SKY_EXECUTOR_H_ */
