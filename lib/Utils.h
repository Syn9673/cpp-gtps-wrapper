#ifndef UTILS_H
#define UTILS_H

typedef unsigned short enet_uint16;
typedef unsigned int enet_uint32;
typedef unsigned char enet_uint8;
typedef unsigned char BYTE;

extern BYTE* itemsDat;

class Utils
{
	private:
		static std::map<std::string, ENetPeer*> peers;
		static ENetHost* server;

	public:
		Utils();
		Utils(ENetPeer* _peer, ENetHost* _server);

		unsigned char* getA(std::string fileName, int* pSizeOut, bool bAddBasePath, bool bAutoDecompress);
		std::ifstream::pos_type filesize(const char* filename);
		uint32_t HashString(unsigned char* str, int len);
		void setServer(ENetHost* server);
		void _sendData(std::string id, int num, char* data, int len);
		ENetPeer* getPeer(std::string id);
		std::map<std::string, ENetPeer*>* getPeers();
		ENetHost* getServer();
		int GetMessageTypeFromPacket(ENetPacket* packet);
		std::string GetTextPointerFromPacket(ENetPacket* packet);
		void buildItemsDatabase(std::string location);
};

#endif