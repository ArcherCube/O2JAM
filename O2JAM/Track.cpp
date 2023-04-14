#include "Track.h"

#include "Game.h"
#include "Message.h"

Track::Track(const Note::NoteColorType& noteColorType, const double& x)
	:noteColorType(noteColorType), x(x), noteDeque(new NoteDeque()), noteHitDeque(new NoteDeque()), holdingNote(nullptr)
{

}

const UINT Track::AddNote(const TimestampType& timestamp, const UINT& delay) const
{
	//音符的高度为其delay（时间，秒）*speed（速度，像素/秒），delay大于等于NOT_DELAY_LIMIT才按“延音”算，否则按短音
	UINT height = delay >= NOTE_DELAY_MIN ? (UINT)(((double)delay / 1000.0) * Game::gameSpeed) : NOTE_DEFAULT_HEIGHT;

	//音符的移动距离为speed（速度，像素/秒）*timestamp（时间，秒），以键盘y坐标为音符的起始y（也就是‘最佳敲击点’）
	//之后，先往上移一个自身高度的距离，再往下移一个NOTE_DEFAULT_HEIGHT，使得‘延音’移动到正确的起始点，同时保持对齐（相当于音符先按上线往上翻转，再往下平移对齐到原本的高度（时间戳））
	//最后再往上移一个移动距离
	double y = KEY_POSITION_Y - height + NOTE_DEFAULT_HEIGHT - Game::gameSpeed * (double)timestamp / 1000.0;

	Note* const note = new Note(noteColorType, POSITION(x, y), delay, height);
	noteDeque->push_back(note);
	noteHitDeque->push_back(note);

	//未启用判断
	return 0;
}

const NoteDeque* const Track::GetNoteDeque() const
{
	//返回的是记录了所有note的队列
	return noteDeque;
}

const HitType Track::PressHit()
{
	if (noteHitDeque->empty()) return HitType::AIR;

	Note* const firstNote = noteHitDeque->front();

	if (!firstNote) return HitType::AIR;

	//判定的高度为actor位图左下角，所以用左上角坐标加自身高度
	const double judgeY = firstNote->GetY() + firstNote->GetHeight();

	//按区域从内往外开始判断
	if (judgeY >= COOL_AREA_TOP && judgeY <= COOL_AREA_BOTTOM)
	{
		if (firstNote->IsDelay())
		{
			holdingNote = firstNote;
			holdingNote->Hold();
			return HitType::AIR;
		}
		else
		{
			noteHitDeque->pop_front();
			return HitType::COOL;
		}
	}
	else if (judgeY >= GOOD_AREA_TOP && judgeY <= GOOD_AREA_BOTTOM)
	{
		if (firstNote->IsDelay())
		{
			holdingNote = firstNote;
			holdingNote->Hold();
			return HitType::AIR;
		}
		else
		{
			noteHitDeque->pop_front();
			return HitType::GOOD;
		}
	}
	else if (judgeY >= BAD_AREA_TOP && judgeY <= BAD_AREA_BOTTOM)
	{
		//bad的情况下无论短音、延音，直接pop
		noteHitDeque->pop_front();

		return HitType::BAD;
	}
	else
	{
		return HitType::AIR;
	}
}

const HitType Track::ReleaseHit()
{
	//三种情况下，release为air：不存在未hit的note；不存在holding的note；holding的note不是当前音轨第一个未hit的note（此时holdingNote已经miss掉）
	//第二种情况可以归并到第三种，因为此时noteHitDeque不为空，若holdingNote为nullptr，自然不会等于noteHitDeque->front()
	if (noteHitDeque->empty() || holdingNote != noteHitDeque->front())
	{
		holdingNote = nullptr;
		return HitType::AIR;
	}

	//release只需要判断延音的结尾
	//判定的高度为actor所在高度的左上角，所以用左上角坐标即可
	const double judgeY = noteHitDeque->front()->GetY();

	//直接从队列移除第一个音符；
	noteHitDeque->pop_front();

	//holdingNote置空（经过前面的判定，它在这个操作下会被release
	holdingNote = nullptr;

	//按区域从内往外开始判断
	if (judgeY >= COOL_AREA_TOP && judgeY <= COOL_AREA_BOTTOM)
	{
		return HitType::COOL;
	}
	else if (judgeY >= GOOD_AREA_TOP && judgeY <= GOOD_AREA_BOTTOM)
	{
		return HitType::GOOD;
	}
	else if (judgeY >= BAD_AREA_TOP && judgeY <= BAD_AREA_BOTTOM)
	{
		return HitType::BAD;
	}
	else
	{
		//注意release在范围以外的的话是miss
		return HitType::MISS;
	}
}

const UINT Track::Shift(double shift)
{
	for (const auto& note : *noteDeque)
	{
		note->Shift(shift);
	}

	//全部音符位移后，递归判断第一个未hit的note是否越过了判定区
	UINT missCount = 0;
	while (!noteHitDeque->empty() && (noteHitDeque->front()->GetY() + (noteHitDeque->front()->IsHolding() ? 0 : noteHitDeque->front()->GetHeight())) > BAD_AREA_BOTTOM)
	{
		noteHitDeque->pop_front();
		missCount++;
	}

	return missCount;
}

const UINT Track::Clear()
{
	for (const auto& note : *noteDeque)
	{
		delete note;
	}
	noteDeque->clear();
	noteHitDeque->clear();

	//未启用判断
	return 0;
}


Track::~Track()
{
	Clear();

	delete noteDeque;
	delete noteHitDeque;
}