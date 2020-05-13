#include <iostream>
#include <enet/enet.h>
#include "napi.h"
#include <map>
#include "../../Structs/GamePacket.hpp"
#include "../../Utils/Packet.hpp"
#include "../../Utils/Utils.hpp"

using namespace std;
using namespace Napi;

namespace Packets
{
	void sendStringPacket(const CallbackInfo& info)
	{
		string id = info[0].As<String>().Utf8Value();
		string data = info[1].As<String>().Utf8Value();
		
		ENetPeer* peer = Utils::getPeer(id);

		Packet::sendStringPacket(peer, Utils::getServer(), data);
	}

	void sendPacket(const CallbackInfo& info)
	{
		string id = info[0].As<String>().Utf8Value();
		Buffer<char> buffer = info[1].As<Buffer<char>>();
		int len = info[2].As<Number>().Uint32Value();

		BYTE* data = (BYTE*)buffer.Data();

		ENetPacket* packet = enet_packet_create(data,
			len,
			ENET_PACKET_FLAG_RELIABLE);

		enet_peer_send(Utils::getPeer(id), 0, packet);
	}

	void sendQuit(const CallbackInfo& info)
	{
		string id = info[0].As<String>().Utf8Value();

		ENetPeer* peer = Utils::getPeer(id);
		enet_peer_disconnect_later(peer, 0);

		Utils::peers.erase(id);
	}
}