#include <iostream>
#include <map>
#include "napi.h"
#include <enet/enet.h>
#include <omp.h>
#include <fstream>
#include <cstring>
#include <vector>

#include "lib/others/PlayerMoving.h"
#include "lib/others/InventoryItem.h"
#include "lib/others/ItemSharedUID.h"
#include "lib/others/PlayerInventory.h"
#include "lib/others/PlayerInfo.h"
#include "lib/others/DroppedItem.h"
#include "lib/others/WorldItem.h"
#include "lib/others/WorldInfo.h"

#include "lib/Utils.h"
#include "lib/Gtps.h"
#include "lib/Packet.h"
#include "lib/Methods.h"

ENetAddress address;

Utils utils;
Gtps gtps;

Napi::Object addressData;
Napi::Object threadObj;

int threadCount;
std::vector<WorldInfo> worlds;

Methods methods;

int x = 3040;
int y = 736;

std::string getUID(ENetPeer* peer) {
	std::string ret;
	std::string serverID;
	int serverIDSize = serverID.length();
	ret.resize(serverIDSize + 8);
	unsigned int id = peer->connectID;
	static const char* digits = "0123456789ABCDEF";
	for (unsigned int i=0; i<serverIDSize; ++i)
		ret[i] = serverID[i];
	for (unsigned int i=0; i<8; ++i)
		ret[i+serverIDSize] = digits[(id>>(i<<2)) & 0x0f];
	return ret;
}


Napi::Value buildItemsDatabase(const Napi::CallbackInfo& args)
{
	Napi::Env env = args.Env();

	#pragma omp parallel
	#pragma omp single nowait
	{
		methods.buildItemsDatabase(args[0].As<Napi::String>().Utf8Value(), &utils);
	}

	return Napi::Value();
}

Napi::Value sendWorldError(const Napi::CallbackInfo& args)
{	
	Napi::Env env = args.Env();

	omp_set_num_threads(threadCount);

	#pragma omp parallel
	#pragma omp single nowait
	{
		methods.sendWorldError(args[0].As<Napi::String>().Utf8Value(), &utils);
	}

	return Napi::Value();
}

Napi::Value sendItemsData(const Napi::CallbackInfo& args)
{
	Napi::Env env = args.Env();

	omp_set_num_threads(threadCount);
	std::string id = args[0].As<Napi::String>().Utf8Value();

	#pragma omp parallel
	#pragma omp single nowait
	{
		methods.sendItemsData(id, &utils);
	}

	return Napi::Value();
}

Napi::Value sendLoginPacket(const Napi::CallbackInfo& args)
{
	Napi::Env env = args.Env();
	std::string id = args[0].As<Napi::String>().Utf8Value();

	omp_set_num_threads(threadCount);

	#pragma omp parallel
	#pragma omp single nowait
	{
		methods.sendLoginPacket(id, &utils);
	}

	return Napi::Value();
}

Napi::Value sendConsoleMessage(const Napi::CallbackInfo& args)
{
	Napi::Env env = args.Env();
	std::string message = args[0].As<Napi::String>().Utf8Value();
	std::string id = args[1].As<Napi::String>().Utf8Value();

	omp_set_num_threads(threadCount);

	#pragma omp parallel
	#pragma omp single nowait
	{
		methods.sendConsoleMessage(id, message, &utils);
	}

	return Napi::Value();
}

Napi::Value sendDialogRequest(const Napi::CallbackInfo& args)
{
	Napi::Env env = args.Env();
	std::string message = args[0].As<Napi::String>().Utf8Value();
	std::string id = args[1].As<Napi::String>().Utf8Value();

	omp_set_num_threads(threadCount);

	#pragma omp parallel
	#pragma omp single nowait
	{
		methods.sendDialogRequest(id, message, &utils);
	}

	return Napi::Value();
}

Napi::Value sendData(const Napi::CallbackInfo& args)
{
	Napi::Env env = args.Env();

	int num = args[0].As<Napi::Number>().Uint32Value();
	int _data = args[1].As<Napi::Number>().Uint32Value();
	int len = args[2].As<Napi::Number>().Uint32Value();
	std::string id = args[3].As<Napi::String>().Utf8Value();
	char data[10] = "";

	omp_set_num_threads(threadCount);

	#pragma omp parallel
	#pragma omp single nowait
	{
		methods.sendData(id, data, _data, num, len, &utils);
	}

	return Napi::Value();
}

