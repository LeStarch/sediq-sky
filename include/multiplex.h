/*
 * multiplex.h
 *
 *  Contains the definitions for the priority-multiplexer used
 *  to read packets in a priority order from multiple sources.
 *
 *  Created on: Mar 18, 2016
 *      Author: starchmd
 */

#ifndef MULTIPLEX_H_
#define MULTIPLEX_H_
	#include <packet.h>
	//Number of sources for the multiplexer
	#define NUM_SOURCES 5
	/**
	 * Function pointer to a packet reading function
     * TaggedPacket - (output) pointer to packet to fill
     * return - 0 on success, 1 no packets to read, negative on error
     */
	typedef int (*PacketSource)(TaggedPacket*);
	/**
	 * Holds the state of the multiplexer, sources are assumed to be in
	 * priority order
	 */
	typedef struct
	{
		PacketSource sources[NUM_SOURCES];
	} MultiplexerState;
	/**
	 * Reads the next packet from the sources defined in multiplexer state.
	 * Prefer packets from sources with higher-priority.
	 * MultiplexerState* state - pointer to multiplexer state data
	 * TaggedPacket* packet - (output)address to fill with packet
	 * return - 0 on success, 1 on no packet read, negative on error
	 */
	int readNextPacket(TaggedPacket* packet, MultiplexerState* state);

#endif /* MULTIPLEX_H_ */
