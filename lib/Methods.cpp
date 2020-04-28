#include <enet/enet.h>
#include <iostream>
#include <vector>
#include <cstring>
#include <fstream>
#include "napi.h"
#include <map>
#include "Utils.h"
#include "Gtps.h"
#include "others/DroppedItem.h"
#include "others/WorldItem.h"
#include "others/WorldInfo.h"
#include "others/InventoryItem.h"
#include "others/PlayerInventory.h"
#include "others/ItemSharedUID.h"
#include "others/PlayerInfo.h"
#include "others/PlayerMoving.h"
#include "others/DroppedItem.h"
#include "Packet.h"
#include "Methods.h"

Methods::Methods()
{}

void Methods::buildItemsDatabase(std::string location, Utils* utils)
{
	utils->buildItemsDatabase(location);
}

void Methods::sendWorldError(std::string id, Utils* utils)
{
	Packets::sendWorldError(utils->getPeer(id));
}

void Methods::sendItemsData(std::string id, Utils* utils)
{
	if (itemsDat != NULL)
	{
		ENetPacket * packet = enet_packet_create(itemsDat,
			itemsDatSize + 60,
			ENET_PACKET_FLAG_RELIABLE);
		enet_peer_send(utils->getPeer(id), 0, packet);
	}
}

int Methods::_enet_initialize()
{
	return enet_initialize();
}

void Methods::sendWorldRequest(std::string id, Utils* utils)
{
	Packets::sendWorldRequest(utils->getPeer(id));
}

void Methods::sendData(std::string id, char* data, int _data, int num, int len, Utils* utils)
{
	std::cout << "Methods: " << id << std::endl;
	sprintf(data, "%d", _data);
	utils->_sendData(id, num, data, len);
}

void Methods::sendDialogRequest(std::string id, std::string message, Utils* utils)
{
	Packets::sendDialogRequest(utils->getPeer(id), message);
}

void Methods::sendConsoleMessage(std::string id, std::string message, Utils* utils)
{
	Packets::sendConsoleMessage(utils->getPeer(id), message);
}

