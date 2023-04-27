#pragma once


#include "Actor.h"
#include "Path.h"
#include "Constant.h"

#define MELODY_FILE_SUFFIX TEXT(".o2jam")

#define MELODY_PIECE_FULL(timestamp,delayA,delayS,delayD,delaySPACE,delayJ,delayK,delayL) {timestamp,{{KeyName::A,delayA},{KeyName::S,delayS},{KeyName::D,delayD},{KeyName::SPACE,delaySPACE},{KeyName::J,delayJ},{KeyName::K,delayK},{KeyName::L,delayL}}}

typedef std::unordered_map<KeyName, TimestampType> DelayHashmap;
struct MelodyPiece
{
	//对于玩家，这个时间戳是按键按下的时间
	TimestampType timestamp;
	DelayHashmap delayHashmap;
};
typedef std::list<MelodyPiece> MelodyPieceList;
typedef MelodyPieceList::iterator MelodyPieceListIter;


class Melody
{
private:
	bool loaded;
	MelodyPieceList* const melodyPieceList;
public:
	explicit Melody(MelodyPieceList* const melodyPieceList);
	explicit Melody(const String& musicName);

	String& ToText() const;

	bool IsLoaded() const;

	const MelodyPieceList* getMelodyPieceList() const;
};

