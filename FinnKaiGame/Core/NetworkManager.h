#pragma once

#include <mutex>
#include <queue>
#include <steam/steamnetworkingsockets.h>

class NetworkManager
{
public:
	void update(bool running);

private:

	bool m_running = false;
	HSteamNetConnection m_hConnection;
	ISteamNetworkingSockets* m_pInterface;
	std::string ipAddress = "192.168.0.202";
	SteamNetworkingIPAddr serverAddr;
	std::queue< std::string > queueUserInput;
	std::mutex mutexUserInputQueue;
	std::unique_ptr<std::thread> s_pThreadUserInput = nullptr;

	void init();

	void InitSteamDatagramConnectionSockets();
	void LocalUserInput_Init();

	void PollIncomingMessages();

	void PollLocalUserInput();

	void OnSteamNetConnectionStatusChanged(SteamNetConnectionStatusChangedCallback_t* pInfo);

	static void SteamNetConnectionStatusChangedCallback(SteamNetConnectionStatusChangedCallback_t* pInfo);

	void PollConnectionStateChanges();
	bool LocalUserInput_GetNext(std::string& result);
};