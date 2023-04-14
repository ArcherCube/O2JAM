#pragma once

#include <windows.h>
#include <string>
#include <initializer_list>
#include <list>
#include <set>
#include <vector>
#include <unordered_map>
#include <iterator>

#include <fstream>
#include <sstream>

#ifdef UNICODE
typedef std::wstring String;
typedef std::wifstream Ifstream;
typedef std::wistringstream Istringstream;
typedef std::wofstream Ofstream;
typedef std::wostringstream Ostringstream;
#define TO_STRING(obj) std::to_wstring(obj)
#else
typedef std::string String;
typedef std::ifstream Ifstream;
typedef std::istringstream Istringstream;
typedef std::ofstream Ofstream;
typedef std::ostringstream Ostringstream;
#define TO_STRING(obj) std::to_string(obj);
#endif // DEBUG


#define POSITION(X,Y) {X,Y}
struct Position
{
	//static const Position ZERO;

	double x = 0;
	double y = 0;

	bool operator== (const Position& position) const
	{
		return x == position.x && y == position.y;
	}
};
//const Position Position::ZERO = POSITION(0, 0);


#define SIZE(width,height) {width,height}
struct Size
{
	//static const Size ZERO;

	UINT width = 0;
	UINT height = 0;

	bool operator== (const Size& size) const
	{
		return width == size.width && height == size.height;
	}
};
//const Size Size::ZERO = SIZE(0, 0);


#define AREA(X1,Y1,X2,Y2) {{X1,Y1},{X2,Y2}}
struct Area
{
	//static const Area ZERO;

	Position sPosition = POSITION(0, 0);
	Position ePosition = POSITION(0, 0);

	bool operator== (const Area& area) const
	{
		return sPosition == area.sPosition && ePosition == area.ePosition;
	}
};
//const Area Area::ZERO = AREA(0, 0, 0, 0);


#define ACTION_PIECE(x,y,width,height,delay) {{x,y},{width,height},delay}
struct ActionPiece
{
	Position sourcePosition;
	Size size;
	UINT delay;
};
typedef std::initializer_list<const ActionPiece> ActionPieceInitList;
typedef std::list<ActionPiece> ActionPieceList;


typedef DWORD TimestampType;


enum class KeyName :WPARAM
{
	A = 'A',
	S = 'S',
	D = 'D',
	SPACE = VK_SPACE,
	J = 'J',
	K = 'K',
	L = 'L'
};


#define MELODY_SINGLE(timestamp,keyName,delay) {timestamp,{{keyName,delay}}}
#define MELODY_FULL(timestamp,delayA,delayS,delayD,delaySPACE,delayJ,delayK,delayL) {timestamp,{{KeyName::A,delayA},{KeyName::S,delayS},{KeyName::D,delayD},{KeyName::SPACE,delaySPACE},{KeyName::J,delayJ},{KeyName::K,delayK},{KeyName::L,delayL}}}
struct Melody
{
private:
	typedef std::unordered_map<KeyName, TimestampType> DelayHashmap;
public:
	//对于玩家，这个时间戳是按键按下的时间
	TimestampType timestamp;
	DelayHashmap delayHashmap;
};
//struct MelodyNOLess
//{
//	bool operator()(const Melody&  _Left, const Melody& _Right) const
//	{
//		return _Left.timestamp <= _Right.timestamp;
//	}
//};
typedef std::list<Melody> MelodyList;
typedef MelodyList::iterator MelodyListIter;


enum class HitType :UINT
{
	AIR,//范围内没有音符
	MISS, //漏掉
	BAD,
	GOOD,
	COOL
};