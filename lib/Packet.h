#ifndef PACKET_H
#define PACKET_H

extern int itemsDatSize, itemdathash;

namespace Packets
{
	void sendConsoleMessage(ENetPeer* peer, std::string message);
	void sendDialogRequest(ENetPeer* peer, std::string message);
	void sendSpawn(ENetPeer* peer, std::string message);
	void sendWorldError(ENetPeer* peer);
	void sendLoginPacket(ENetPeer* peer);
	void sendWorldRequest(ENetPeer* peer);
}

#endif