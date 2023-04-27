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
#include "Melody.h"
#include "Path.h"

#define READY_TIME (3*1000) //播放前准备时间
#define NOTHING_TO_DO 

UINT Game::CommandOpen()
{
	TCHAR path[MAX_PATH] = { 0 };

	if (FileUtil::Select(path, Path::melodyPath, TEXT("选择旋律"), { MELODY_FILE_SUFFIX }))
	{
		CommandClose();

		//提取文件名
		const auto& musicName = FileUtil::GetFilename(path);

		if (MusicPlayer::Open(musicName))
		{
			//加载旋律（音符集）
			Melody* melody = new Melody(musicName);
			if (!melody->IsLoaded())
			{
				Message::ShowMessage(TEXT("加载旋律失败"));
				MusicPlayer::Close();
				return FAIL;
			}
			railway->LoadMelody(melody);
			delete melody;

			status = Status::PAUSING_TO_PLAY;

			this->currentMusicName = musicName;

			return SUCCESS;
		}
	}

	return FAIL;
}


UINT Game::CommandRecord()
{
	TCHAR path[MAX_PATH] = { 0 };

	if (FileUtil::Select(path, Path::musicPath, TEXT("选择要录制旋律的音乐"), { TEXT(".wav"),TEXT(".mp3") }))
	{
		//提取文件名
		const auto& musicName = FileUtil::GetFilename(path);

		if (MusicPlayer::Open(musicName))
		{
			//让Recorder开始录制
			Recorder::StartRecord();

			this->currentMusicName = musicName;

			status = Status::PAUSING_TO_RECORD;

			return SUCCESS;
		}
	}

	return FAIL;

}


UINT Game::CommandPlay()
{
	Message::DisplayText(TEXT("正在播放：") + this->currentMusicName, AREA(0, KEY_POSITION_Y - 12, 192, KEY_POSITION_Y));
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

		CentralTimer::Add(PLAY_MUSIC_TASK_TAG, READY_TIME, [this]
			{
				MusicPlayer::Play();
			}
		);
		CentralTimer::Add(TO_PLAYING_TASK_TAG, READY_TIME, [this]
			{
				status = Status::PLAYING;
			}
		);
		status = Status::COUNTING_TO_PLAY;
	}
	break;
	case Status::PAUSING_TO_PLAY:
	{
		CountDown::ShowCountDown(READY_TIME);

		TimestampType waitTime = READY_TIME > musicPauseTimestamp ? READY_TIME - musicPauseTimestamp : 0;
		CentralTimer::Add(PLAY_MUSIC_TASK_TAG, waitTime, [this]
			{
				MusicPlayer::Play();
			}
		);
		CentralTimer::Add(TO_PLAYING_TASK_TAG, READY_TIME, [this]
			{
				status = Status::PLAYING;
			}
		);
		status = Status::COUNTING_TO_PLAY;
	}
	break;
	case Status::COUNTING_TO_PLAY:
	{
		TimestampType waitTime = READY_TIME > musicPauseTimestamp ? READY_TIME - musicPauseTimestamp : 0;
		//会更新倒计时
		CentralTimer::Add(PLAY_MUSIC_TASK_TAG, waitTime, [this]
			{
				MusicPlayer::Play();
			}
		);
		CentralTimer::Add(TO_PLAYING_TASK_TAG, READY_TIME, [this]
			{
				status = Status::PLAYING;
			}
		);
	}
	break;
	case Status::COUNTING_TO_RECORD:
	{
		TimestampType waitTime = READY_TIME > musicPauseTimestamp ? READY_TIME - musicPauseTimestamp : 0;
		//会更新倒计时
		CentralTimer::Add(PLAY_MUSIC_TASK_TAG, waitTime, [this]
			{
				MusicPlayer::Play();
			}
		);
		CentralTimer::Add(TO_RECORDING_TASK_TAG, waitTime, [this]
			{
				status = Status::RECORDING;
			}
		);
	}
	break;
	case Status::PAUSING_TO_RECORD:
	{
		TimestampType waitTime = READY_TIME > musicPauseTimestamp ? READY_TIME - musicPauseTimestamp : 0;
		CentralTimer::Add(PLAY_MUSIC_TASK_TAG, waitTime, [this]
			{
				MusicPlayer::Play();
			}
		);
		CentralTimer::Add(TO_RECORDING_TASK_TAG, waitTime, [this]
			{
				status = Status::RECORDING;
			}
		);
	}
	break;
	case Status::RECORDING: //本质上相当于close->record->play，但去掉了多余的操作
	{
		//音乐回退到开头
		MusicPlayer::Seek(0);

		//让Recorder开始录制
		Recorder::StartRecord();

		CentralTimer::Add(PLAY_MUSIC_TASK_TAG, READY_TIME, [this]
			{
				MusicPlayer::Play();
			}
		);
		CentralTimer::Add(TO_RECORDING_TASK_TAG, READY_TIME, [this]
			{
				status = Status::RECORDING;
			}
		);
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
		MusicPlayer::Seek(musicPauseTimestamp > READY_TIME ? musicPauseTimestamp - READY_TIME : 0);

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
		MusicPlayer::Seek(musicPauseTimestamp > READY_TIME ? musicPauseTimestamp - READY_TIME : 0);

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
		MusicPlayer::Seek(musicPauseTimestamp > READY_TIME ? musicPauseTimestamp - READY_TIME : 0);

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
		MusicPlayer::Seek(musicPauseTimestamp > READY_TIME ? musicPauseTimestamp - READY_TIME : 0);

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
		FileUtil::Create(Recorder::FinishRecord().ToText(), Path::melodyPath + currentMusicName + MELODY_FILE_SUFFIX);

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
		FileUtil::Create(Recorder::FinishRecord().ToText(), Path::melodyPath + currentMusicName + MELODY_FILE_SUFFIX);

		Message::ShowMessage(TEXT("记录完成"));

		status = Status::HOLDING;
	}
	break;
	case Status::RECORDING:
	{
		//关闭音乐
		MusicPlayer::Close();

		//写入文件
		FileUtil::Create(Recorder::FinishRecord().ToText(), Path::melodyPath + currentMusicName + MELODY_FILE_SUFFIX);

		Message::ShowMessage(TEXT("记录完成"));

		status = Status::HOLDING;
	}
	break;
	default:
		break;
	}
	return 0;
}