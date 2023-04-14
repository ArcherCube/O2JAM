#pragma once

#include <iterator>

#include "Constant.h"
#include "Path.h"
#include "Key.h"

#include "Actor.h"

class Keyboard : Actor
{
private:
	KeyHashmap* const keyHashmap;
public:
	explicit Keyboard();

	const TimestampType Press(const KeyName& key) const;
	const TimestampType Release(const KeyName& key) const;

	const TimestampType GetLastHoldTime(const KeyName& key) const;

	virtual ~Keyboard();
};


