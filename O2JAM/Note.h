#pragma once

#include <deque>
#include "Actor.h"
#include "Path.h"
#include "CommonType.h"
#include "Constant.h"

class Note :public Actor
{
private:
	enum class NoteAction :UINT
	{
		DEFAULT = 0,
		STAND = 1
	};
public:
	enum class NoteColorType :UINT
	{
		WHITE,
		BLUE,
		YELLOW
	};
private:
	const UINT delay;
	bool isHolding;
public:
	explicit Note(const NoteColorType& noteColorType, const Position& position, const UINT& delay, const UINT& height);

	const bool IsDelay() const;

	const UINT GetDelay() const;

	const double GetY() const;

	const UINT GetHeight() const;

	const UINT Shift(double shift);

	const UINT Hold();

	const bool IsHolding();

	virtual ~Note();
};
typedef std::deque<Note*> NoteDeque;