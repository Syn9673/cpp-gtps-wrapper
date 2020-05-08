#ifndef PACKET_HPP
#define PACKET_HPP

class Packet
{
	public:
		Packet();
		static void sendRawPacket(ENetPeer* peer, ENetHost* server, std::string data);
		static GamePacket PacketEnd(GamePacket packet);
		static void sendPacket(ENetPeer* peer, GamePacket _packet);

};

#endif