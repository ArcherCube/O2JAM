#pragma once

#include <Windows.h>
#include <unordered_map>

#include "Actor.h"
#include "CommonType.h"
#include "Constant.h"

class Key : public Actor
{
public:
	enum class KeyType :UINT
	{
		WHITE_L,
		WHITE_R,
		BLUE,
		YELLOW
	};
private:
	const KeyType keyType;
	bool isPressed;

	TimestampType lastPressTime,lastReleaseTime;
public:
	explicit Key(const KeyType&, const Position&);

	const TimestampType Press();
	const TimestampType Release();

	const TimestampType GetLastHoldTime() const;
};

typedef std::unordered_map<KeyName, Key* const> KeyHashmap;
typedef KeyHashmap::const_iterator KeyHashmapCiter;