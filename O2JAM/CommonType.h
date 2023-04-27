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


enum class HitType :UINT
{
	AIR,//·¶Î§ÄÚÃ»ÓÐÒô·û
	MISS, //Â©µô
	BAD,
	GOOD,
	COOL
};