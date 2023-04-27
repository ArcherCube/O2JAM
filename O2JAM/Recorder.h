#pragma once

#include <Windows.h>
#include "CommonType.h"
#include "Constant.h"
#include "Melody.h"

#define MELODY_PIECE_SINGLE(timestamp,keyName,delay) {timestamp,{{keyName,delay}}}

class Recorder
{
private:
	static MelodyPieceList* const melodyPieceList;
private:
	static UINT AlignHead();
	static UINT AlignTail();
	static UINT Combine();
public:
	static UINT StartRecord();

	static UINT Record(const TimestampType& releaseTimestamp,const KeyName& key, const TimestampType& holdTime);

	static const Melody& FinishRecord();

	static UINT Clean();
};

