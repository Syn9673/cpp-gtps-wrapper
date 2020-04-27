#include <iostream>
#include <string>
#include "napi.h"
#include <enet/enet.h>
#include <fstream>
#include <cstdio>
#include "lib/Utils.h"
#include "lib/Gtps.h"

ENetAddress address;

Utils utils = Utils();
Gtps gtps = Gtps();

BYTE* itemsDat;
int itemsDatSize, itemdathash;

Napi::Object addressData;

namespace Packets
{
	void sendConsoleMessage(ENetPeer* peer, std::string message)
	{
		GamePacket p = gtps.packetEnd(gtps.appendString(gtps.appendString(gtps.createPacket(), "OnConsoleMessage"), message));
		ENetPacket* packet = enet_packet_create(p.data,
			p.len,
			ENET_PACKET_FLAG_RELIABLE);
		enet_peer_send(peer, 0, packet);
		delete p.data;
	}

	void sendDialogRequest(ENetPeer* peer, std::string message)
	{
		GamePacket p = gtps.packetEnd(gtps.appendString(gtps.appendString(gtps.createPacket(), "OnDialogRequest"), message));
		ENetPacket* packet = enet_packet_create(p.data,
			p.len,
			ENET_PACKET_FLAG_RELIABLE);
		enet_peer_send(peer, 0, packet);
		delete p.data;
	}

	void sendSpawn(ENetPeer* peer, std::string message) {
		GamePacket p = gtps.packetEnd(gtps.appendString(gtps.appendString(gtps.createPacket(), "OnSpawn"), message));
		ENetPacket* packet = enet_packet_create(p.data,
			p.len,
			ENET_PACKET_FLAG_RELIABLE);
		enet_peer_send(peer, 0, packet);
		delete p.data;
	}

	void sendWorldError(ENetPeer* peer)
	{
		GamePacket p2 = gtps.packetEnd(gtps.appendIntx(gtps.appendString(gtps.createPacket(), "OnFailedToEnterWorld"), 1));
		ENetPacket* packet2 = enet_packet_create(p2.data,
			p2.len,
			ENET_PACKET_FLAG_RELIABLE);
		enet_peer_send(peer, 0, packet2);
		delete p2.data;
	}

	void sendLoginPacket(ENetPeer* peer)
	{
		GamePacket p = gtps.packetEnd(
			gtps.appendString(
				gtps.appendString(
					gtps.appendString(
						gtps.appendString(
							gtps.appendInt(
								gtps.appendString(
									gtps.createPacket(),
									"OnSuperMainStartAcceptLogonHrdxs47254722215a"),
								itemdathash),
							"ubistatic-a.akamaihd.net"),
						"0098/CDNContent59/cache/"),
					"cc.cz.madkite.freedom org.aqua.gg idv.aqua.bulldog com.cih.gamecih2 com.cih.gamecih com.cih.game_cih cn.maocai.gamekiller com.gmd.speedtime org.dax.attack com.x0.strai.frep com.x0.strai.free org.cheatengine.cegui org.sbtools.gamehack com.skgames.traffikrider org.sbtoods.gamehaca com.skype.ralder org.cheatengine.cegui.xx.multi1458919170111 com.prohiro.macro me.autotouch.autotouch com.cygery.repetitouch.free com.cygery.repetitouch.pro com.proziro.zacro com.slash.gamebuster"),
				"proto=84|choosemusic=audio/mp3/about_theme.mp3|active_holiday=0|server_tick=226933875|clash_active=0|drop_lavacheck_faster=1|isPayingUser=0|"));


		ENetPacket* packet = enet_packet_create(p.data,
			p.len,
			ENET_PACKET_FLAG_RELIABLE);
		enet_peer_send(peer, 0, packet);
		delete p.data;
	}

	void sendWorldRequest(ENetPeer* peer)
	{
		GamePacket p = gtps.packetEnd(
			gtps.appendString(
				gtps.appendString(
					gtps.createPacket(),
					"OnRequestWorldSelectMenu"),
				"default|NODEJS\nadd_button|Showing: `wWorlds``|_catselect_|0.6|3529161471|\nadd_floater|NODEJS|0|0.55|3529161471\n"));
	
		ENetPacket* packet = enet_packet_create(p.data,
			p.len,
			ENET_PACKET_FLAG_RELIABLE);
		enet_peer_send(peer, 0, packet);
		delete p.data;
	}
}

