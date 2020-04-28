#ifndef PACKET_H
#define PACKET_H

extern int itemsDatSize, itemdathash;

typedef unsigned short enet_uint16;
typedef unsigned int enet_uint32;
typedef unsigned char enet_uint8;
typedef unsigned char BYTE;

namespace Packets
{
	void sendConsoleMessage(ENetPeer* peer, std::string message);
	void sendDialogRequest(ENetPeer* peer, std::string message);
	void sendSpawn(ENetPeer* peer, std::string message);
	void sendWorldError(ENetPeer* peer);
	void sendLoginPacket(ENetPeer* peer);
	void sendWorldRequest(ENetPeer* peer);
	void SendPacketRaw(int a1, void *packetData, size_t packetDataSize, void *a4, ENetPeer* peer, int packetFlag);
	BYTE* packPlayerMoving(PlayerMoving* dataStruct);
	PlayerMoving* unpackPlayerMoving(BYTE* data);
	//void sendDrop(ENetPeer* peer, int netID, int x, int y, int item, int count, BYTE specialEffect, bool onlyForPeer);
}

#endif