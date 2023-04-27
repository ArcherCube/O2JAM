#pragma once

#include <windows.h>
#include <mmsystem.h>
#include <vector>
#include "CommonType.h"
#pragma comment(lib,"winmm.lib")

class MusicPlayer
{
private:
	typedef struct 
	{
		LPCTSTR suffix;
		LPCTSTR deviceType;
	} MusicType;
	static const std::vector<MusicType> acceptMusicTypes;
	static UINT wDeviceID;

public:
	static bool Open(const String& musicName);
	static void Close();

	static void Play();
	static void Seek(const UINT& position);
	static void Pause();

	static TimestampType GetCurrentTimestamp();
};

