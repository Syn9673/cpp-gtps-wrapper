#include <iostream>
#include <vector>
#include <enet/enet.h>
#include "Gtps.h"
#include "others/DroppedItem.h"
#include "others/WorldItem.h"
#include "others/WorldInfo.h"
#include "others/PlayerMoving.h"
#include "Packet.h"

int itemsDatSize, itemdathash;

namespace Packets
{
	void sendConsoleMessage(ENetPeer* peer, std::string message)
	{
		GamePacket p = Gtps::packetEnd(Gtps::appendString(Gtps::Gtps::appendString(Gtps::createPacket(), "OnConsoleMessage"), message));
		ENetPacket* packet = enet_packet_create(p.data,
			p.len,
			ENET_PACKET_FLAG_RELIABLE);
		enet_peer_send(peer, 0, packet);
		delete p.data;
	}

	void sendDialogRequest(ENetPeer* peer, std::string message)
	{
		GamePacket p = Gtps::packetEnd(Gtps::appendString(Gtps::appendString(Gtps::createPacket(), "OnDialogRequest"), message));
		ENetPacket* packet = enet_packet_create(p.data,
			p.len,
			ENET_PACKET_FLAG_RELIABLE);
		enet_peer_send(peer, 0, packet);
		delete p.data;
	}

	void sendSpawn(ENetPeer* peer, std::string message)
	{
		GamePacket p = Gtps::packetEnd(Gtps::appendString(Gtps::appendString(Gtps::createPacket(), "OnSpawn"), message));
		ENetPacket* packet = enet_packet_create(p.data,
			p.len,
			ENET_PACKET_FLAG_RELIABLE);
		enet_peer_send(peer, 0, packet);
		delete p.data;
	}

	void sendWorldError(ENetPeer* peer)
	{
		GamePacket p2 = Gtps::packetEnd(Gtps::appendIntx(Gtps::appendString(Gtps::createPacket(), "OnFailedToEnterWorld"), 1));
		ENetPacket* packet2 = enet_packet_create(p2.data,
			p2.len,
			ENET_PACKET_FLAG_RELIABLE);
		enet_peer_send(peer, 0, packet2);
		delete p2.data;
	}

