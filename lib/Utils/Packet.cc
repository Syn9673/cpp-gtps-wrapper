#include <iostream>
#include <enet/enet.h>
#include "napi.h"
#include <cstring>
#include "../Structs/GamePacket.hpp"
#include "Packet.hpp"

typedef unsigned char BYTE;

using namespace std;
using namespace Napi;

// credits gt noobs
Packet::Packet()
{}

void Packet::sendRawPacket(ENetPeer* peer, ENetHost* server, string data)
{
	enet_peer_send(peer, 0, enet_packet_create(&data, data.size(), ENET_PACKET_FLAG_RELIABLE));
	enet_host_flush(server);
}

void Packet::sendConsoleMessage(ENetPeer* peer, GamePacket createdPacket, string data)
{
	GamePacket p = PacketEnd(AppendString(AppendString(createdPacket, "OnConsoleMessage"), data));
	ENetPacket* packet = enet_packet_create(p.data,
		p.len,
		ENET_PACKET_FLAG_RELIABLE);

	enet_peer_send(peer, 0, packet);
	delete p.data;
}

// credits gt noobs
GamePacket Packet::AppendString(GamePacket packet, string str)
{
	BYTE* n = new BYTE[packet.len + 2 + str.length() + 4];
	memcpy(n, packet.data, packet.len);

	delete packet.data;

	packet.data = n;

	n[packet.len] = packet.indexes;
	n[packet.len + 1] = 2;

	int sLen = str.length();

	memcpy(n+packet.len+2, &sLen, 4);
	memcpy(n + packet.len + 6, str.c_str(), sLen);

	packet.len = packet.len + 2 + str.length() + 4;
	packet.indexes++;

	return packet;
}
// credits gt noobs
GamePacket Packet::PacketEnd(GamePacket packet)
{
	BYTE* n = new BYTE[packet.len + 1];
	memcpy(n, packet.data, packet.len);
	delete packet.data;
	packet.data = n;
	char zero = 0;
	memcpy(packet.data+packet.len, &zero, 1);
	packet.len += 1;
	//*(int*)(p.data + 52) = p.len;
	*(int*)(packet.data + 56) = packet.indexes;//p.len-60;//p.indexes;
	*(BYTE*)(packet.data + 60) = packet.indexes;
	//*(p.data + 57) = p.indexes;
	return packet;
}