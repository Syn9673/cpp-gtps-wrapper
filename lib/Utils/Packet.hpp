#ifndef PACKET_HPP
#define PACKET_HPP

class Packet
{
	public:
		Packet();
		static void sendRawPacket(ENetPeer* peer, ENetHost* server, std::string data);
		static void sendConsoleMessage(ENetPeer* peer, GamePacket createdPacket, std::string data);
		static GamePacket AppendString(GamePacket packet, std::string str);
		static GamePacket PacketEnd(GamePacket packet);
};

#endif