	void sendLoginPacket(ENetPeer* peer)
	{
		GamePacket p = Gtps::packetEnd(
			Gtps::appendString(
				Gtps::appendString(
					Gtps::appendString(
						Gtps::appendString(
							Gtps::appendInt(
								Gtps::appendString(
									Gtps::createPacket(),
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
		GamePacket p = Gtps::packetEnd(
			Gtps::appendString(
				Gtps::appendString(
					Gtps::createPacket(),
					"OnRequestWorldSelectMenu"),
				"default|NODEJS\nadd_button|Showing: `wWorlds``|_catselect_|0.6|3529161471|\nadd_floater|NODEJS|0|0.55|3529161471\n"));
	
		ENetPacket* packet = enet_packet_create(p.data,
			p.len,
			ENET_PACKET_FLAG_RELIABLE);
		enet_peer_send(peer, 0, packet);
		delete p.data;
	}

	void SendPacketRaw(int a1, void *packetData, size_t packetDataSize, void *a4, ENetPeer* peer, int packetFlag)
	{
		ENetPacket *p;

		if (peer) // check if we have it setup
		{
			if (a1 == 4 && *((BYTE *)packetData + 12) & 8)
			{
				p = enet_packet_create(0, packetDataSize + *((DWORD *)packetData + 13) + 5, packetFlag);
				int four = 4;
				memcpy(p->data, &four, 4);
				memcpy((char *)p->data + 4, packetData, packetDataSize);
				memcpy((char *)p->data + packetDataSize + 4, a4, *((DWORD *)packetData + 13));
				enet_peer_send(peer, 0, p);
			}
			else
			{
				p = enet_packet_create(0, packetDataSize + 5, packetFlag);
				memcpy(p->data, &a1, 4);
				memcpy((char *)p->data + 4, packetData, packetDataSize);
				enet_peer_send(peer, 0, p);
			}
		}
		delete (char*)packetData;
	}

	BYTE* packPlayerMoving(PlayerMoving* dataStruct)
	{
		BYTE* data = new BYTE[56];

		for (int i = 0; i < 56; i++)
		{
			data[i] = 0;
		}

		memcpy(data, &dataStruct->packetType, 4);
		memcpy(data + 4, &dataStruct->netID, 4);
		memcpy(data + 12, &dataStruct->characterState, 4);
		memcpy(data + 20, &dataStruct->plantingTree, 4);
		memcpy(data + 24, &dataStruct->x, 4);
		memcpy(data + 28, &dataStruct->y, 4);
		memcpy(data + 32, &dataStruct->XSpeed, 4);
		memcpy(data + 36, &dataStruct->YSpeed, 4);
		memcpy(data + 44, &dataStruct->punchX, 4);
		memcpy(data + 48, &dataStruct->punchY, 4);
		return data;
	}

	PlayerMoving* unpackPlayerMoving(BYTE* data)
	{
		PlayerMoving* dataStruct = new PlayerMoving;
		memcpy(&dataStruct->packetType, data, 4);
		memcpy(&dataStruct->netID, data + 4, 4);
		memcpy(&dataStruct->characterState, data + 12, 4);
		memcpy(&dataStruct->plantingTree, data + 20, 4);
		memcpy(&dataStruct->x, data + 24, 4);
		memcpy(&dataStruct->y, data + 28, 4);
		memcpy(&dataStruct->XSpeed, data + 32, 4);
		memcpy(&dataStruct->YSpeed, data + 36, 4);
		memcpy(&dataStruct->punchX, data + 44, 4);
		memcpy(&dataStruct->punchY, data + 48, 4);
		return dataStruct;
	}

	/*void sendDrop(WorldInfo* worldDB, ENetPeer* peer, int netID, int x, int y, int item, int count, BYTE specialEffect, bool onlyForPeer)
	{
		if (item >= 7068) return;
		if (item < 0) return;
		if (onlyForPeer) {
			PlayerMoving data;
			data.packetType = 14;
			data.x = x;
			data.y = y;
			data.netID = netID;
			data.plantingTree = item;
			float val = count; // item count
			BYTE val2 = specialEffect;

			BYTE* raw = packPlayerMoving(&data);
			memcpy(raw + 16, &val, 4);
			memcpy(raw + 1, &val2, 1);

			SendPacketRaw(4, raw, 56, 0, peer, ENET_PACKET_FLAG_RELIABLE);
		}
		else {
			DroppedItem dropItem;
			dropItem.x = x;
			dropItem.y = y;
			dropItem.count = count;
			dropItem.id = item;
			dropItem.uid = worldDB.get2(((PlayerInfo *)(peer->data))->currentWorld).ptr->currentItemUID++;
			worldDB.get2(((PlayerInfo *)(peer->data))->currentWorld).ptr->droppedItems.push_back(dropItem);
			ENetPeer * currentPeer;
			for (currentPeer = server->peers;
				currentPeer < &server->peers[server->peerCount];
				++currentPeer)
			{
				if (currentPeer->state != ENET_PEER_STATE_CONNECTED)
					continue;
				if (isHere(peer, currentPeer)) {

					ItemSharedUID m_uid;
					m_uid.actual_uid = dropItem.uid;
					m_uid.shared_uid = (((PlayerInfo*)(currentPeer->data)))->last_uid++;
					(((PlayerInfo*)(currentPeer->data)))->item_uids.push_back(m_uid);
					PlayerMoving data;
					data.packetType = 14;
					data.x = x;
					data.y = y;
					data.netID = netID;
					data.plantingTree = item;
					float val = count; // item count
					BYTE val2 = specialEffect;

					BYTE* raw = packPlayerMoving(&data);
					memcpy(raw + 16, &val, 4);
					memcpy(raw + 1, &val2, 1);

					SendPacketRaw(4, raw, 56, 0, currentPeer, ENET_PACKET_FLAG_RELIABLE);
				}
			}
		}
	}*/
};