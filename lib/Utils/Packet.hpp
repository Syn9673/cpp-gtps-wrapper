#ifndef PACKET_HPP
#define PACKET_HPP

class Packet
{
	public:
		Packet();
		static void sendStringPacket(ENetPeer* peer, ENetHost* server, std::string data);

};

#endif