Napi::Number _enet_initialize(const Napi::CallbackInfo& args)
{
	Napi::Env env = args.Env();
	int res;

	omp_set_num_threads(threadCount);

	#pragma omp parallel
	#pragma omp single nowait
	{
		res = methods._enet_initialize();
	}

	return Napi::Number::New(env, res);
}

Napi::Value hostCreate(const Napi::CallbackInfo& args)
{
	Napi::Env env = args.Env();

	if (!args[0].IsObject())
	{
		throw Napi::TypeError::New(env, "Please pass an object for the arguments.");
	}

  Napi::Object argObject = args[0].As<Napi::Object>().ToObject();
  methods.hostCreate(env, argObject, address, &utils);

	return Napi::Value();
}

Napi::Value startServer(const Napi::CallbackInfo& args)
{
	Napi::Env env = args.Env();

	if (args[0].As<Napi::Boolean>().ToBoolean())
	{
		utils.getServer()->checksum = enet_crc32;
	}

	if (args[1].As<Napi::Boolean>().ToBoolean())
	{
		enet_host_compress_with_range_coder(utils.getServer());
	}

	Napi::Function emit = args[2].As<Napi::Function>();
	ENetEvent event;

	while (true)
	{
		while (enet_host_service(utils.getServer(), &event, 1000) > 0)
		{
			switch (event.type)
			{
				case ENET_EVENT_TYPE_CONNECT:
				{
					std::string uID = getUID(event.peer);
					event.peer->data = new PlayerInfo;
					//((PlayerInfo*)(event.peer->data))->displayName = "`#@AV84";
					utils.getPeers()->insert(std::pair<std::string, ENetPeer*>(uID, event.peer));	

					emit.Call({
						Napi::String::New(env, "connect"),
						Napi::String::New(env, uID)
					});
				}
					break;

				case ENET_EVENT_TYPE_RECEIVE:
					std::string uID = getUID(event.peer);

					Napi::Object receivedData = Napi::Object::New(env);
					receivedData.Set("data", Napi::String::New(env, utils.GetTextPointerFromPacket(event.packet)));
					receivedData.Set("type", Napi::Number::New(env, utils.GetMessageTypeFromPacket(event.packet)));
					receivedData.Set("connectID", Napi::String::New(env, uID));

					emit.Call({
						Napi::String::New(env, "receive"),
						receivedData
					});
					break;
			}
		}
	}

	return Napi::Value();
}

Napi::Value sendWorldRequest(const Napi::CallbackInfo& args)
{
	Napi::Env env = args.Env();
	omp_set_num_threads(threadCount);

	std::string id = args[0].As<Napi::String>().Utf8Value();

	#pragma omp parallel
	#pragma omp single nowait
	{
		methods.sendWorldRequest(id, &utils);
	}

	return Napi::Value();
}

WorldInfo getWorld(std::string name, int width, int height)
{
	std::cout << name << std::endl;
	std::cout << worlds.size() << std::endl;
	if (worlds.size() < 1)
		return methods.generateWorld(name, width, height);
	else
	{
		std::cout << worlds.front().name;
		return worlds.front();
	}
}

Napi::Value sendWorldData(const Napi::CallbackInfo& args)
{
	Napi::Env env = args.Env();
	
	Napi::Object worldObj = args[0].As<Napi::Object>().ToObject();
	std::string id = args[1].As<Napi::String>().Utf8Value();
	WorldInfo world;

	std::string name = worldObj.Get("name").As<Napi::String>().Utf8Value();
	int width = worldObj.Get("width").As<Napi::Number>().Uint32Value();
	int height = worldObj.Get("height").As<Napi::Number>().Uint32Value();

	#pragma omp parallel
	#pragma omp single nowait
	{
		WorldInfo _world = getWorld(name, width, height);

		if (worlds.size() < 1) {
			std::cout << "world does not exists" << std::endl;
			worlds.push_back(world);
		}
		
		methods.sendWorld(utils.getPeer(id), &_world);

		for (int j = 0; j < _world.width*_world.height; j++)
		{
			if (_world.items[j].foreground == 6) {
				x = (j%_world.width) * 32;
				y = (j / _world.width) * 32;
			}
		}
	}
}

Napi::Value setThreadCount(const Napi::CallbackInfo& args)
{
	Napi::Env env = args.Env();

	threadCount = args[0].As<Napi::Number>().Uint32Value();
	return Napi::Value();
}

