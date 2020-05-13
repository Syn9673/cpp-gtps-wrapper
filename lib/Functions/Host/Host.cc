#include <iostream>
#include <enet/enet.h>
#include <map>
#include "napi.h"
#include "Host.hpp"
#include "../../Structs/GamePacket.hpp"
#include "../../Utils/Utils.hpp"
#include "../../Utils/Packet.hpp"

using namespace std;
using namespace Napi;

Function emit;
ENetAddress address;
	
namespace Host
{
	Number init(const CallbackInfo& info)
	{
		Env env = info.Env();
		return Number::New(env, enet_initialize());
	}

	void create(const CallbackInfo& info)
	{		
		Env env = info.Env();
		Object data = info[0].As<Object>();

		int channels = data.Get("channels").As<Number>().Uint32Value();
		int peers = data.Get("peers").As<Number>().Uint32Value();
		int ico = data.Get("ico").As<Number>().Uint32Value();
		int ogo = data.Get("ogo").As<Number>().Uint32Value();

		address.host = ENET_HOST_ANY;
		address.port = data.Get("port").As<Number>().Uint32Value();
		
		Utils::setAddress(address);
		Utils::setServer(enet_host_create(&address, channels, peers, ico, ogo));
	}

	void start(const CallbackInfo& info)
	{
		Env env = info.Env();
		ENetHost* server = Utils::getServer();

		if (server == NULL)
		{
			TypeError::New(env, "Server is null, make sure to create a host first.");
		}

		server->checksum = enet_crc32;
		enet_host_compress_with_range_coder(server);

		emit = info[0].As<Function>();

		ENetEvent event;
		while(true)
		{
			while(enet_host_service(server, &event, 1000) > 0)
			{
				ENetPacket* packet = event.packet;
				switch(event.type)
				{
					case ENET_EVENT_TYPE_CONNECT:
					{
						string peerID = Utils::getUID(event.peer);
						Utils::peers.emplace(peerID, event.peer);

						emit.Call({
							String::New(env, "connect"),
							String::New(env, peerID)
						});
						break;
					}

					case ENET_EVENT_TYPE_RECEIVE:
					{
						string peerID = Utils::getUID(event.peer);
						emit.Call({
							String::New(env, "receive"),
							ArrayBuffer::New(env, packet->data, packet->dataLength),
							String::New(env, peerID)
						});
						break;
					}
				}
			}
		}
	}
}