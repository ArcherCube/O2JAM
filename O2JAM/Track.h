#pragma once

#include <Windows.h>
#include <unordered_map>

#include "CommonType.h"
#include "Constant.h"
#include "Note.h"

//按键有效区的y范围
#define BAD_AREA_TOP			KEY_POSITION_Y - (Game::gameSpeed/3)
#define GOOD_AREA_TOP			KEY_POSITION_Y - (Game::gameSpeed/6)
#define COOL_AREA_TOP			KEY_POSITION_Y - (Game::gameSpeed/10)
#define COOL_AREA_BOTTOM		KEY_POSITION_Y + (Game::gameSpeed/15)
#define GOOD_AREA_BOTTOM		KEY_POSITION_Y + (Game::gameSpeed/7.5)
#define BAD_AREA_BOTTOM			KEY_POSITION_Y + (Game::gameSpeed/5)

class Track
{
private:
	//当前音轨的音符颜色类型
	const Note::NoteColorType noteColorType;

	//音轨对应按键的x坐标
	const double x;

	//音符的队列，存储的是未被hit的note
	NoteDeque* const noteHitDeque;
	//真实记录音符的队列
	NoteDeque* const noteDeque;

	//表示当前音轨按住的“延音”
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