/*
 * sky-comm.cpp
 *
 *  Created on: Jun 17, 2017
 *      Author: starchmd
 */
#include "comm.h"
using namespace Sediq;

//Basic initialization does nothing
int Comm::init()
{
    return 0;
}
//Put the device to sleep
int Comm::sleep()
{
    return 0;
}
//Resume the device
int Comm::resume()
{
    return 0;
}
//Destructor required
Comm::~Comm() {}
