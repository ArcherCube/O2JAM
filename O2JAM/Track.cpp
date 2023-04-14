#include "Track.h"

#include "Game.h"
#include "Message.h"

Track::Track(const Note::NoteColorType& noteColorType, const double& x)
	:noteColorType(noteColorType), x(x), noteDeque(new NoteDeque()), noteHitDeque(new NoteDeque()), holdingNote(nullptr)
{

}

const UINT Track::AddNote(const TimestampType& timestamp, const UINT& delay) const
{
	//�����ĸ߶�Ϊ��delay��ʱ�䣬�룩*speed���ٶȣ�����/�룩��delay���ڵ���NOT_DELAY_LIMIT�Ű����������㣬���򰴶���
	UINT height = delay >= NOTE_DELAY_MIN ? (UINT)(((double)delay / 1000.0) * Game::gameSpeed) : NOTE_DEFAULT_HEIGHT;

	//�������ƶ�����Ϊspeed���ٶȣ�����/�룩*timestamp��ʱ�䣬�룩���Լ���y����Ϊ��������ʼy��Ҳ���ǡ�����û��㡯��
	//֮����������һ������߶ȵľ��룬��������һ��NOTE_DEFAULT_HEIGHT��ʹ�á��������ƶ�����ȷ����ʼ�㣬ͬʱ���ֶ��루�൱�������Ȱ��������Ϸ�ת��������ƽ�ƶ��뵽ԭ���ĸ߶ȣ�ʱ�������
	//�����������һ���ƶ�����
	double y = KEY_POSITION_Y - height + NOTE_DEFAULT_HEIGHT - Game::gameSpeed * (double)timestamp / 1000.0;

	Note* const note = new Note(noteColorType, POSITION(x, y), delay, height);
	noteDeque->push_back(note);
	noteHitDeque->push_back(note);

	//δ�����ж�
	return 0;
}

const NoteDeque* const Track::GetNoteDeque() const
{
	//���ص��Ǽ�¼������note�Ķ���
	return noteDeque;
}

const HitType Track::PressHit()
{
	if (noteHitDeque->empty()) return HitType::AIR;

	Note* const firstNote = noteHitDeque->front();

	if (!firstNote) return HitType::AIR;

	//�ж��ĸ߶�Ϊactorλͼ���½ǣ����������Ͻ����������߶�
	const double judgeY = firstNote->GetY() + firstNote->GetHeight();

	//������������⿪ʼ�ж�
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
		//bad����������۶�����������ֱ��pop
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
	//��������£�releaseΪair��������δhit��note��������holding��note��holding��note���ǵ�ǰ�����һ��δhit��note����ʱholdingNote�Ѿ�miss����
	//�ڶ���������Թ鲢�������֣���Ϊ��ʱnoteHitDeque��Ϊ�գ���holdingNoteΪnullptr����Ȼ�������noteHitDeque->front()
	if (noteHitDeque->empty() || holdingNote != noteHitDeque->front())
	{
		holdingNote = nullptr;
		return HitType::AIR;
	}

	//releaseֻ��Ҫ�ж������Ľ�β
	//�ж��ĸ߶�Ϊactor���ڸ߶ȵ����Ͻǣ����������Ͻ����꼴��
	const double judgeY = noteHitDeque->front()->GetY();

	//ֱ�ӴӶ����Ƴ���һ��������
	noteHitDeque->pop_front();

	//holdingNote�ÿգ�����ǰ����ж���������������»ᱻrelease
	holdingNote = nullptr;

	//������������⿪ʼ�ж�
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
		//ע��release�ڷ�Χ����ĵĻ���miss
		return HitType::MISS;
	}
}

const UINT Track::Shift(double shift)
{
	for (const auto& note : *noteDeque)
	{
		note->Shift(shift);
	}

	//ȫ������λ�ƺ󣬵ݹ��жϵ�һ��δhit��note�Ƿ�Խ�����ж���
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

	//δ�����ж�
	return 0;
}


Track::~Track()
{
	Clear();

	delete noteDeque;
	delete noteHitDeque;
}