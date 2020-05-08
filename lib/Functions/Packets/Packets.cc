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

	void sendRawPacket(const CallbackInfo& info)
	{
		string id = info[0].As<String>().Utf8Value();
		Buffer<char> buffer = info[1].As<Buffer<char>>();

		GamePacket p;

		p.data = (BYTE*)buffer.Data();
		p.len = buffer.Length();
		p.indexes = 0;
		
		Packet::sendRawPacket(Utils::getPeer(id), p);
	}

	void sendPacket(const CallbackInfo& info)
	{
		string id = info[0].As<String>().Utf8Value();
		Buffer<char> buffer = info[1].As<Buffer<char>>();
		int len = info[2].As<Number>().Uint32Value();
		int indexes = info[3].As<Number>().Uint32Value();

		GamePacket p;

		p.data = (BYTE*)buffer.Data();
		p.len = len;
		p.indexes = indexes;

		Packet::sendPacket(Utils::getPeer(id), p);
	}

	void sendQuit(const CallbackInfo& info)
	{
		string id = info[0].As<String>().Utf8Value();

		ENetPeer* peer = Utils::getPeer(id);
		enet_peer_disconnect_later(peer, 0);

		Utils::peers.erase(id);
	}
}