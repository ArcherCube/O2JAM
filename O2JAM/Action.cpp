#include "Action.h"

#include <iterator>


Action::Action(const ActionPieceList& actionPieceList) :actionPieceList(new ActionPieceList())
{
	this->delay = 0;

	//如果动作片段列表为空，则该动作不显示任何内容
	if (!actionPieceList.size())
	{
		AddActionPiece(ACTION_PIECE(0, 0, 0, 0, 1));
		return;
	}

	for (const auto& actionPiece : actionPieceList)
	{
		AddActionPiece(actionPiece);
		this->delay += actionPiece.delay;
	}
}

UINT Action::AddActionPiece(const ActionPiece& actionPiece) const
{
	if (actionPiece.delay == 0) return 0; //时长不允许为0

	actionPieceList->push_back(actionPiece);

	//未启用判断
	return 0;
}

const ActionPiece& Action::GetActionPieceByTime(TimestampType time) const
{
	time %= this->delay;
	for (const auto& actionPiece : *actionPieceList)
	{
		if (time >= actionPiece.delay) time -= actionPiece.delay; //当时间大于等于当前动作片段的时长时，当前动作片段已结束，换到下一个动作片段
		else return actionPiece;
	}

	//如果走到这一步说明算法有问题
	return ActionPiece();
}

const TimestampType Action::GetDelay() const
{
	return delay;
}

Action::~Action()
{
	actionPieceList->clear();
	delete actionPieceList;
}
