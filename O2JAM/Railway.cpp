#include "Railway.h"

#include "Game.h"

#include "Message.h"

Railway::Railway()
	:trackHashmap(new TrackHashmap()), currentTimestamp(0)
{
	//�˴���֤��keyboard��Ӧ
	trackHashmap->insert({ KeyName::A,new Track(Note::NoteColorType::WHITE, TRACK_A_POSITION_X) });
	trackHashmap->insert({ KeyName::S,new Track(Note::NoteColorType::BLUE,  TRACK_S_POSITION_X) });
	trackHashmap->insert({ KeyName::D,new Track(Note::NoteColorType::WHITE,  TRACK_D_POSITION_X) });
	trackHashmap->insert({ KeyName::SPACE,new Track(Note::NoteColorType::YELLOW,  TRACK_SPACE_POSITION_X) });
	trackHashmap->insert({ KeyName::J,new Track(Note::NoteColorType::WHITE,  TRACK_J_POSITION_X) });
	trackHashmap->insert({ KeyName::K,new Track(Note::NoteColorType::BLUE, TRACK_K_POSITION_X) });
	trackHashmap->insert({ KeyName::L,new Track(Note::NoteColorType::WHITE,  TRACK_L_POSITION_X) });
}

const UINT Railway::LoadMelodyList(const MelodyList* const melodyList) const
{
	for (const auto& melody : *melodyList)
	{
		for (const auto& delayPair : melody.delayHashmap)
		{
			const KeyName& keyName = delayPair.first;
			const TimestampType& delay = delayPair.second;
			if (delay)
			{
				trackHashmap->at(keyName)->AddNote(melody.timestamp, delay);
			}
		}
	}

	//Ϊ�����ж�
	return 0;
}

const HitType Railway::PressHit(const KeyName& key) const
{
	TrackHashmapCiter cIter = trackHashmap->find(key);

	//�����찴��������û�㵽
	if (cIter == trackHashmap->cend()) return HitType::AIR;

	return cIter->second->PressHit();
}

const HitType Railway::ReleaseHit(const KeyName& key) const
{
	TrackHashmapCiter cIter = trackHashmap->find(key);

	//�����찴��������û�㵽
	if (cIter == trackHashmap->cend()) return HitType::AIR;

	return cIter->second->ReleaseHit();
}

const UINT Railway::ScrollTo(const TimestampType& timestamp)
{
	//�����ƫ�������µ�ʱ���-�ϴε�ʱ����õ�ʱ�����룩��ת��Ϊ�� * �ٶ�
	double&& shift = (double)((long)timestamp - (long)currentTimestamp) / 1000.0 * Game::gameSpeed;

	//���¹����ʱ��λ
	currentTimestamp = timestamp;

	UINT missCount = 0;
	for (const auto& trackPair : *trackHashmap)
	{
		Track* const track = trackPair.second;
		if (track)
		{
			missCount += track->Shift(shift);
		}
	}

	return missCount;
}

const UINT Railway::Clear()
{
	for (const auto& trackPair : *trackHashmap)
	{
		trackPair.second->Clear();
	}
	currentTimestamp = 0;
	//δ�����ж�
	return 0;
}

Railway::~Railway()
{
	for (const auto& trackPair : *trackHashmap)
	{
		delete trackPair.second;
	}
	trackHashmap->clear();
	delete trackHashmap;
}


