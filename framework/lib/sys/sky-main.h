
/*
 * SkyMain.h
 *
 * Defines a class designed to represent the main program loop of the program, which
 * sets up a list of executors and then loops calling them each, collecting the output in a
 * List to be passed to the down-linker
 *
 *  Created on: Sep 29, 2017
 *      Author: starchmd
 */
#include "comm.h"
#include "executor.h"
#include "logger.h"

#ifndef LIB_SKY_SYS_SKYMAIN_H_
#define LIB_SKY_SYS_SKYMAIN_H_
#define SKY_MAIN_BUFFER_SIZE 4096

namespace Sediq {
    /**
     * A class representing the main-loop of a program executing executors and writing down data
     *
     * @author starchmd
     */
    class SkyMain
    {
        private:
            Logger* LOGGER;
            uint8_t buffer[SKY_MAIN_BUFFER_SIZE];
            int bufferSize = SKY_MAIN_BUFFER_SIZE;
        public:
            SkyMain();
            virtual ~SkyMain();
            /**
             * Run a program loop that consists of calling executors and downlinking the results.
             * @param SkyComm comm - [in] SkyComm used to downlink commands
             * @param SkyExecutor[] executors - [in] an array of executors to execute
             * @param size_t count - [in] number of executors
             */
            void main(Comm& comm, Executor* executors, size_t count);
            /**
             * Execute an executor and downlink the resulting data, return error
             * on error.
             * @param SkyComm comm - comm to use for downlink
             * @param SkyExecutor executor - executor to run
             * @return - 0 on SUCCESS, anything else on error
             */
            int execute(Comm& comm, Executor& executor);
    };
};

#endif /* LIB_SKY_SYS_SKYMAIN_H_ */
