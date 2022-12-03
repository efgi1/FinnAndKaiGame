#pragma once

#include "GameServer.h"
#include "Utils.h"
#include "TransformComponent.h"

void GameServer::run()
{
	m_running = true;
	while (m_running)
	{
		update();
	}

	shutdown();
}

void GameServer::quit()
{
	m_running = false;
}

void GameServer::shutdown()
{
	quit();
	// Close all the connections
	Utils::Printf("Closing connections...\n");
	for (auto it : m_mapClients)
	{
		// Send them one more goodbye message.  Note that we also have the
		// connection close reason as a place to send final data.  However,
		// that's usually best left for more diagnostic/debug text not actual
		// protocol strings.
		SendStringToClient(it.first, "Server is shutting down.  Goodbye.");

		// Close the connection.  We use "linger mode" to ask SteamNetworkingSockets
		// to flush this out and close gracefully.
		m_pInterface->CloseConnection(it.first, 0, "Server Shutdown", true);
	}
	m_mapClients.clear();

	m_pInterface->CloseListenSocket(m_hListenSock);
	m_hListenSock = k_HSteamListenSocket_Invalid;

	m_pInterface->DestroyPollGroup(m_hPollGroup);
	m_hPollGroup = k_HSteamNetPollGroup_Invalid;

	// Give connections time to finish up.  This is an application layer protocol
		// here, it's not TCP.  Note that if you have an application and you need to be
		// more sure about cleanup, you won't be able to do this.  You will need to send
		// a message and then either wait for the peer to close the connection, or
		// you can pool the connection to see if any reliable data is pending.
	std::this_thread::sleep_for(std::chrono::milliseconds(500));

	GameNetworkingSockets_Kill();
}

bool GameServer::isRunning()
{
	return m_running;
}

void GameServer::init()
{
	InitSteamDatagramConnectionSockets();
	LocalUserInput_Init();

	m_pInterface = SteamNetworkingSockets();

	// Start listening
	SteamNetworkingIPAddr serverLocalAddr;
	serverLocalAddr.Clear();
	serverLocalAddr.m_port = DEFAULT_SERVER_PORT;
	SteamNetworkingConfigValue_t opt;
	opt.SetPtr(k_ESteamNetworkingConfig_Callback_ConnectionStatusChanged, (void*)SteamNetConnectionStatusChangedCallback);
	m_hListenSock = m_pInterface->CreateListenSocketIP(serverLocalAddr, 1, &opt);
	if (m_hListenSock == k_HSteamListenSocket_Invalid)
		Utils::FatalError( "Failed to listen on port %d", DEFAULT_SERVER_PORT);
	m_hPollGroup = m_pInterface->CreatePollGroup();
	if (m_hPollGroup == k_HSteamNetPollGroup_Invalid)
		Utils::FatalError("Failed to listen on port %d", DEFAULT_SERVER_PORT);
	Utils::Printf("Server listening on port %d\n", DEFAULT_SERVER_PORT);

	m_entityManager = std::make_unique<EntityManager>();
}

void GameServer::update()
{
	PollIncomingMessages();
	SendEntityUpdates();
	PollConnectionStateChanges();
	PollLocalUserInput();
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
}

