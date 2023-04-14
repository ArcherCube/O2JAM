#pragma once

#include <Windows.h>
#include "CommonType.h"
#include "Constant.h"

class Recorder
{
private:
	static MelodyList* const melodyList;
private:
	static UINT AlignHead();
	static UINT AlignTail();
	static UINT Combine();
public:
	static UINT StartRecord();

	static UINT Record(const TimestampType& releaseTimestamp,const KeyName& key, const TimestampType& holdTime);

	static const MelodyList* const FinishRecord();

	static UINT Clean();
};