Napi::Value buildItemsDatabase(const Napi::CallbackInfo& args)
{
	Napi::Env env = args.Env();

	std::string secret = "PBG892FXX982ABC*";
	std::ifstream file(args[0].As<Napi::String>().Utf8Value(), std::ios::binary | std::ios::ate);
	int size = file.tellg();
	char* data = new char[size];
	file.seekg(0, std::ios::beg);

	if (file.read((char*)(data), size))
	{
		itemsDat = new BYTE[60 + size];
		std::string asdf = "0400000010000000FFFFFFFF000000000800000000000000000000000000000000000000000000000000000000000000000000000000000000000000";

		for (int i = 0; i < asdf.length(); i += 2)
		{
			char x = gtps.ch2n(asdf[i]);
			x = x << 4;
			x += gtps.ch2n(asdf[i + 1]);
			memcpy(itemsDat + (i / 2), &x, 1);
			if (asdf.length() > 60 * 2) throw 0;
		}

		memcpy(itemsDat + 56, &size, 4);
		file.seekg(0, std::ios::beg);

		if (file.read((char*)(itemsDat + 60), size))
		{
			uint8_t* pData;
			int size = 0;
			const char filename[] = "items.dat";
			size = utils.filesize(filename);
			pData = utils.getA((std::string)filename, &size, false, false);
			itemdathash = utils.HashString((unsigned char*)pData, size);
			file.close();
		}
	}

	return Napi::Value();
}

Napi::Value sendWorldError(const Napi::CallbackInfo& args)
{	
	Napi::Env env = args.Env();
	Packets::sendWorldError(utils.getPeer());

	return Napi::Value();
}

Napi::Value sendItemsData(const Napi::CallbackInfo& args)
{
	Napi::Env env = args.Env();

	if (itemsDat != NULL)
	{
		ENetPacket * packet = enet_packet_create(itemsDat,
			itemsDatSize + 60,
			ENET_PACKET_FLAG_RELIABLE);
		enet_peer_send(utils.getPeer(), 0, packet);
	}


	return Napi::Value();
}

Napi::Value sendLoginPacket(const Napi::CallbackInfo& args)
{
	Napi::Env env = args.Env();
	Packets::sendLoginPacket(utils.getPeer());

	return Napi::Value();
}

Napi::Value sendConsoleMessage(const Napi::CallbackInfo& args)
{
	Napi::Env env = args.Env();
	std::string message = args[0].As<Napi::String>().Utf8Value();

	Packets::sendConsoleMessage(utils.getPeer(), message);

	return Napi::Value();
}

Napi::Value sendDialogRequest(const Napi::CallbackInfo& args)
{
	Napi::Env env = args.Env();
	std::string message = args[0].As<Napi::String>().Utf8Value();

	Packets::sendDialogRequest(utils.getPeer(), message);

	return Napi::Value();
}

Napi::Value sendData(const Napi::CallbackInfo& args)
{
	Napi::Env env = args.Env();

	int num = args[0].As<Napi::Number>().Uint32Value();
	uint32_t _data = args[1].As<Napi::Number>().Uint32Value();
	int len = args[2].As<Napi::Number>().Uint32Value();
	char data[10] = "";

	sprintf(data, "%d", (long)_data);

	utils._sendData(num, data, len);

	return Napi::Value();
}

Napi::Number _enet_initialize(const Napi::CallbackInfo& args)
{
	Napi::Env env = args.Env();

	return Napi::Number::New(env, enet_initialize());
}

Napi::Boolean hostCreate(const Napi::CallbackInfo& args)
{
	Napi::Env env = args.Env();
	
	if (!args[0].IsObject())
	{
		throw Napi::TypeError::New(env, "Please pass an object for the arguments.");
	}

	Napi::Object argObject = args[0].As<Napi::Object>().ToObject();
	address.host = ENET_HOST_ANY;
	address.port = argObject.Get("port").As<Napi::Number>().Uint32Value();

	utils.setServer(enet_host_create(&address,
		argObject.Get("channels").As<Napi::Number>().Uint32Value(),
		argObject.Get("peers").As<Napi::Number>().Uint32Value(),
		argObject.Get("incomingBandwith").As<Napi::Number>().Uint32Value(),
		argObject.Get("outgoingBandwith").As<Napi::Number>().Uint32Value()));

	return Napi::Boolean::New(env, true);
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
					utils.setPeer(event.peer);
					emit.Call({
						Napi::String::New(env, "connect")
					});

					break;

				case ENET_EVENT_TYPE_RECEIVE:
					utils.setPeer(event.peer);
					Napi::Object receivedData = Napi::Object::New(env);
					receivedData.Set("data", Napi::String::New(env, utils.GetTextPointerFromPacket(event.packet)));
					receivedData.Set("type", Napi::Number::New(env, utils.GetMessageTypeFromPacket(event.packet)));
					receivedData.Set("connectID", Napi::Number::New(env, utils.getPeer()->connectID));

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
	Packets::sendWorldRequest(utils.getPeer());

	return Napi::Value();
}

Napi::Value sendSpawn(const Napi::CallbackInfo& args)
{
	Napi::Env env = args.Env();
	Packets::sendSpawn(utils.getPeer(), args[0].As<Napi::String>().Utf8Value());

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
	exports.Set(Napi::String::New(env, "sendSpawn"), Napi::Function::New(env, sendSpawn));
	return exports;
}

NODE_API_MODULE(ENetWrapper, Init)