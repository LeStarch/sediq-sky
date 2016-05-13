/*
 * testing.c
 *
 * Contains the implementation for the testing code
 *
 *  Created on: Mar 18, 2016
 *      Author: starchmd
 */
#include "gps-test.h"
#ifdef SEDIQ_TESTING
	#include <stdio.h>


	#include "packet.h"
	#include "gps.h"

	FileFauxSerial Faux;
	/**
	 * Function to setup the faux-serial
	 * char* file - filename to read for input
	 */
	void setupFauxSerial(char* file)
	{
		Faux.fp = fopen(file,"r");;
		Faux.read = fauxRead;
	}
	/**
	 * A fake reading function bridging c file IO to
	 * the read like interface
	 * unsigned char* buffer - (output) buffer to write to
	 * int count - number of bytes to read
	 * return number of bytes read
	 */
	int fauxRead(unsigned char* buffer,int count)
	{
		int ret = 0;
		ret = fread(buffer,1,count,Faux.fp);
		fprintf(stdout,"%c",buffer[0]);
		return ret;
	}
	/**
	 * Main function for testing purposes
	 * int argc - count of arguments
	 * char** argv - values of arfuments
	 */
	int main(int argc, char** argv)
	{
		//Check argument count
		if (argc != 2)
		{
			fprintf(stderr,"Error: input-file not specified.\nUsage:\n\t%s <input file>\n",argv[0]);
			return -1;
		}
		fprintf(stdout,"Running Sediq-Sky in testing mode. Input file: %s\n",argv[1]);
		setupFauxSerial(argv[1]);
		//Loop running GPS reader
		TaggedPacket packet;
		while (readGPSPacket(&packet) <= 0)
		{
			switch (packet.type)
			{
				case ErrorPacketType:
					fprintf(stdout,"Error detected(%x): %s\n",packet.packet.error.code,packet.packet.error.message);
					break;
				case GPSPacketType:
					fprintf(stdout,"GPS Location found: (%f,%f,%f) @%.*s with %d satellites\n",
								packet.packet.gps.latitude,
								packet.packet.gps.longitude,
								packet.packet.gps.altitude,
								(int)sizeof(packet.packet.gps.time),
								packet.packet.gps.time,
								packet.packet.gps.satellites
							);
					break;
				default:
					fprintf(stderr,"Error: Unknown packet type detected\n");
			}
		}
	}
#endif