inline void GameServer::InitSteamDatagramConnectionSockets()
{
	SteamDatagramErrMsg errMsg;
	if (!GameNetworkingSockets_Init(nullptr, errMsg))
		Utils::FatalError("GameNetworkingSockets_Init failed.  %s", errMsg);

	Utils::g_logTimeZero = SteamNetworkingUtils()->GetLocalTimestamp();

	SteamNetworkingUtils()->SetDebugOutputFunction(k_ESteamNetworkingSocketsDebugOutputType_Msg, Utils::DebugOutput);
}
 void GameServer::OnSteamNetConnectionStatusChanged(SteamNetConnectionStatusChangedCallback_t* pInfo)
{
	char temp[1024];

	// What's the state of the connection?
	switch (pInfo->m_info.m_eState)
	{
	case k_ESteamNetworkingConnectionState_None:
		// NOTE: We will get callbacks here when we destroy connections.  You can ignore these.
		break;

	case k_ESteamNetworkingConnectionState_ClosedByPeer:
	case k_ESteamNetworkingConnectionState_ProblemDetectedLocally:
	{
		// Ignore if they were not previously connected.  (If they disconnected
		// before we accepted the connection.)
		if (pInfo->m_eOldState == k_ESteamNetworkingConnectionState_Connected)
		{

			// Locate the client.  Note that it should have been found, because this
			// is the only codepath where we remove clients (except on shutdown),
			// and connection change callbacks are dispatched in queue order.
			auto itClient = m_mapClients.find(pInfo->m_hConn);
			assert(itClient != m_mapClients.end());

			// Select appropriate log messages
			const char* pszDebugLogAction;
			if (pInfo->m_info.m_eState == k_ESteamNetworkingConnectionState_ProblemDetectedLocally)
			{
				pszDebugLogAction = "problem detected locally";
				sprintf(temp, "Alas, %s hath fallen into shadow.  (%s)", pInfo->m_info.m_szConnectionDescription, pInfo->m_info.m_szEndDebug);
			}
			else
			{
				// Note that here we could check the reason code to see if
				// it was a "usual" connection or an "unusual" one.
				pszDebugLogAction = "closed by peer";
				sprintf(temp, "%s hath departed", pInfo->m_info.m_szConnectionDescription);
			}

			// Spew something to our own log.  Note that because we put their nick
			// as the connection description, it will show up, along with their
			// transport-specific data (e.g. their IP address)
			Utils::Printf("Connection %s %s, reason %d: %s\n",
				pInfo->m_info.m_szConnectionDescription,
				pszDebugLogAction,
				pInfo->m_info.m_eEndReason,
				pInfo->m_info.m_szEndDebug
			);

			
			// Send a message so everybody else knows what happened
			SendStringToAllClients(temp);
			SendRemoveEntityToAllClients(itClient->first, itClient->second);
			m_entityManager->destroy(itClient->second);
			m_mapClients.erase(itClient);
		}
		else
		{
			assert(pInfo->m_eOldState == k_ESteamNetworkingConnectionState_Connecting);
		}

		// Clean up the connection.  This is important!
		// The connection is "closed" in the network sense, but
		// it has not been destroyed.  We must close it on our end, too
		// to finish up.  The reason information do not matter in this case,
		// and we cannot linger because it's already closed on the other end,
		// so we just pass 0's.
		m_pInterface->CloseConnection(pInfo->m_hConn, 0, nullptr, false);
		break;
	}

	case k_ESteamNetworkingConnectionState_Connecting:
	{
		// This must be a new connection
		assert(m_mapClients.find(pInfo->m_hConn) == m_mapClients.end());
		initNewConnection(pInfo, temp);
		break;
	}

	case k_ESteamNetworkingConnectionState_Connected:
		// We will get a callback immediately after accepting the connection.
		// Since we are the server, we can ignore this, it's not news to us.
		break;

	default:
		// Silences -Wswitch
		break;
	}
}

void GameServer::initNewConnection(SteamNetConnectionStatusChangedCallback_t* pInfo, char  temp[1024])
{
	Utils::Printf("Connection request from %s", pInfo->m_info.m_szConnectionDescription);

	// A client is attempting to connect
	// Try to accept the connection.
	if (m_pInterface->AcceptConnection(pInfo->m_hConn) != k_EResultOK)
	{
		// This could fail.  If the remote host tried to connect, but then
		// disconnected, the connection may already be half closed.  Just
		// destroy whatever we have on our side.
		m_pInterface->CloseConnection(pInfo->m_hConn, 0, nullptr, false);
		Utils::Printf("Can't accept connection.  (It was already closed?)");
		return;
	}

	// Assign the poll group
	if (!m_pInterface->SetConnectionPollGroup(pInfo->m_hConn, m_hPollGroup))
	{
		m_pInterface->CloseConnection(pInfo->m_hConn, 0, nullptr, false);
		Utils::Printf("Failed to set poll group?");
		return;
	}

	// Generate a random nick.  A random temporary nick
	// is really dumb and not how you would write a real chat server.
	// You would want them to have some sort of signon message,
	// and you would keep their client in a state of limbo (connected,
	// but not logged on) until them.  I'm trying to keep this example
	// code really simple.
	char nick[64];
	sprintf(nick, "BraveWarrior%d", 10000 + (rand() % 100000));
	message m;
	sprintf(m.name, "BraveWarrior%d", 10000 + (rand() % 100000));
	m.x = 100.f;
	m.y = 100.f;

	auto newPlayer = m_entityManager->create();
	m_entityManager->emplace<std::string>(newPlayer, m.name);
	m_entityManager->emplace<CTransform>(newPlayer, glm::vec2(m.x, m.y));


	// Send them a welcome message
	sprintf(temp, "Welcome, stranger.  Thou art known to us for now as '%s'; upon thine command '/nick' we shall know thee otherwise.", nick);
	SendStringToClient(pInfo->m_hConn, temp);
	// Send client's entity to client
	m_pInterface->SendMessageToConnection(pInfo->m_hConn, &m, sizeof(m), k_nSteamNetworkingSend_Reliable, nullptr);

	// Also send them a list of everybody who is already connected
	if (m_mapClients.empty())
	{
		SendStringToClient(pInfo->m_hConn, "Thou art utterly alone.");
	}
	else
	{
		sprintf(temp, "%d companions greet you:", (int)m_mapClients.size());
	}

	// Let everybody else know who they are for now
	sprintf(temp, "Hark!  A stranger hath joined this merry host.  For now we shall call them '%s'", nick);
	SendStringToAllClients(temp, pInfo->m_hConn);

	// Add them to the client list, using std::map wacky syntax
	m_mapClients[pInfo->m_hConn];
	SetClientNick(pInfo->m_hConn, nick, newPlayer);

	return;
}

