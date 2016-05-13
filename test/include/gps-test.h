/*
 * testing.h
 *
 * Defines the definitions required for testing
 *
 *  Created on: Mar 18, 2016
 *      Author: starchmd
 */

#ifndef TESTING_H_
#define TESTING_H_
	#include <stdio.h>
	#define SEDIQ_TESTING
	/**
	 * Structure used to mimick Serial Reader using C file IO
	 */
	typedef struct
	{
		FILE* fp;
		int (*read)(unsigned char*,int);
	} FileFauxSerial;
	/**
	 * Function to setup the faux-serial
	 * char* file - filename to read for input
	 */
	void setupFauxSerial(char* file);
	/**
	 * A fake reading function bridging c file IO to
	 * the read like interface
	 * unsigned char* buffer - (output) buffer to write to
	 * int count - number of bytes to read
	 * return number of bytes read
	 */
	int fauxRead(unsigned char* buffer,int count);
	extern FileFauxSerial Faux;
	#define GPS_SERIAL Faux

#endif /* TESTING_H_ */
