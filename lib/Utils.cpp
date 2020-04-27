#include <enet/enet.h>
#include <iostream>
#include <cstdio>
#include <fstream>
#include "Utils.h"
#include "Packet.h"
#include "Gtps.h"

BYTE* itemsDat;

ENetPeer* Utils::peer;
ENetHost* Utils::server;

void Utils::buildItemsDatabase(std::string location)
{
	std::string secret = "PBG892FXX982ABC*";
	std::ifstream file(location, std::ios::binary | std::ios::ate);
	int size = file.tellg();
	char* data = new char[size];
	file.seekg(0, std::ios::beg);

	if (file.read((char*)(data), size))
	{
		itemsDat = new BYTE[60 + size];
		std::string asdf = "0400000010000000FFFFFFFF000000000800000000000000000000000000000000000000000000000000000000000000000000000000000000000000";

		for (int i = 0; i < asdf.length(); i += 2)
		{
			char x = Gtps::ch2n(asdf[i]);
			x = x << 4;
			x += Gtps::ch2n(asdf[i + 1]);
			memcpy(itemsDat + (i / 2), &x, 1);
			if (asdf.length() > 60 * 2) throw 0;
		}

		memcpy(itemsDat + 56, &size, 4);
		file.seekg(0, std::ios::beg);

		if (file.read((char*)(itemsDat + 60), size))
		{
			uint8_t* pData;
			int size = 0;
			const char filename[] = "items.dat";
			size = filesize(filename);
			pData = getA((std::string)filename, &size, false, false);
			itemdathash = HashString((unsigned char*)pData, size);
			file.close();
		}
	}
}

unsigned char* Utils::getA(std::string fileName, int* pSizeOut, bool bAddBasePath, bool bAutoDecompress)
{
	unsigned char* pData = NULL;
	FILE* fp = fopen(fileName.c_str(), "rb");
	if (!fp)
	{
		std::cout << "File not found" << std::endl;
		if (!fp) return NULL;
	}

	fseek(fp, 0, SEEK_END);
	*pSizeOut = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	pData = (unsigned char*)new unsigned char[((*pSizeOut) + 1)];
	if (!pData)
	{
		printf("Out of memory opening %s?", fileName.c_str());
		return 0;
	}
	pData[*pSizeOut] = 0;
	fread(pData, *pSizeOut, 1, fp);
	fclose(fp);

	return pData;
}

uint32_t Utils::HashString(unsigned char* str, int len)
{
	if (!str) return 0;

	unsigned char* n = (unsigned char*)str;
	uint32_t acc = 0x55555555;

	if (len == 0)
	{
		while (*n)
			acc = (acc >> 27) + (acc << 5) + *n++;
	}
	else
	{
		for (int i = 0; i < len; i++)
		{
			acc = (acc >> 27) + (acc << 5) + *n++;
		}
	}

	return acc;
}

std::ifstream::pos_type Utils::filesize(const char* filename)
{
	std::ifstream in(filename, std::ifstream::ate | std::ifstream::binary);
	return in.tellg();
}

Utils::Utils()
{}

Utils::Utils(ENetPeer* _peer, ENetHost* _server)
{
	peer = _peer;
	server = _server;
}

void Utils::_sendData(int num, char* data, int len)
{
	ENetPacket* packet = enet_packet_create(0,
		len + 5,
		ENET_PACKET_FLAG_RELIABLE);

	memcpy(packet->data, &num, 4);
	if (data != NULL)
	{
		memcpy(packet->data + 4, data, len);
	}
	char zero = 0;
	memcpy(packet->data + 4 + len, &zero, 1);
	enet_peer_send(getPeer(), 0, packet);
	enet_host_flush(getServer());
}

void Utils::setPeer(ENetPeer* _peer)
{
	peer = _peer;
}

void Utils::setServer(ENetHost* _server)
{
	server = _server;
}

ENetPeer* Utils::getPeer()
{
	return peer;
}

ENetHost* Utils::getServer()
{
	return server;
}

int Utils::GetMessageTypeFromPacket(ENetPacket* packet)
{
	int result;

	if (packet->dataLength > 3u)
	{
		result = *(packet->data);
	}
	else
	{
		std::cout << "Bad packet length, ignoring message" << std::endl;
		result = 0;
	}
	return result;
}

std::string Utils::GetTextPointerFromPacket(ENetPacket* packet)
{
	int zero = 0;
		
	memcpy(packet->data + packet->dataLength - 1, &zero, 1);
	return (char*)(packet->data + 4);
}