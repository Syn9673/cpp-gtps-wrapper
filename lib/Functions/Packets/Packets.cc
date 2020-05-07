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
	void sendRawPacket(const CallbackInfo& info)
	{
		string id = info[0].As<String>().Utf8Value();
		string data = info[1].As<String>().Utf8Value();
		
		ENetPeer* peer = Utils::getPeer(id);

		Packet::sendRawPacket(peer, Utils::getServer(), data);
	}

	void log(const CallbackInfo& info)
	{
		string id = info[0].As<String>().Utf8Value();
		string msg = info[1].As<String>().Utf8Value();
		Buffer<char> buffer = info[2].As<Buffer<char>>();

		GamePacket p;

		p.data = (BYTE*)buffer.Data();
		p.len = 61;

		Packet::sendConsoleMessage(Utils::getPeer(id), p, msg);
	}

	void sendQuit(const CallbackInfo& info)
	{
		string id = info[0].As<String>().Utf8Value();

		ENetPeer* peer = Utils::getPeer(id);
		enet_peer_disconnect_later(peer, 0);

		Utils::peers.erase(id);
	}
}