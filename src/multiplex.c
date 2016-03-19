#include <multiplex.h>
/**
 * Reads the next packet from the sources defined in multiplexer state.
 * Prefer packets from sources with higher-priority.
 * MultiplexerState* state - pointer to multiplexer state data
 * TaggedPacket* packet - (output)address to fill with packet
 * return - 0 on success, 1 on no packet read, negative on error
 */
int readNextPacket(TaggedPacket* packet, MultiplexerState* state)
{
	//Currently assumes sorted list
	int i = 0,ret = 0;
	for (i = 0; i < NUM_SOURCES; i++)
	{
		ret = state->sources[i](packet);
		if (ret == 0)
		{
			return ret;
		}
	}
	return 1;
}
