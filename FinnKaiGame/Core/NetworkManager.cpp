#include "NetworkManager.h"
#include "Utils.h"

void NetworkManager::update(bool running)
{
	m_running = running;
	init();

	while (m_running)
	{
		PollIncomingMessages();
		PollConnectionStateChanges();
		PollLocalUserInput();
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}
}

void NetworkManager::init()
{
	serverAddr.Clear();
	if (!serverAddr.ParseString(ipAddress.c_str()))
		Utils::FatalError("Invalid server address '%s'", ipAddress);
	if (serverAddr.m_port == 0)
		serverAddr.m_port = 27020U;
	if (serverAddr.IsIPv6AllZeros())
	{
		Utils::FatalError("Invalid server address '%s'", ipAddress);
	}

	InitSteamDatagramConnectionSockets();
	LocalUserInput_Init();
	// Select instance to use.  For now we'll always use the default.
	m_pInterface = SteamNetworkingSockets();

	// Start connecting
	char szAddr[SteamNetworkingIPAddr::k_cchMaxString];
	serverAddr.ToString(szAddr, sizeof(szAddr), true);
	Utils::Printf("Connecting to chat server at %s", szAddr);
	SteamNetworkingConfigValue_t opt;
	opt.SetPtr(k_ESteamNetworkingConfig_Callback_ConnectionStatusChanged, (void*)SteamNetConnectionStatusChangedCallback);
	m_hConnection = m_pInterface->ConnectByIPAddress(serverAddr, 1, &opt);
	if (m_hConnection == k_HSteamNetConnection_Invalid)
		Utils::FatalError("Failed to create connection");
}

void NetworkManager::InitSteamDatagramConnectionSockets()
{
	SteamDatagramErrMsg errMsg;
	if (!GameNetworkingSockets_Init(nullptr, errMsg))
		Utils::FatalError("GameNetworkingSockets_Init failed.  %s", errMsg);

	Utils::g_logTimeZero = SteamNetworkingUtils()->GetLocalTimestamp();

	SteamNetworkingUtils()->SetDebugOutputFunction(k_ESteamNetworkingSocketsDebugOutputType_Msg, Utils::DebugOutput);
}

void NetworkManager::LocalUserInput_Init()
{
	s_pThreadUserInput = std::make_unique<std::thread>([this]()
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
					m_running = false;
					Utils::Printf("Failed to read on stdin, quitting\n");
					break;
				}

				mutexUserInputQueue.lock();
				queueUserInput.push(std::string(szLine));
				mutexUserInputQueue.unlock();
			}
		});
}

void NetworkManager::PollIncomingMessages()
{
	while (m_running)
	{
		ISteamNetworkingMessage* pIncomingMsg = nullptr;
		int numMsgs = m_pInterface->ReceiveMessagesOnConnection(m_hConnection, &pIncomingMsg, 1);
		if (numMsgs == 0)
			break;
		if (numMsgs < 0)
			Utils::FatalError("Error checking for messages");

		// Just echo anything we get from the server
		auto test = static_cast<int*>(pIncomingMsg->m_pData);
		if (*test == 1)
		{
			fwrite(pIncomingMsg->m_pData, 1, pIncomingMsg->m_cbSize, stdout);
			fputc('\n', stdout);
			fputc('Y', stdout);
			mutexCommandQueue.lock();
			commandQueue.push(*test);
			mutexCommandQueue.unlock();
		}
		fwrite(pIncomingMsg->m_pData, 1, pIncomingMsg->m_cbSize, stdout);
		fputc('\n', stdout);

		// We don't need this anymore.
		pIncomingMsg->Release();
	}
}

void NetworkManager::PollLocalUserInput()
{
	std::string cmd;
	bool test = true;
	while (m_running && LocalUserInput_GetNext(cmd))
	{

		// Check for known commands
		if (strcmp(cmd.c_str(), "/quit") == 0)
		{
			m_running = false;
			Utils::Printf("Disconnecting from chat server");

			// Close the connection gracefully.
			// We use linger mode to ask for any remaining reliable data
			// to be flushed out.  But remember this is an application
			// protocol on UDP.  See ShutdownSteamDatagramConnectionSockets
			m_pInterface->CloseConnection(m_hConnection, 0, "Goodbye", true);
			break;
		}

		// Anything else, just send it to the server and let them parse it
		m_pInterface->SendMessageToConnection(m_hConnection, cmd.c_str(), (uint32)cmd.length(), k_nSteamNetworkingSend_Reliable, nullptr);
		
	}
}

void NetworkManager::SteamNetConnectionStatusChangedCallback(SteamNetConnectionStatusChangedCallback_t* pInfo)
{
	//GameEngine::instance()->networkManager()->OnSteamNetConnectionStatusChanged(pInfo);
}

inline void NetworkManager::OnSteamNetConnectionStatusChanged(SteamNetConnectionStatusChangedCallback_t* pInfo)
{
	assert(pInfo->m_hConn == m_hConnection || m_hConnection == k_HSteamNetConnection_Invalid);

	// What's the state of the connection?
	switch (pInfo->m_info.m_eState)
	{
	case k_ESteamNetworkingConnectionState_None:
		// NOTE: We will get callbacks here when we destroy connections.  You can ignore these.
		break;

	case k_ESteamNetworkingConnectionState_ClosedByPeer:
	case k_ESteamNetworkingConnectionState_ProblemDetectedLocally:
	{
		m_running = false;

		// Print an appropriate message
		if (pInfo->m_eOldState == k_ESteamNetworkingConnectionState_Connecting)
		{
			// Note: we could distinguish between a timeout, a rejected connection,
			// or some other transport problem.
			Utils::Printf("We sought the remote host, yet our efforts were met with defeat.  (%s)", pInfo->m_info.m_szEndDebug);
		}
		else if (pInfo->m_info.m_eState == k_ESteamNetworkingConnectionState_ProblemDetectedLocally)
		{
			Utils::Printf("Alas, troubles beset us; we have lost contact with the host.  (%s)", pInfo->m_info.m_szEndDebug);
		}
		else
		{
			// NOTE: We could check the reason code for a normal disconnection
			Utils::Printf("The host hath bidden us farewell.  (%s)", pInfo->m_info.m_szEndDebug);
		}

		// Clean up the connection.  This is important!
		// The connection is "closed" in the network sense, but
		// it has not been destroyed.  We must close it on our end, too
		// to finish up.  The reason information do not matter in this case,
		// and we cannot linger because it's already closed on the other end,
		// so we just pass 0's.
		m_pInterface->CloseConnection(pInfo->m_hConn, 0, nullptr, false);
		m_hConnection = k_HSteamNetConnection_Invalid;
		break;
	}

	case k_ESteamNetworkingConnectionState_Connecting:
		// We will get this callback when we start connecting.
		// We can ignore this.
		break;

	case k_ESteamNetworkingConnectionState_Connected:
		Utils::Printf("Connected to server OK");
		break;

	default:
		// Silences -Wswitch
		break;
	}
}

void NetworkManager::PollConnectionStateChanges()
{
	m_pInterface->RunCallbacks();
}

bool NetworkManager::LocalUserInput_GetNext(std::string& result)
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
