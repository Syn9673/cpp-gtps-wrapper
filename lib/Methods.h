#ifndef METHODS_H
#define METHODS_H

class Methods
{
	public:
		Methods();

		static void buildItemsDatabase(std::string location, Utils* utils);
		static void sendWorldError(Utils* utils);
		static void sendItemsData(Utils* utils);
		static void _enet_initialize(std::promise<int>* promise);
		static void sendWorldRequest(Utils* utils);
		static void sendData(char* data, int _data, int num, int len, Utils* utils);
		static void sendDialogRequest(std::string message, Utils* utils);
		static void sendConsoleMessage(std::string message, Utils* utils);
		static void sendLoginPacket(Utils* utils);
};

#endif