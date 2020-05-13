#include <iostream>
#include <enet/enet.h>
#include <map>
#include "napi.h"
#include "lib/Structs/GamePacket.hpp"
#include "lib/Utils/Packet.hpp"

Packet _packet;

using namespace std;
using namespace Napi;

// HANDLE FUNCTIONS TO EXPORT
#include "lib/HandleFunc.hpp"

Object Init(Env env, Object exports)
{	
	HandleFunc::reg(env, exports);

	return exports;
}

NODE_API_MODULE(test, Init)