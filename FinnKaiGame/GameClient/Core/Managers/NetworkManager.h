#pragma once

#include <mutex>
#include <queue>
#include <steam/steamnetworkingsockets.h>
#include <thread>
#include <EntityManager.h>

class NetworkManager
{
public:
	~NetworkManager() { 
	}

	static NetworkManager* instance() {
		if (_instance == nullptr)
			_instance = std::unique_ptr<NetworkManager>(new NetworkManager);
		return _instance.get();
	}

	void init();
	void update();
	std::queue<message> m_messageQ;

	void SendPlayerUpdate(message& m);
	void CloseConnection();

private:
	inline static std::unique_ptr<NetworkManager> _instance = nullptr;
	bool m_running = true;
	HSteamNetConnection m_hConnection;
	ISteamNetworkingSockets* m_pInterface;
	std::string ipAddress = "192.168.0.80";
	SteamNetworkingIPAddr serverAddr;
	std::queue< std::string > queueUserInput;
	std::mutex mutexUserInputQueue;
	std::unique_ptr<std::thread> s_pThreadUserInput = nullptr;

	void InitSteamDatagramConnectionSockets();
	void LocalUserInput_Init();

	void PollIncomingMessages();

	void PollLocalUserInput();

	void OnSteamNetConnectionStatusChanged(SteamNetConnectionStatusChangedCallback_t* pInfo);

	static void SteamNetConnectionStatusChangedCallback(SteamNetConnectionStatusChangedCallback_t* pInfo);

	void PollConnectionStateChanges();
	bool LocalUserInput_GetNext(std::string& result);
};