#ifndef PACKETS_HPP
#define PACKETS_HPP

namespace Packets
{
	void sendRawPacket(const Napi::CallbackInfo& info);
	void log(const Napi::CallbackInfo& info);
	void sendQuit(const Napi::CallbackInfo& info);
}

#endif