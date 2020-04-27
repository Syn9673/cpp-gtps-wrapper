#include <enet/enet.h>
#include <iostream>
#include <thread>
#include <future>
#include <cstring>
#include <fstream>
#include "napi.h"
#include "Utils.h"
#include "Methods.h"
#include "Packet.h"

Methods::Methods()
{}

void Methods::buildItemsDatabase(std::string location, Utils* utils)
{
	utils->buildItemsDatabase(location);
}

void Methods::sendWorldError(Utils* utils)
{
	Packets::sendWorldError(utils->getPeer());
}

void Methods::sendItemsData(Utils* utils)
{
	if (itemsDat != NULL)
	{
		ENetPacket * packet = enet_packet_create(itemsDat,
			itemsDatSize + 60,
			ENET_PACKET_FLAG_RELIABLE);
		enet_peer_send(utils->getPeer(), 0, packet);
	}
}

int Methods::_enet_initialize()
{
	return enet_initialize();
}

void Methods::sendWorldRequest(Utils* utils)
{
	Packets::sendWorldRequest(utils->getPeer());
}

void Methods::sendData(char* data, int _data, int num, int len, Utils* utils)
{
	sprintf(data, "%d", (long)_data);
	utils->_sendData(num, data, len);
}

void Methods::sendDialogRequest(std::string message, Utils* utils)
{
	Packets::sendDialogRequest(utils->getPeer(), message);
}

void Methods::sendConsoleMessage(std::string message, Utils* utils)
{
	Packets::sendConsoleMessage(utils->getPeer(), message);
}

void Methods::sendLoginPacket(Utils* utils)
{
	Packets::sendLoginPacket(utils->getPeer());
}

void Methods::hostCreate(Napi::Env env, Napi::Object argObject, ENetAddress address, Utils* utils)
{
	address.host = ENET_HOST_ANY;
	address.port = argObject.Get("port").As<Napi::Number>().Uint32Value();

	utils->setServer(enet_host_create(&address,
		argObject.Get("channels").As<Napi::Number>().Uint32Value(),
		argObject.Get("peers").As<Napi::Number>().Uint32Value(),
		argObject.Get("incomingBandwith").As<Napi::Number>().Uint32Value(),
		argObject.Get("outgoingBandwith").As<Napi::Number>().Uint32Value()));
}