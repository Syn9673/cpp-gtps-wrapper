#ifndef GTPS_H
#define GTPS_H

typedef unsigned short enet_uint16;
typedef unsigned int enet_uint32;
typedef unsigned char enet_uint8;
typedef unsigned char BYTE;

struct GamePacket
{
	BYTE* data;
	int len;
	int indexes;
};

class Gtps
{
	public:
		Gtps();

		int ch2n(char x);
		GamePacket appendString(GamePacket p, std::string s);
		GamePacket createPacket();
		GamePacket packetEnd(GamePacket p);
		GamePacket appendInt(GamePacket p, int val);
		GamePacket appendIntx(GamePacket p, int val);
};

#endif GTPS_H