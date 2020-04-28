#ifndef METHODS_H
#define METHODS_H

class Methods
{
	public:
		Methods();

		static void buildItemsDatabase(std::string location, Utils* utils);
		static void sendWorldError(std::string id, Utils* utils);
		static void sendItemsData(std::string id, Utils* utils);
		static int _enet_initialize();
		static void sendWorldRequest(std::string id, Utils* utils);
		static void sendData(std::string id, char* data, int _data, int num, int len, Utils* utils);
		static void sendDialogRequest(std::string id, std::string message, Utils* utils);
		static void sendConsoleMessage(std::string id, std::string message, Utils* utils);
		static void sendLoginPacket(std::string id, Utils* utils);
		static void hostCreate(Napi::Env env, Napi::Object argObject, ENetAddress address, Utils* utils);
		static WorldInfo generateWorld(std::string name, int width, int height);	
		static void sendWorld(ENetPeer* peer, WorldInfo* worldInfo);
		static bool isHere(ENetPeer* peer, ENetPeer* peer2);
		static void sendChatMessage(ENetHost* server, ENetPeer* peer, int netID, std::string message);
		static void onPeerConnect(ENetPeer* peer, ENetHost* server);
};

#endif 