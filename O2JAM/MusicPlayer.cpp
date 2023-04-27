#include "MusicPlayer.h"
#include "Message.h"
#include "Path.h"
#include "FileUtil.h"

UINT MusicPlayer::wDeviceID = 0;
const std::vector<MusicPlayer::MusicType> MusicPlayer::acceptMusicTypes =
{
	{
		TEXT(".wav"),
		TEXT("waveaudio")
	},
	{
		TEXT(".mp3"),
		TEXT("mpegvideo")
	}
};

bool MusicPlayer::Open(const String& musicName)
{
	MCI_OPEN_PARMS openParms = { 0 };
	for (auto current = acceptMusicTypes.cbegin(); current != acceptMusicTypes.cend(); ++current)
	{
		const String& fullPath = Path::musicPath + musicName + current->suffix;
		if (FileUtil::IsExist(fullPath)) {
			openParms.lpstrElementName = fullPath.c_str();
			openParms.lpstrDeviceType = current->deviceType;
			break;
		}
		if ((current + 1) == acceptMusicTypes.cend()) {
			Message::ShowMessage(TEXT("音乐文件不存在，请确认"));
			return false;
		}
	}

	DWORD error = mciSendCommand(NULL, MCI_OPEN, MCI_OPEN_TYPE | MCI_OPEN_ELEMENT, (DWORD_PTR)&openParms);
	if (error)
	{
		Message::ShowMCIError(error);
		return false;
	}

	wDeviceID = openParms.wDeviceID;

	return true;
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