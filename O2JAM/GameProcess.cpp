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

#define PLAY_READY_TIME (3*1000) //����ǰ��ʱ��
#define NOTHING_TO_DO 

UINT Game::CommandOpen()
{
	TCHAR path[MAX_PATH] = { 0 };

	if (FileUtil::SelectFile(path))
	{
		CommandClose();

		MusicPlayer::Open(path);

		//��ȡ�ļ���
		FileUtil::GetFilenameFromPath(path, currentMusicFilename);

		//�������ɣ���������
		MelodyList* melodylist = new MelodyList();
		if (FileUtil::LoadMelodyListFromFile(currentMusicFilename, melodylist) == FAIL)
		{
			Message::ShowMessage(TEXT("���������ļ�ʧ��"));
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

		//��Recorder��ʼ¼��
		Recorder::StartRecord();

		//��¼�ļ���
		FileUtil::GetFilenameFromPath(path, currentMusicFilename);

		status = Status::PAUSING_TO_RECORD;

		return SUCCESS;
	}

	return FAIL;

}


UINT Game::CommandPlay()
{
	Message::DisplayText(TEXT("���ڲ��ţ�") + currentMusicFilename, AREA(0, KEY_POSITION_Y - 12, 192, KEY_POSITION_Y));
	switch (status)
	{
	case Status::HOLDING:
	{
		NOTHING_TO_DO;
	}
	break;
	case Status::PLAYING: //�������൱��close->open->play����ȥ���˶���Ĳ���
	{
		//���ֻ��˵���ͷ
		MusicPlayer::Seek(0);
		//���������ͬ��
		MusicNRailwaySync();
		//��ռƷְ�
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
		//����µ���ʱ
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
		//����µ���ʱ
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
	case Status::RECORDING: //�������൱��close->record->play����ȥ���˶���Ĳ���
	{
		//���ֻ��˵���ͷ
		MusicPlayer::Seek(0);

		//��Recorder��ʼ¼��
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
		//��ͣ����
		MusicPlayer::Pause();

		//��¼��ǰ����ʱ���
		musicPauseTimestamp = MusicPlayer::GetCurrentTimestamp();

		//���ֻ���
		MusicPlayer::Seek(musicPauseTimestamp > PLAY_READY_TIME ? musicPauseTimestamp - PLAY_READY_TIME : 0);

		//���������ͬ��
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
		//��ͣ����
		MusicPlayer::Pause();

		//��¼��ǰ����ʱ���
		musicPauseTimestamp = MusicPlayer::GetCurrentTimestamp();

		//���ֻ���
		MusicPlayer::Seek(musicPauseTimestamp > PLAY_READY_TIME ? musicPauseTimestamp - PLAY_READY_TIME : 0);

		//���������ͬ��
		MusicNRailwaySync();

		//ɾ������
		CentralTimer::Del(PLAY_MUSIC_TASK_TAG);
		CentralTimer::Del(TO_PLAYING_TASK_TAG);

		status = Status::PAUSING_TO_PLAY;
	}
	break;
	case Status::COUNTING_TO_RECORD:
	{
		//��ͣ����
		MusicPlayer::Pause();

		//��¼��ǰ����ʱ���
		musicPauseTimestamp = MusicPlayer::GetCurrentTimestamp();

		//���ֻ���
		MusicPlayer::Seek(musicPauseTimestamp > PLAY_READY_TIME ? musicPauseTimestamp - PLAY_READY_TIME : 0);

		//ɾ������
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
		//��ͣ����
		MusicPlayer::Pause();

		//��¼��ǰ����ʱ���
		musicPauseTimestamp = MusicPlayer::GetCurrentTimestamp();

		//���ֻ���
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
		//�ر�����
		MusicPlayer::Close();

		//��ռƷְ�
		scoreBoard->Clear();

		//��չ��
		railway->Clear();

		status = Status::HOLDING;
	}
	break;
	case Status::PAUSING_TO_PLAY:
	{
		//�ر�����
		MusicPlayer::Close();

		//��ռƷְ�
		scoreBoard->Clear();

		//��չ��
		railway->Clear();

		status = Status::HOLDING;
	}
	break;
	case Status::COUNTING_TO_PLAY:
	{
		//�ر�����
		MusicPlayer::Close();

		//��ռƷְ�
		scoreBoard->Clear();

		//��չ��
		railway->Clear();

		//ɾ������
		CentralTimer::Del(PLAY_MUSIC_TASK_TAG);
		CentralTimer::Del(TO_PLAYING_TASK_TAG);

		status = Status::HOLDING;
	}
	break;
	case Status::COUNTING_TO_RECORD:
	{
		//�ر�����
		MusicPlayer::Close();

		//д���ļ�
		FileUtil::CreateMelodyFile(currentMusicFilename, Recorder::FinishRecord());

		Message::ShowMessage(TEXT("��¼���"));

		//ɾ������
		CentralTimer::Del(PLAY_MUSIC_TASK_TAG);
		CentralTimer::Del(TO_RECORDING_TASK_TAG);

		status = Status::HOLDING;
	}
	break;
	case Status::PAUSING_TO_RECORD:
	{
		//�ر�����
		MusicPlayer::Close();

		//д���ļ�
		FileUtil::CreateMelodyFile(currentMusicFilename, Recorder::FinishRecord());

		Message::ShowMessage(TEXT("��¼���"));

		status = Status::HOLDING;
	}
	break;
	case Status::RECORDING:
	{
		//�ر�����
		MusicPlayer::Close();

		//д���ļ�
		FileUtil::CreateMelodyFile(currentMusicFilename, Recorder::FinishRecord());

		Message::ShowMessage(TEXT("��¼���"));

		status = Status::HOLDING;
	}
	break;
	default:
		break;
	}
	return 0;
}