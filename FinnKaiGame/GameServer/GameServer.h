#pragma once

#include <memory>
#include <vector>
#include <mutex>
#include <queue>
#include <steam/steamnetworkingsockets.h>

#include "EntityManager.h"
#include "entt/entt.hpp"

class GameServer
{
public:
    ~GameServer() { if (m_running) shutdown(); }

  static GameServer* instance() { 
    if (_instance == nullptr)
      _instance = std::unique_ptr<GameServer>(new GameServer);
    return _instance.get();
  }

  void run();
  void quit();
  void shutdown();

  EntityManager* entityManager() { return m_entityManager.get(); };

  bool isRunning();

  static void SteamNetConnectionStatusChangedCallback(SteamNetConnectionStatusChangedCallback_t* pInfo);
private:
  static std::unique_ptr<GameServer> _instance;
  uint16 DEFAULT_SERVER_PORT = 27020U;
  HSteamListenSocket m_hListenSock;
  HSteamNetPollGroup m_hPollGroup;
  ISteamNetworkingSockets* m_pInterface;
  std::map< HSteamNetConnection, std::string> m_mapClients;
  std::thread* s_pThreadUserInput = nullptr;
  std::queue< std::string > queueUserInput;
  std::mutex mutexUserInputQueue;

  GameServer() : m_running(true) { init(); }
  
  std::unique_ptr<EntityManager> m_entityManager;

  bool m_running;
  void init();
  void update();
  void InitSteamDatagramConnectionSockets();
  void OnSteamNetConnectionStatusChanged(SteamNetConnectionStatusChangedCallback_t* pInfo);

  void initNewConnection(SteamNetConnectionStatusChangedCallback_t* pInfo, char  temp[1024]);

  void PollConnectionStateChanges();

  // TEST FUNCTIONS

  void LocalUserInput_Init();
  bool LocalUserInput_GetNext(std::string& result);
  void SendStringToClient(HSteamNetConnection conn, const char* str);
  void SendStringToAllClients(const char* str, HSteamNetConnection except = k_HSteamNetConnection_Invalid);
  void PollIncomingMessages();
  void PollLocalUserInput();
  void SetClientNick(HSteamNetConnection hConn, const char* nick);

};