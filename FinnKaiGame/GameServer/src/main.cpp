#pragma once

#include "GameServer.h"
#include "Utils.h"

#include <iostream>
#include <filesystem>

int main(int argc, char* argv[])
{
	SteamNetworkingIPAddr addrServer; addrServer.Clear();
	if (addrServer.IsIPv6AllZeros() && argc == 2)
	{
		if (!addrServer.ParseString(argv[1]))
			Utils::FatalError("Invalid server address '%s'", argv[1]);
		if (addrServer.m_port == 0)
			addrServer.m_port = 27020U;
	}
	if (addrServer.IsIPv6AllZeros())
	{
		std::cout << "Invalid IP address" << std::endl;
		return 0;
	}



	GameServer::instance()->run();
	return 0;
}
