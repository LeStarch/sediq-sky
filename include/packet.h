/*
 * packet.h
 *
 * Holds definitions relating to transmission packets
 *
 *  Created on: Mar 18, 2016
 *      Author: starchmd
 */

#ifndef PACKET_H_
#define PACKET_H_
	/**
	 * A enumeration of packet types. One enumeration entry
	 * should be added for each new packet type.
	 */
	typedef enum
	{
		GPSPacketType,
		AltitudePacketType,
		ErrorPacketType,
		MiscPacketType
	} PacketType;
	/**
	 * A sub-packet used to hold GPS data
	 */
	typedef struct
	{
		float latitude;
		float longitude;
		float altitude;
		unsigned char time[10];
		unsigned char satellites;
	} GPSPacket;
	/**
	 * A sub-packet used to hold altitude data
	 */
	typedef struct
	{
		unsigned char data[512];
	} AltitudePacket;
	/**
	 * A sub-packet used to hold error data
	 */
	typedef struct
	{
		int code;
		unsigned char message[512];
	} ErrorPacket;
	/**
	 * A sub-packet used to hold unclassified data
	 */
	typedef struct
	{
		unsigned char data[512];
	} MiscPacket;
	/**
     * A generalization of the various packet types used for treating all
     * of the different packet types in a uniform way.
     *
     * Note on the use of union: The union is used here to homogenize
     * different packet types into a single type. Combined with the tagging
     * from the TaggedPacket (below) it will allow different packets to be used safely.
     *
     * Note on memory efficiency: using a union does imply waste in memory as it uses the
     * memory footprint of the largest member, however; this is traded for the following:
     *
     *     1. avoids void* pointers
     *     2. allows use of non-dynamic memory
     *     3. allows some compile-time type safety
     */
	typedef union
	{
		GPSPacket gps;
		AltitudePacket altitude;
		ErrorPacket error;
		MiscPacket misc;
	} GenericPacket;
	/**
	 * A packet tagged with the type of generic packet it contains. Also contains the
	 * true length of the packet for use in efficient serialization.
	 */
	typedef struct
	{
		PacketType type;
		unsigned  int length;
		GenericPacket packet;
	} TaggedPacket;
	//Number of tagged packets to send per transmission packet
	#define NUM_PACKETS 100
	/**
	 * A transmission packet for sending over the RF link, composed of an array of sub-packets
	 * and the total length for transmission.
	 *
	 * Note: length contains the real data length (a summation of the length property of the
	 * child tagged packets) and not the NUM_PACKETS * sizeof(TaggedPacket)
	 */
    typedef struct
    {
    	unsigned int length;
    	TaggedPacket packets[NUM_PACKETS];
    } TransmissionPacket;

#endif /* PACKET_H_ */
