#include "resource.h"

#include "Game.h"

#include "Renderer.h"
#include "FileUtil.h"
#include "MusicPlayer.h"
#include "Message.h"
#include "CentralTimer.h"
#include "Keyboard.h"
#include "Background.h"
#include "TextModel.h"
#include "Railway.h"
#include "Recorder.h"
#include "ScoreBoard.h"

#define NOTHING_TO_DO 

double Game::gameSpeed = DEFAULT_GAME_SPEED;

Game::Game()
	:currentMusicFilename(TEXT("")), musicPauseTimestamp(0), status(Status::HOLDING), countDown(0)
	, background(new Background()), railway(new Railway()), keyboard(new Keyboard()), scoreBoard(new ScoreBoard())
{
}


void Game::Update()
{
	switch (status)
	{
	case Status::HOLDING:
	{
		NOTHING_TO_DO;
	}
	break;
	case Status::PLAYING:
	{
		MusicNRailwaySync();
	}
	break;
	case Status::PAUSING_TO_PLAY:
	{
		NOTHING_TO_DO;
	}
	break;
	case Status::COUNTING_TO_PLAY:
	{
		MusicNRailwaySync();
	}
	break;
	case Status::COUNTING_TO_RECORD:
	{
		NOTHING_TO_DO;
	}
	break;
	case Status::PAUSING_TO_RECORD:
	{
		NOTHING_TO_DO;
	}
	break;
	case Status::RECORDING:
	{
		NOTHING_TO_DO;
	}
	break;
	default:
		break;
	}
}

void Game::MusicNRailwaySync()
{
	UINT count = railway->ScrollTo(MusicPlayer::GetCurrentTimestamp());
	if (count)
	{
		scoreBoard->Count(HitType::MISS, count);
	}
}

void Game::ProcessKeyDown(const KeyName& key)
{
	//键盘给出反馈
	keyboard->Press(key);

	//按照游戏状态来判断其他事务
	switch (status)
	{
	case Status::PLAYING:
	{

		scoreBoard->Count(railway->PressHit(key), 1);
	}
	break;
	default:
		break;
	}
}

void Game::ProcessKeyUp(const KeyName& key)
{
	//键盘给出反馈
	keyboard->Release(key);

	//按照游戏状态来判断其他事务
	switch (status)
	{
	case Status::PLAYING:
	{
		scoreBoard->Count(railway->ReleaseHit(key), 1);
	}
	case Status::RECORDING:
	{
		//松开的时候才记录
		Recorder::Record(MusicPlayer::GetCurrentTimestamp(), key, keyboard->GetLastHoldTime(key));
	}
	break;
	default:
		break;
	}
}

void Game::ProcessCommand(const int& cmd)
{
	switch (cmd)
	{
	case CMD_OPEN:
		CommandOpen();
		break;
	case CMD_CLOSE:
		CommandClose();
		break;
	case CMD_PLAY:
		CommandPlay();
		break;
	case CMD_PAUSE:
		CommandPause();
		break;
	case CMD_RECORD:
		CommandRecord();
		break;
	default:
		break;
	}

	return;
}

Game::~Game()
{
	delete keyboard;
	delete background;
	delete railway;
	delete scoreBoard;
}