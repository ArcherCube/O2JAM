#include "resource.h"

#include "Game.h"

#include "FileUtil.h"
#include "MusicPlayer.h"
#include "Message.h"
#include "CentralTimer.h"
#include "Keyboard.h"
#include "Railway.h"
#include "Recorder.h"
#include "ScoreBoard.h"
#include "CountDown.h"

#define PLAY_READY_TIME (3*1000) //播放前空时间
#define NOTHING_TO_DO 

UINT Game::CommandOpen()
{
	TCHAR path[MAX_PATH] = { 0 };

	if (FileUtil::SelectFile(path))
	{
		CommandClose();

		MusicPlayer::Open(path);

		//提取文件名
		FileUtil::GetFilenameFromPath(path, currentMusicFilename);

		//加载旋律（音符集）
		MelodyList* melodylist = new MelodyList();
		if (FileUtil::LoadMelodyListFromFile(currentMusicFilename, melodylist) == FAIL)
		{
			Message::ShowMessage(TEXT("加载音乐文件失败"));
			MusicPlayer::Close();
			return false;
		}
		railway->LoadMelodyList(melodylist);
		delete melodylist;

		status = Status::PAUSING_TO_PLAY;
		return SUCCESS;
	}

	return FAIL;
}


UINT Game::CommandRecord()
{
	TCHAR path[MAX_PATH] = { 0 };

	if (FileUtil::SelectFile(path))
	{
		MusicPlayer::Open(path);

		//让Recorder开始录制
		Recorder::StartRecord();

		//记录文件名
		FileUtil::GetFilenameFromPath(path, currentMusicFilename);

		status = Status::PAUSING_TO_RECORD;

		return SUCCESS;
	}

	return FAIL;

}


UINT Game::CommandPlay()
{
	Message::DisplayText(TEXT("正在播放：") + currentMusicFilename, AREA(0, KEY_POSITION_Y - 12, 192, KEY_POSITION_Y));
	switch (status)
	{
	case Status::HOLDING:
	{
		NOTHING_TO_DO;
	}
	break;
	case Status::PLAYING: //本质上相当于close->open->play，但去掉了多余的操作
	{
		//音乐回退到开头
		MusicPlayer::Seek(0);
		//轨道与音乐同步
		MusicNRailwaySync();
		//清空计分板
		scoreBoard->Clear();

		CentralTimer::Add(PLAY_MUSIC_TASK_TAG, PLAY_READY_TIME, [this]
			{
				MusicPlayer::Play();
			});
		CentralTimer::Add(TO_PLAYING_TASK_TAG, PLAY_READY_TIME, [this]
			{
				status = Status::PLAYING;
			});
		status = Status::COUNTING_TO_PLAY;
	}
	break;
	case Status::PAUSING_TO_PLAY:
	{
		CountDown::ShowCountDown(PLAY_READY_TIME);

		TimestampType waitTime = PLAY_READY_TIME > musicPauseTimestamp ? PLAY_READY_TIME - musicPauseTimestamp : 0;
		CentralTimer::Add(PLAY_MUSIC_TASK_TAG, waitTime, [this]
			{
				MusicPlayer::Play();
			});
		CentralTimer::Add(TO_PLAYING_TASK_TAG, PLAY_READY_TIME, [this]
			{
				status = Status::PLAYING;
			});
		status = Status::COUNTING_TO_PLAY;
	}
	break;
	case Status::COUNTING_TO_PLAY:
	{
		TimestampType waitTime = PLAY_READY_TIME > musicPauseTimestamp ? PLAY_READY_TIME - musicPauseTimestamp : 0;
		//会更新倒计时
		CentralTimer::Add(PLAY_MUSIC_TASK_TAG, waitTime, [this]
			{
				MusicPlayer::Play();
			});
		CentralTimer::Add(TO_PLAYING_TASK_TAG, PLAY_READY_TIME, [this]
			{
				status = Status::PLAYING;
			});
	}
	break;
	case Status::COUNTING_TO_RECORD:
	{
		TimestampType waitTime = PLAY_READY_TIME > musicPauseTimestamp ? PLAY_READY_TIME - musicPauseTimestamp : 0;
		//会更新倒计时
		CentralTimer::Add(PLAY_MUSIC_TASK_TAG, waitTime, [this]
			{
				MusicPlayer::Play();
			});
		CentralTimer::Add(TO_RECORDING_TASK_TAG, waitTime, [this]
			{
				status = Status::RECORDING;
			});
	}
	break;
	case Status::PAUSING_TO_RECORD:
	{
		TimestampType waitTime = PLAY_READY_TIME > musicPauseTimestamp ? PLAY_READY_TIME - musicPauseTimestamp : 0;
		CentralTimer::Add(PLAY_MUSIC_TASK_TAG, waitTime, [this]
			{
				MusicPlayer::Play();
			});
		CentralTimer::Add(TO_RECORDING_TASK_TAG, waitTime, [this]
			{
				status = Status::RECORDING;
			});
	}
	break;
	case Status::RECORDING: //本质上相当于close->record->play，但去掉了多余的操作
	{
		//音乐回退到开头
		MusicPlayer::Seek(0);

		//让Recorder开始录制
		Recorder::StartRecord();

		CentralTimer::Add(PLAY_MUSIC_TASK_TAG, PLAY_READY_TIME, [this]
			{
				MusicPlayer::Play();
			});
		CentralTimer::Add(TO_RECORDING_TASK_TAG, PLAY_READY_TIME, [this]
			{
				status = Status::RECORDING;
			});
		status = Status::COUNTING_TO_RECORD;
	}
	break;
	default:
		break;
	}
	return 0;
}


