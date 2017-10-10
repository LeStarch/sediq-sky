/*
 * sky-comm.cpp
 *
 *  Created on: Jun 17, 2017
 *      Author: starchmd
 */
#include "gps.h"
#include "gps-test-comm.h"


using namespace Sediq;
using namespace Sediq::Test;

GPSTestComm::GPSTestComm()
{
    this->LOGGER = Logger::getLogger(Logger::DEBUG);
}


//Basic initialization does nothing
int GPSTestComm::init()
{
    return 0;
}
//Put the device to sleep
int GPSTestComm::sleep()
{
    return 0;
}
//Resume the device
int GPSTestComm::resume()
{
    return 0;
}
bool GPSTestComm::ready()
{
    return 1;
}
//Destructor required
GPSTestComm::~GPSTestComm() {}

int GPSTestComm::send(uint8_t* data, size_t& size)
{
    GPS::GPSPacket pack;
   /* if (size != sizeof(GPS::GPSPacket)) {
        this->LOGGER->log(Logger::WARNING, "Bad GPS size %d expected %d", size, sizeof(pack));
        return 0;
    }*/
    pack.deserialize(data);
    this->LOGGER->log(Logger::INFO, "GPS Packet(%d):\n\t(%f, %f)\n\tAltitude: %f\n\tTime: %l\n\tSize: %d/%d",
            pack.satellites, pack.latitude, pack.longitude, pack.altitude, pack.deviceTime, size, sizeof(pack));
    return 0;
}
int GPSTestComm::recv(uint8_t* data, size_t& size)
{
    this->LOGGER->log(Logger::WARNING, "Receive not implemented");
    return 0;
}
