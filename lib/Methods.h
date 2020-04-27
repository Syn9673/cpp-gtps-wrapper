#ifndef METHODS_H
#define METHODS_H

class Methods
{
	public:
		Methods();

		static void buildItemsDatabase(std::string location, Utils* utils);
		static void sendWorldError(Utils* utils);
		static void sendItemsData(Utils* utils);
		static int _enet_initialize();
		static void sendWorldRequest(Utils* utils);
		static void sendData(char* data, int _data, int num, int len, Utils* utils);
		static void sendDialogRequest(std::string message, Utils* utils);
		static void sendConsoleMessage(std::string message, Utils* utils);
		static void sendLoginPacket(Utils* utils);
		static void hostCreate(Napi::Env env, Napi::Object argObject, ENetAddress address, Utils* utils);

};

#endif