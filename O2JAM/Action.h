#pragma once

#include <vector>
#include <list>
#include <initializer_list>

#include "CommonType.h"


class Action
{
private:
	ActionPieceList* const actionPieceList;

	TimestampType totalDelay;
private:
	//功能：给自身添加动作片段
	//参数：动作片段。delay为0的动作片段会被忽略。
	UINT AddActionPiece(const ActionPiece& actionPiece) const;
public:
	explicit Action(const ActionPieceInitList& actionPieceInitList);
	explicit Action(ActionPieceList* const actionPieceList);


	//功能：获取当前时间的动作片段
	//参数：time指动作开始后经过的时间
	const ActionPiece& GetActionPieceByTime(TimestampType time) const;

	const TimestampType GetTotalDelay() const;

	virtual ~Action();
};
typedef std::vector<const Action*> Actions;