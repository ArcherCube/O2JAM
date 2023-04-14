#include "MusicPlayer.h"
#include "Message.h"
#include "FileUtil.h"

UINT MusicPlayer::wDeviceID = 0;

void MusicPlayer::Open(const String& path)
{
	DWORD dwReturn;

	MCI_OPEN_PARMS OpenParms;
	OpenParms.lpstrElementName = path.c_str();

	//通过WchaStirng
	if (FileUtil::FileIsSuffix(path, TEXT("wav"))) OpenParms.lpstrDeviceType = TEXT("waveaudio");
	else OpenParms.lpstrDeviceType = TEXT("mpegvideo");

	if (dwReturn = mciSendCommand(NULL, MCI_OPEN, MCI_OPEN_TYPE | MCI_OPEN_ELEMENT,
		(DWORD_PTR)&OpenParms))
	{
		Message::ShowMCIError(dwReturn);
		return;
	}

	wDeviceID = OpenParms.wDeviceID;
}

void MusicPlayer::Close()
{
	DWORD dwReturn;
	if (dwReturn = mciSendCommand(MCI_ALL_DEVICE_ID, MCI_CLOSE, 0, NULL))
	{
		Message::ShowMCIError(dwReturn);
		return;
	}
}

void MusicPlayer::Play()
{
	DWORD dwReturn;

	MCI_PLAY_PARMS PlayParms;
	if (dwReturn = mciSendCommand(wDeviceID, MCI_PLAY, NULL, (DWORD_PTR)&PlayParms))
	{
		Message::ShowMCIError(dwReturn);
		return;
	}
}

void MusicPlayer::Seek(const UINT& position)
{
	DWORD dwReturn;

	MCI_SEEK_PARMS SeekParms;
	SeekParms.dwTo = position;
	if (dwReturn = mciSendCommand(wDeviceID, MCI_SEEK, MCI_TO, (DWORD_PTR)&SeekParms))
	{
		Message::ShowMCIError(dwReturn);
		return;
	}
}

void MusicPlayer::Pause()
{
	DWORD dwReturn;

	MCI_PLAY_PARMS PlayParms;
	if (dwReturn = mciSendCommand(wDeviceID, MCI_PAUSE, NULL, (DWORD_PTR)&PlayParms))
	{
		Message::ShowMCIError(dwReturn);
		return;
	}
}

TimestampType MusicPlayer::GetCurrentTimestamp()
{
	DWORD dwReturn;

	//获取当前MCI播放位置
	MCI_STATUS_PARMS StatusParms;
	StatusParms.dwItem = MCI_STATUS_POSITION;
	if (dwReturn = mciSendCommand(wDeviceID, MCI_STATUS, MCI_WAIT | MCI_STATUS_ITEM, (DWORD_PTR)&StatusParms))
	{
		Message::ShowMCIError(dwReturn);
		return 0;
	}

	return (TimestampType)StatusParms.dwReturn;
}