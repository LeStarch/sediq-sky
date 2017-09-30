/*
 * sensor-executor.h
 *
 *  Created on: Sep 29, 2017
 *      Author: starchmd
 */
#include "string.h"

#include "sensor.h"
#include "executor.h"

#ifndef LIB_SYS_SENSOR_EXECUTOR_H_
#define LIB_SYS_SENSOR_EXECUTOR_H_

namespace Sediq
{

    class SensorExecutor : public Executor
    {
        private:
            Sensor* sensor;
        public:
            SensorExecutor(const char* name, Sensor& sensor);
            virtual ~SensorExecutor();
            int execute(uint8_t* buffer, size_t& size);
    };

} /* namespace Sediq */

#endif /* LIB_SYS_SENSOR_EXECUTOR_H_ */
