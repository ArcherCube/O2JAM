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
		HOLDING = 0, //δ��������
		PLAYING, //���ֲ���ʱ
		PAUSING_TO_PLAY, //�����Ѽ��أ���δ����
		COUNTING_TO_PLAY, //�����������ź󵹼�ʱ
		RECORDING,  //¼��
		PAUSING_TO_RECORD, //ѡ�������֣�׼��¼�ƹ��
		COUNTING_TO_RECORD //��������¼�ƺ󵹼�ʱ
	};
private:
	Status status;
	TimestampType musicPauseTimestamp;
	String currentMusicName;
	UINT countDown;

	//Ԫ������Ϊ�����ָ�룬��������Ϊconst
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

