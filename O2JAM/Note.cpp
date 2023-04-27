#include "Note.h"

#include "BitmapModel.h"

Note::Note(const NoteColorType& noteColorType, const Position& position, const UINT& delay, const UINT& height)
	:Actor(noteColorType == NoteColorType::WHITE ? WHITE_NOTE_PATH : (noteColorType == NoteColorType::BLUE ? BLUE_NOTE_PATH : YELLOW_NOTE_PATH), position, NOTE_Z, SIZE(0, height))
	, delay(delay),isHolding(false)
{
	PlayAction(TEXT("stand"));
}

const bool Note::IsDelay() const
{
	return delay >= NOTE_DELAY_MIN;
}

const UINT Note::GetDelay() const
{
	return delay;
}

const double Note::GetY() const
{
	return GetArea().sPosition.y;
}

const UINT Note::GetHeight() const
{
	return ((const BitmapModel* const)GetModel())->GetFullSize().height;
}

const UINT Note::Shift(double shift)
{
	MoveTo(POSITION(GetArea().sPosition.x, GetArea().sPosition.y + shift));

	//Œ¥∆Ù”√≈–∂œ
	return 0;
}

const UINT Note::Hold()
{
	isHolding = true;

	//Œ¥∆Ù”√≈–∂œ
	return 0;
}

const bool Note::IsHolding()
{
	return isHolding;
}

Note::~Note()
{
}
