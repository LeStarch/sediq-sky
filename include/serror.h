/*
 * serror.h
 *
 * Error codes for the project
 *
 *  Created on: Mar 18, 2016
 *      Author: starchmd
 */

#ifndef SERROR_H_
#define SERROR_H_
	#include <packet.h>

	//GPS Error codes
	#define ER_GPS_MALFORMED_DATA 0xFFFFFA00
	#define ER_GPS_MALFORMED_DATA_MSG "GPS GGA data frame is malformed"

	#define ER_GPS_BAD_STATE 0xFFFFFA01
	#define ER_GPS_BAD_STATE_MSG "GPS State machine in invalid state, resetting"

	#define ER_GPS_NOT_LOCKED 0xFFFFFA02
	#define ER_GPS_NOT_LOCKED_MSG "GPS has not acquired satellite lock"

	#define ER_GPS_NOT_CHECKSUM 0xFFFFFA03
	#define ER_GPS_NOT_CHECKSUM_MSG "GPS GGA data frame as bad checksum"

	/**
	 * Fills a tagged packet to be an error packet
	 * int code - error code to use
	 * char* message - message to use
	 * TaggedPacket* packet - (output) packet to fill
	 */
	void fillErrorPacket(int code, char* message, TaggedPacket* packet);

#endif /* SERROR_H_ */
