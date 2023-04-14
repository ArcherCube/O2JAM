#pragma once

#include "CommonType.h"
#include "Constant.h"
#include "Track.h"


class Railway
{
private:
	TimestampType currentTimestamp;

	TrackHashmap* const trackHashmap;
public:
	explicit Railway();

	const UINT LoadMelodyList(const MelodyList* const melodyList) const;

	const HitType PressHit(const KeyName& key) const;
	const HitType ReleaseHit(const KeyName& key) const;

	const UINT ScrollTo(const TimestampType& timestamp);

	const UINT Clear();

	virtual ~Railway();
};
