#ifndef PACKET_HPP
#define PACKET_HPP

class Packet
{
	public:
		Packet();
		static void sendStringPacket(ENetPeer* peer, ENetHost* server, std::string data);
		static void sendRawPacket(ENetPeer* peer, GamePacket p);
		static GamePacket PacketEnd(GamePacket packet);
		static void sendPacket(ENetPeer* peer, GamePacket _packet);

};

#endif