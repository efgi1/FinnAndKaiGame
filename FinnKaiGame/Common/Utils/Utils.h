#pragma once
#include <windows.h> // Ug, for NukeProcess -- see below
#include <assert.h>
#include <string>

#include <stdio.h>
#include <stdarg.h>

#include <algorithm>
#include <string>
#include <random>
#include <chrono>
#include <thread>
#include <mutex>
#include <queue>
#include <map>
#include <cctype>

#include <steam/steamnetworkingsockets.h>
#include <steam/isteamnetworkingutils.h>
/////////////////////////////////////////////////////////////////////////////
//
// Common stuff
//
/////////////////////////////////////////////////////////////////////////////


namespace Utils
{

	static SteamNetworkingMicroseconds g_logTimeZero = 0;
	// We do this because I won't want to figure out how to cleanly shut
	// down the thread that is reading from stdin.
	static void NukeProcess(int rc)
	{
		ExitProcess(rc);
	}

	static void DebugOutput(ESteamNetworkingSocketsDebugOutputType eType, const char* pszMsg)
	{
		
		SteamNetworkingMicroseconds time = SteamNetworkingUtils()->GetLocalTimestamp() - g_logTimeZero;
		printf("%10.6f %s\n", time * 1e-6, pszMsg);
		fflush(stdout);
		if (eType == k_ESteamNetworkingSocketsDebugOutputType_Bug)
		{
			fflush(stdout);
			fflush(stderr);
			NukeProcess(1);
		}
		
	}

	static void FatalError( const char* fmt, ...)
	{
		char text[2048];
		va_list ap;
		va_start(ap, fmt);
		vsprintf(text, fmt, ap);
		va_end(ap);
		char* nl = strchr(text, '\0') - 1;
		if (nl >= text && *nl == '\n')
			*nl = '\0';
		DebugOutput(k_ESteamNetworkingSocketsDebugOutputType_Bug, text);
	}

	static void Printf(const char* fmt, ...)
	{
		char text[2048];
		va_list ap;
		va_start(ap, fmt);
		vsprintf(text, fmt, ap);
		va_end(ap);
		char* nl = strchr(text, '\0') - 1;
		if (nl >= text && *nl == '\n')
			*nl = '\0';
		DebugOutput(k_ESteamNetworkingSocketsDebugOutputType_Msg, text);
	}

	
	// trim from start (in place)
	static inline void ltrim(std::string& s) {
		s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) {
			return !std::isspace(ch);
			}));
	}

	// trim from end (in place)
	static inline void rtrim(std::string& s) {
		s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) {
			return !std::isspace(ch);
			}).base(), s.end());
	}

};