inline void GameServer::LocalUserInput_Init()
{
	s_pThreadUserInput = new std::thread([this]()
		{
			while (m_running)
			{
				char szLine[4000];
				if (!fgets(szLine, sizeof(szLine), stdin))
				{
					// Well, you would hope that you could close the handle
					// from the other thread to trigger this.  Nope.
					if (!m_running)
						return;
					quit();
					Utils::Printf("Failed to read on stdin, quitting\n");
					break;
				}

				mutexUserInputQueue.lock();
				queueUserInput.push(std::string(szLine));
				mutexUserInputQueue.unlock();
			}
		});
}

inline bool GameServer::LocalUserInput_GetNext(std::string& result)
{
	bool got_input = false;
	mutexUserInputQueue.lock();
	while (!queueUserInput.empty() && !got_input)
	{
		result = queueUserInput.front();
		queueUserInput.pop();
		Utils::ltrim(result);
		Utils::rtrim(result);
		got_input = !result.empty(); // ignore blank lines
	}
	mutexUserInputQueue.unlock();
	return got_input;
}

inline void GameServer::PollConnectionStateChanges()
{
	m_pInterface->RunCallbacks();
}

void GameServer::SteamNetConnectionStatusChangedCallback(SteamNetConnectionStatusChangedCallback_t* pInfo)
{
	instance()->OnSteamNetConnectionStatusChanged(pInfo);
}

// TEST FUNCTIONS
void GameServer::SendStringToClient(HSteamNetConnection conn, const char* str)
{
	m_pInterface->SendMessageToConnection(conn, str, (uint32)strlen(str), k_nSteamNetworkingSend_Reliable, nullptr);
}

void GameServer::SendStringToAllClients(const char* str, HSteamNetConnection except)
{
	for (auto& c : m_mapClients)
	{
		if (c.first != except)
			SendStringToClient(c.first, str);
	}
}

void GameServer::SendEntityUpdates()
{
	for (auto& c : m_mapClients)
	{
		auto view = m_entityManager->view<std::string, CTransform>();
		for (auto [entity, name, transform] : view.each())
		{
			if(abs(transform.pos.x - transform.prevPos.x) > 1 || abs(transform.pos.x - transform.prevPos.x) > 1)
			{
				message m;
				m.x = transform.pos.x;
				m.y = transform.pos.y;
				m.type = 1;
				strcpy(m.name, name.c_str());

				m_pInterface->SendMessageToConnection(c.first, &m, sizeof(m), k_nSteamNetworkingSend_Reliable, nullptr);
			}
		}
	}
}

 void GameServer::PollIncomingMessages()
{
	char temp[1024];

	while (m_running)
	{
		ISteamNetworkingMessage* pIncomingMsg = nullptr;
		int numMsgs = m_pInterface->ReceiveMessagesOnPollGroup(m_hPollGroup, &pIncomingMsg, 1);
		if (numMsgs == 0)
			break;
		if (numMsgs < 0)
			Utils::FatalError("Error checking for messages");
		assert(numMsgs == 1 && pIncomingMsg);
		auto itClient = m_mapClients.find(pIncomingMsg->m_conn);
		assert(itClient != m_mapClients.end());

		// '\0'-terminate it to make it easier to parse
		message m = *static_cast<message*>(pIncomingMsg->m_pData);

		// We don't need this anymore.
		pIncomingMsg->Release();

		// Check for known commands.  None of this example code is secure or robust.
		// Don't write a real server like this, please.

		auto view = m_entityManager->view<std::string, CTransform>();
		for (auto [entity, name, transform] : view.each())
		{
			if (name == std::string(m.name))
			{
				transform.pos.x = m.x;
				transform.pos.y = m.y;
			}
		}
	}
}

void GameServer::PollLocalUserInput()
{
	std::string cmd;
	while (m_running && LocalUserInput_GetNext(cmd))
	{
		if (strcmp(cmd.c_str(), "/quit") == 0)
		{
			quit();
			Utils::Printf("Shutting down server");
			break;
		}

		// That's the only command we support
		Utils::Printf("The server only knows one command: '/quit'");
	}
}

void GameServer::SetClientNick(HSteamNetConnection hConn, const char* nick, entt::entity newPlayer)
{

	// Remember their nick
	m_mapClients[hConn] = newPlayer;

	// Set the connection name, too, which is useful for debugging
	m_pInterface->SetConnectionName(hConn, nick);
}

void GameServer::SendRemoveEntityToAllClients(HSteamNetConnection except, entt::entity toRemove)
{
	for (auto& c : m_mapClients)
	{
		if (c.first != except)
		{
			message m;
			m.type = 2;
			strcpy(m.name, m_entityManager->get<std::string>(toRemove).c_str());
			m_pInterface->SendMessageToConnection(c.first, &m, sizeof(m), k_nSteamNetworkingSend_Reliable, nullptr);
		}
	}
}

