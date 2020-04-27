#include <iostream>
#include "napi.h"
#include <enet/enet.h>
#include <fstream>
#include <cstring>
#include <thread>
#include <future>
#include "lib/Utils.h"
#include "lib/Gtps.h"
#include "lib/Packet.h"
#include "lib/Methods.h"

ENetAddress address;

Utils utils;
Gtps gtps;

Napi::Object addressData;
Napi::Object threadObj;

Methods methods;

Napi::Value buildItemsDatabase(const Napi::CallbackInfo& args)
{
	Napi::Env env = args.Env();
	std::thread th(methods.buildItemsDatabase, args[0].As<Napi::String>().Utf8Value(), &utils);
	th.join();

	return Napi::Value();
}

Napi::Value sendWorldError(const Napi::CallbackInfo& args)
{	
	Napi::Env env = args.Env();
	std::thread th(methods.sendWorldError, &utils);
	th.join();

	return Napi::Value();
}

Napi::Value sendItemsData(const Napi::CallbackInfo& args)
{
	Napi::Env env = args.Env();
	std::thread th(methods.sendItemsData, &utils);
	th.join();

	return Napi::Value();
}

Napi::Value sendLoginPacket(const Napi::CallbackInfo& args)
{
	Napi::Env env = args.Env();
	std::thread th(methods.sendLoginPacket, &utils);
	th.join();

	return Napi::Value();
}

Napi::Value sendConsoleMessage(const Napi::CallbackInfo& args)
{
	Napi::Env env = args.Env();
	std::string message = args[0].As<Napi::String>().Utf8Value();

	std::thread th(methods.sendConsoleMessage, message, &utils);
	th.join();

	return Napi::Value();
}

Napi::Value sendDialogRequest(const Napi::CallbackInfo& args)
{
	Napi::Env env = args.Env();
	std::string message = args[0].As<Napi::String>().Utf8Value();
	std::thread th(methods.sendDialogRequest, message, &utils);
	th.join();

	return Napi::Value();
}

Napi::Value sendData(const Napi::CallbackInfo& args)
{
	Napi::Env env = args.Env();

	int num = args[0].As<Napi::Number>().Uint32Value();
	int _data = args[1].As<Napi::Number>().Uint32Value();
	int len = args[2].As<Napi::Number>().Uint32Value();
	char data[10] = "";

	std::thread th(methods.sendData, data, _data, num, len, &utils);
	th.join();

	return Napi::Value();
}

Napi::Number _enet_initialize(const Napi::CallbackInfo& args)
{
	Napi::Env env = args.Env();

	std::promise<int> promise;
	std::future<int> future = promise.get_future();
	std::thread th(methods._enet_initialize, &promise);
	th.join();

	return Napi::Number::New(env, future.get());
}

Napi::Value hostCreate(const Napi::CallbackInfo& args)
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
	std::thread th(methods.sendWorldRequest, &utils);
	th.join();

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
	return exports;
}

NODE_API_MODULE(ENetWrapper, Init)