void Methods::sendLoginPacket(std::string id, Utils* utils)
{
	Packets::sendLoginPacket(utils->getPeer(id));
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

WorldInfo Methods::generateWorld(std::string name, int width, int height)
{
	WorldInfo world;
	world.name = name;
	world.width = width;
	world.height = height;
	world.items = new WorldItem[world.width * world.height];

	for (int i = 0; i < world.width*world.height; i++)
	{
		if (i >= 3800 && i < 5400 && !(rand() % 50)){ world.items[i].foreground = 10; }
		else if (i >= 3700 && i < 5400) {
			if(i > 5000) {
				if (i % 7 == 0) { world.items[i].foreground = 4;}
				else { world.items[i].foreground = 2; }
			}
			else { world.items[i].foreground = 2; }
		}
		else if (i >= 5400) { world.items[i].foreground = 8; }
		if (i >= 3700)
			world.items[i].background = 14;
		if (i == 3650)
			world.items[i].foreground = 6;
		else if (i >= 3600 && i<3700)
			world.items[i].foreground = 0; //fixed the grass in the world!
		if (i == 3750)
			world.items[i].foreground = 8;
	}

	return world;
}

void Methods::sendWorld(ENetPeer* peer, WorldInfo* worldInfo)
{
	((PlayerInfo*)(peer->data))->joinClothesUpdated = false;
	std::string worldName = worldInfo->name;

	int xSize = worldInfo->width;
	int ySize = worldInfo->height;
	int square = xSize*ySize; 
	__int16 namelen = worldName.length();

	int alloc = (8 * square);
	int total = 78 + namelen + square + 24 + alloc     ;
		
	BYTE* data = new BYTE[total];
	int s1 = 4,s3 = 8,zero = 0;  
		 
	memset(data, 0, total);

	memcpy(data, &s1, 1);
	memcpy(data + 4, &s1, 1);
	memcpy(data + 16, &s3, 1);  
	memcpy(data + 66, &namelen, 1);
	memcpy(data + 68, worldName.c_str(), namelen);
	memcpy(data + 68 + namelen, &xSize, 1);
	memcpy(data + 72 + namelen, &ySize, 1);
	memcpy(data + 76 + namelen, &square, 2);
	BYTE* blc = data + 80 + namelen;

	for (int i = 0; i < square; i++) {
		//removed cus some of blocks require tile extra and it will crash the world without
		memcpy(blc, &zero, 2);
			
		memcpy(blc + 2, &worldInfo->items[i].background, 2);
		int type = 0x00000000;
		// type 1 = locked
		if (worldInfo->items[i].water)
			type |= 0x04000000;
		if (worldInfo->items[i].glue)
			type |= 0x08000000;
		if (worldInfo->items[i].fire)
			type |= 0x10000000;
		if (worldInfo->items[i].red)
			type |= 0x20000000;
		if (worldInfo->items[i].green)
			type |= 0x40000000;
		if (worldInfo->items[i].blue)
			type |= 0x80000000;

		// int type = 0x04000000; = water
		// int type = 0x08000000 = glue
		// int type = 0x10000000; = fire
		// int type = 0x20000000; = red color
		// int type = 0x40000000; = green color
		// int type = 0x80000000; = blue color
		memcpy(blc + 4, &type, 4);
		blc += 8;
	}

	ENetPacket* packetw = enet_packet_create(data, total, ENET_PACKET_FLAG_RELIABLE);
	enet_peer_send(peer, 0, packetw);

	for (int i = 0; i < square; i++) {
		PlayerMoving data;
		//data.packetType = 0x14;
		data.packetType = 0x3;

		//data.characterState = 0x924; // animation
		data.characterState = 0x0; // animation
		data.x = i%worldInfo->width;
		data.y = i/worldInfo->height;
		data.punchX = i%worldInfo->width;
		data.punchY = i / worldInfo->width;
		data.XSpeed = 0;
		data.YSpeed = 0;
		data.netID = -1;
		data.plantingTree = worldInfo->items[i].foreground;
		Packets::SendPacketRaw(4, Packets::packPlayerMoving(&data), 56, 0, peer, ENET_PACKET_FLAG_RELIABLE);
	}

	((PlayerInfo*)(peer->data))->currentWorld = worldInfo->name;
	delete[] data;
	((PlayerInfo*)(peer->data))->item_uids.clear();
	((PlayerInfo*)(peer->data))->last_uid = 1;

	/*for (int i = 0; i < worldInfo->droppedItems.size(); i++) {
		DroppedItem item = worldInfo->droppedItems[i];
		Packets::sendDrop(&worldDb, peer, -1, item.x, item.y, item.id, item.count, 0, true);
		ItemSharedUID m_uid;
		m_uid.actual_uid = item.uid;
		m_uid.shared_uid = (((PlayerInfo*)(peer->data)))->last_uid++;
		(((PlayerInfo*)(peer->data)))->item_uids.push_back(m_uid);
	}*/
}

bool Methods::isHere(ENetPeer* peer, ENetPeer* peer2)
{
	return ((PlayerInfo*)(peer->data))->currentWorld == ((PlayerInfo*)(peer2->data))->currentWorld;
}

void Methods::sendChatMessage(ENetHost* server, ENetPeer* peer, int netID, std::string message)
{
	if (message.length() == 0) return;

	ENetPeer* currentPeer;
	std::string name = "";

	for (currentPeer = server->peers;
		currentPeer < &server->peers[server->peerCount];
		++currentPeer)
	{
		if (currentPeer->state != ENET_PEER_STATE_CONNECTED)
			continue;
		if (((PlayerInfo*)(currentPeer->data))->netID == netID)
			name = ((PlayerInfo*)(currentPeer->data))->displayName;
	}

	GamePacket p = Gtps::packetEnd(
		 Gtps::appendString(
			 Gtps::appendString(
				 Gtps::createPacket(), 
				"OnConsoleMessage"), 
			"CP:0_PL:4_OID:_CT:[W]_ `o<`w" + name + "`o> " + message));

	GamePacket p2 = Gtps::packetEnd(
		 Gtps::appendIntx(
			 Gtps::appendString(
				 Gtps::appendIntx(
					 Gtps::appendString(
						 Gtps::createPacket(), 
						"OnTalkBubble"), 
					netID),
				message), 
			0));

	for (currentPeer = server->peers;
		currentPeer < &server->peers[server->peerCount];
		++currentPeer)
	{
		if (currentPeer->state != ENET_PEER_STATE_CONNECTED)
			continue;

		if (isHere(peer, currentPeer))
		{
				
			ENetPacket * packet = enet_packet_create(p.data,
				p.len,
				ENET_PACKET_FLAG_RELIABLE);

			enet_peer_send(currentPeer, 0, packet);
				
			//enet_host_flush(server);
				
			ENetPacket * packet2 = enet_packet_create(p2.data,
				p2.len,
				ENET_PACKET_FLAG_RELIABLE);
			enet_peer_send(currentPeer, 0, packet2);
				
			//enet_host_flush(server);
		}
	}
	delete p.data;
	delete p2.data;
}

void Methods::onPeerConnect(ENetPeer* peer, ENetHost* server)
{
	ENetPeer* currentPeer;

	for (currentPeer = server->peers;
		currentPeer < &server->peers[server->peerCount];
		++currentPeer)
	{
		if (currentPeer->state != ENET_PEER_STATE_CONNECTED)
			continue;
		if (peer != currentPeer)
		{
			if (isHere(peer, currentPeer))
			{
				std::string netIdS = std::to_string(((PlayerInfo*)(currentPeer->data))->netID);
				Packets::sendSpawn(peer, "spawn|avatar\nnetID|" + netIdS + "\nuserID|" + netIdS + "\ncolrect|0|0|20|30\nposXY|" + std::to_string(((PlayerInfo*)(currentPeer->data))->x) + "|" + std::to_string(((PlayerInfo*)(currentPeer->data))->y) + "\nname|``" + ((PlayerInfo*)(currentPeer->data))->displayName + "``\ncountry|" + ((PlayerInfo*)(currentPeer->data))->country + "\ninvis|0\nmstate|0\nsmstate|0\n"); // ((PlayerInfo*)(server->peers[i].data))->tankIDName
				std::string netIdS2 = std::to_string(((PlayerInfo*)(peer->data))->netID);
				Packets::sendSpawn(currentPeer, "spawn|avatar\nnetID|" + netIdS2 + "\nuserID|" + netIdS2 + "\ncolrect|0|0|20|30\nposXY|" + std::to_string(((PlayerInfo*)(peer->data))->x) + "|" + std::to_string(((PlayerInfo*)(peer->data))->y) + "\nname|``" + ((PlayerInfo*)(peer->data))->displayName + "``\ncountry|" + ((PlayerInfo*)(peer->data))->country + "\ninvis|0\nmstate|0\nsmstate|0\n"); // ((PlayerInfo*)(server->peers[i].data))->tankIDName
			}
		}
	}	
}