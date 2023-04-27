#pragma once

#include <windows.h>
#include "CommonType.h"

#define PLAY_MUSIC_TASK_TAG TEXT("toPlayMusic")
#define TO_PLAYING_TASK_TAG TEXT("toPlaying")
#define TO_RECORDING_TASK_TAG TEXT("toRecording")

class Renderer;
class FileUtil;
class MusicPlayer;
class Keyboard;
class Background;
class Railway;
class ScoreBoard;

class Game
{
private:
	enum class Status :UINT
	{
		HOLDING = 0, //未加载音乐
		PLAYING, //音乐播放时
		PAUSING_TO_PLAY, //音乐已加载，但未播放
		COUNTING_TO_PLAY, //（继续）播放后倒计时
		RECORDING,  //录制
		PAUSING_TO_RECORD, //选择了音乐，准备录制轨道
		COUNTING_TO_RECORD //（继续）录制后倒计时
	};
private:
	Status status;
	TimestampType musicPauseTimestamp;
	String currentMusicName;
	UINT countDown;

	//元件类且为对象的指针，变量本身为const
	Keyboard* const keyboard;
	Background* const background;
	Railway* const railway;
	ScoreBoard* const scoreBoard;
public:
	static double gameSpeed;
private:
	UINT CommandOpen();
	UINT CommandRecord();
	UINT CommandPlay();
	UINT CommandPause();
	UINT CommandClose();

	void MusicNRailwaySync();
public:
	explicit Game();

	void Update();
	void ProcessCommand(const int& cmd);
	void ProcessKeyDown(const KeyName& key);
	void ProcessKeyUp(const KeyName& key);

	virtual ~Game();
};

