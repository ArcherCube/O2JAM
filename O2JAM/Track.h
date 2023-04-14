#pragma once

#include <Windows.h>
#include <unordered_map>

#include "CommonType.h"
#include "Constant.h"
#include "Note.h"

//������Ч����y��Χ
#define BAD_AREA_TOP			KEY_POSITION_Y - (Game::gameSpeed/3)
#define GOOD_AREA_TOP			KEY_POSITION_Y - (Game::gameSpeed/6)
#define COOL_AREA_TOP			KEY_POSITION_Y - (Game::gameSpeed/10)
#define COOL_AREA_BOTTOM		KEY_POSITION_Y + (Game::gameSpeed/15)
#define GOOD_AREA_BOTTOM		KEY_POSITION_Y + (Game::gameSpeed/7.5)
#define BAD_AREA_BOTTOM			KEY_POSITION_Y + (Game::gameSpeed/5)

class Track
{
private:
	//��ǰ�����������ɫ����
	const Note::NoteColorType noteColorType;

	//�����Ӧ������x����
	const double x;

	//�����Ķ��У��洢����δ��hit��note
	NoteDeque* const noteHitDeque;
	//��ʵ��¼�����Ķ���
	NoteDeque* const noteDeque;

	//��ʾ��ǰ���찴ס�ġ�������
	Note* holdingNote;
public:
	explicit Track(const Note::NoteColorType& noteColorType, const double& x);

	const UINT AddNote(const TimestampType& timestamp, const UINT& delay) const;

	const NoteDeque* const GetNoteDeque() const;

	const HitType PressHit();

	const HitType ReleaseHit();

	const UINT Shift(double shift);

	const UINT Clear();

	virtual ~Track();
};
typedef std::unordered_map<KeyName, Track* const> TrackHashmap;
typedef TrackHashmap::const_iterator TrackHashmapCiter;