#pragma once

#include <vector>
#include <list>
#include <unordered_map>

#include "CommonType.h"


#define ACTION_PIECE(x,y,width,height,delay) {{x,y},{width,height},delay}

struct ActionPiece
{
	Position sourcePosition;
	Size size;
	UINT delay;
};
typedef std::list<ActionPiece> ActionPieceList;


class Action
{
private:
	ActionPieceList* const actionPieceList;

	TimestampType delay;
private:
	//功能：给自身添加动作片段
	//参数：动作片段。delay为0的动作片段会被忽略。
	UINT AddActionPiece(const ActionPiece& actionPiece) const;
public:
	explicit Action(const ActionPieceList& actionPieceList);


	//功能：获取当前时间的动作片段
	//参数：time指动作开始后经过的时间
	const ActionPiece& GetActionPieceByTime(TimestampType time) const;

	const TimestampType GetDelay() const;

	virtual ~Action();
};
typedef std::unordered_map<String, const Action*> ActionMap;