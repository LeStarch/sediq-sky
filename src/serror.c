/*
 * serror.c
 *
 * Holds implementation for error files
 *
 *  Created on: Mar 18, 2016
 *      Author: starchmd
 */
#include <string.h>

#include "serror.h"
/**
 * Fills a tagged packet to be an error packet
 * int code - error code to use
 * char* message - message to use
 * TaggedPacket* packet - (output) packet to fill
 */
void fillErrorPacket(int code, char* message, TaggedPacket* packet)
{
	strcpy(packet->packet.error.message,message);
	packet->packet.error.code = code;
	packet->type = ErrorPacketType;
	packet->length = sizeof(ErrorPacket);
}
