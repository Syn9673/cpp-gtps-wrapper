#include <iostream>
#include <enet/enet.h>
#include "Gtps.h"
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
};