UINT Game::CommandPause()
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
		//暂停音乐
		MusicPlayer::Pause();

		//记录当前音乐时间戳
		musicPauseTimestamp = MusicPlayer::GetCurrentTimestamp();

		//音乐回退
		MusicPlayer::Seek(musicPauseTimestamp > PLAY_READY_TIME ? musicPauseTimestamp - PLAY_READY_TIME : 0);

		//轨道与音乐同步
		MusicNRailwaySync();

		status = Status::PAUSING_TO_PLAY;
	}
	break;
	case Status::PAUSING_TO_PLAY:
	{
		NOTHING_TO_DO;
	}
	break;
	case Status::COUNTING_TO_PLAY:
	{
		//暂停音乐
		MusicPlayer::Pause();

		//记录当前音乐时间戳
		musicPauseTimestamp = MusicPlayer::GetCurrentTimestamp();

		//音乐回退
		MusicPlayer::Seek(musicPauseTimestamp > PLAY_READY_TIME ? musicPauseTimestamp - PLAY_READY_TIME : 0);

		//轨道与音乐同步
		MusicNRailwaySync();

		//删除事务
		CentralTimer::Del(PLAY_MUSIC_TASK_TAG);
		CentralTimer::Del(TO_PLAYING_TASK_TAG);

		status = Status::PAUSING_TO_PLAY;
	}
	break;
	case Status::COUNTING_TO_RECORD:
	{
		//暂停音乐
		MusicPlayer::Pause();

		//记录当前音乐时间戳
		musicPauseTimestamp = MusicPlayer::GetCurrentTimestamp();

		//音乐回退
		MusicPlayer::Seek(musicPauseTimestamp > PLAY_READY_TIME ? musicPauseTimestamp - PLAY_READY_TIME : 0);

		//删除事务
		CentralTimer::Del(PLAY_MUSIC_TASK_TAG);
		CentralTimer::Del(TO_RECORDING_TASK_TAG);

		status = Status::PAUSING_TO_RECORD;
	}
	break;
	case Status::PAUSING_TO_RECORD:
	{
		NOTHING_TO_DO;
	}
	break;
	case Status::RECORDING:
	{
		//暂停音乐
		MusicPlayer::Pause();

		//记录当前音乐时间戳
		musicPauseTimestamp = MusicPlayer::GetCurrentTimestamp();

		//音乐回退
		MusicPlayer::Seek(musicPauseTimestamp > PLAY_READY_TIME ? musicPauseTimestamp - PLAY_READY_TIME : 0);

		status = Status::PAUSING_TO_RECORD;
	}
	break;
	default:
		break;
	}

	return 0;
}


UINT Game::CommandClose()
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
		//关闭音乐
		MusicPlayer::Close();

		//清空计分板
		scoreBoard->Clear();

		//清空轨道
		railway->Clear();

		status = Status::HOLDING;
	}
	break;
	case Status::PAUSING_TO_PLAY:
	{
		//关闭音乐
		MusicPlayer::Close();

		//清空计分板
		scoreBoard->Clear();

		//清空轨道
		railway->Clear();

		status = Status::HOLDING;
	}
	break;
	case Status::COUNTING_TO_PLAY:
	{
		//关闭音乐
		MusicPlayer::Close();

		//清空计分板
		scoreBoard->Clear();

		//清空轨道
		railway->Clear();

		//删除事务
		CentralTimer::Del(PLAY_MUSIC_TASK_TAG);
		CentralTimer::Del(TO_PLAYING_TASK_TAG);

		status = Status::HOLDING;
	}
	break;
	case Status::COUNTING_TO_RECORD:
	{
		//关闭音乐
		MusicPlayer::Close();

		//写入文件
		FileUtil::CreateMelodyFile(currentMusicFilename, Recorder::FinishRecord());

		Message::ShowMessage(TEXT("记录完成"));

		//删除事务
		CentralTimer::Del(PLAY_MUSIC_TASK_TAG);
		CentralTimer::Del(TO_RECORDING_TASK_TAG);

		status = Status::HOLDING;
	}
	break;
	case Status::PAUSING_TO_RECORD:
	{
		//关闭音乐
		MusicPlayer::Close();

		//写入文件
		FileUtil::CreateMelodyFile(currentMusicFilename, Recorder::FinishRecord());

		Message::ShowMessage(TEXT("记录完成"));

		status = Status::HOLDING;
	}
	break;
	case Status::RECORDING:
	{
		//关闭音乐
		MusicPlayer::Close();

		//写入文件
		FileUtil::CreateMelodyFile(currentMusicFilename, Recorder::FinishRecord());

		Message::ShowMessage(TEXT("记录完成"));

		status = Status::HOLDING;
	}
	break;
	default:
		break;
	}
	return 0;
}