Napi::Value sendSpawn(const Napi::CallbackInfo& args)
{
	Napi::Env env = args.Env();
	std::string message = args[0].As<Napi::String>().Utf8Value();
	std::string id = args[1].As<Napi::String>().Utf8Value();

	omp_set_num_threads(threadCount);

	#pragma omp parallel
	#pragma omp single nowait
	{
		Packets::sendSpawn(utils.getPeer(id), message);
	}

	return Napi::Value();
}

Napi::Value sendSpawn2(const Napi::CallbackInfo& args)
{
	Napi::Env env = args.Env();
	int cId = args[0].As<Napi::Number>().Uint32Value();
	std::string id = args[1].As<Napi::String>().Utf8Value();
	//std::cout << ((PlayerInfo*)(utils.getPeer(id)->data))->displayName << std::endl;
	std::string message = "spawn|avatar\nnetID|" + std::to_string(cId) + "\nuserID|" + std::to_string(cId) + "\ncolrect|0|0|20|30\nposXY|" + std::to_string(x) + "|" + std::to_string(y) + "\nname|``" + ((PlayerInfo*)(utils.getPeer(id)->data))->displayName + "``\ncountry|" + ((PlayerInfo*)(utils.getPeer(id)->data))->country + "\ninvis|0\nmstate|0\nsmstate|0\ntype|local\n";

	omp_set_num_threads(threadCount);

	#pragma omp parallel
	#pragma omp single nowait
	{	
		//std::cout << cId << std::endl;
		Packets::sendSpawn(utils.getPeer(id), message);
		((PlayerInfo*)(utils.getPeer(id)->data))->netID = cId;
	}

	return Napi::Value();
}

Napi::Value _sendChatMessage(const Napi::CallbackInfo& args)
{
	Napi::Env env = args.Env();
	std::string message = args[0].As<Napi::String>().Utf8Value();
	std::string id = args[1].As<Napi::String>().Utf8Value();

	omp_set_num_threads(threadCount);

	#pragma omp parallel
	#pragma omp single nowait
	{
		methods.sendChatMessage(utils.getServer(), utils.getPeer(id), ((PlayerInfo*)(utils.getPeer(id)->data))->netID, message);
	}

	return Napi::Value();
}

Napi::Value onPeerConnect(const Napi::CallbackInfo& args)
{
	Napi::Env env = args.Env();
	std::string id = args[0].As<Napi::String>().Utf8Value();

	omp_set_num_threads(threadCount);

	#pragma omp parallel
	#pragma omp single nowait
	{
		methods.onPeerConnect(utils.getPeer(id), utils.getServer());
	}

	return Napi::Value();
}

Napi::Object Init(Napi::Env env, Napi::Object exports)
{
	exports.Set(Napi::String::New(env, "hostCreate"), Napi::Function::New(env, hostCreate));
	exports.Set(Napi::String::New(env, "initialize"), Napi::Function::New(env, _enet_initialize));
	exports.Set(Napi::String::New(env, "startServer"), Napi::Function::New(env, startServer));
	exports.Set(Napi::String::New(env, "sendData"), Napi::Function::New(env, sendData));
	exports.Set(Napi::String::New(env, "sendConsoleMessage"), Napi::Function::New(env, sendConsoleMessage));
	exports.Set(Napi::String::New(env, "sendDialogRequest"), Napi::Function::New(env, sendDialogRequest));
	exports.Set(Napi::String::New(env, "sendLoginPacket"), Napi::Function::New(env, sendLoginPacket));
	exports.Set(Napi::String::New(env, "buildItemsDatabase"), Napi::Function::New(env, buildItemsDatabase));
	exports.Set(Napi::String::New(env, "sendItemsData"), Napi::Function::New(env, sendItemsData));
	exports.Set(Napi::String::New(env, "sendWorldRequest"), Napi::Function::New(env, sendWorldRequest));
	exports.Set(Napi::String::New(env, "sendWorldError"), Napi::Function::New(env, sendWorldError));
	exports.Set(Napi::String::New(env, "setThreadCount"), Napi::Function::New(env, setThreadCount));
	exports.Set(Napi::String::New(env, "sendWorldData"), Napi::Function::New(env, sendWorldData));
	exports.Set(Napi::String::New(env, "sendSpawn"), Napi::Function::New(env, sendSpawn));
	exports.Set(Napi::String::New(env, "sendSpawn2"), Napi::Function::New(env, sendSpawn2));
	exports.Set(Napi::String::New(env, "sendChatMessage"), Napi::Function::New(env, _sendChatMessage));
	exports.Set(Napi::String::New(env, "onPeerConnect"), Napi::Function::New(env, onPeerConnect));
	return exports;
}

NODE_API_MODULE(ENetWrapper, Init)