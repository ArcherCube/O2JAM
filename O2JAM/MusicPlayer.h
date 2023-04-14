#pragma once

#include <windows.h>
#include <mmsystem.h>
#include "CommonType.h"
#pragma comment(lib,"winmm.lib")


class MusicPlayer
{
private:
	static UINT wDeviceID;

public:
	static void Open(const String& path);
	static void Close();

	static void Play();
	static void Seek(const UINT& position);
	static void Pause();

	static TimestampType GetCurrentTimestamp();
};

