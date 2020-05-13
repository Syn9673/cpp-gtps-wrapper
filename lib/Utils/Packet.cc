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

void Packet::sendStringPacket(ENetPeer* peer, ENetHost* server, string data)
{
	enet_peer_send(peer, 0, enet_packet_create(data.c_str(), data.size(), ENET_PACKET_FLAG_RELIABLE));
	enet_host_flush(server);
}