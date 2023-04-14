#include "Action.h"

#include <iterator>


Action::Action(const ActionPieceInitList& actionPieceInitList) :actionPieceList(new ActionPieceList())
{
	totalDelay = 0;

	//�������Ƭ���б�Ϊ�գ���ö�������ʾ�κ�����
	if (!actionPieceInitList.size())
	{
		AddActionPiece(ACTION_PIECE(0, 0, 0, 0, 1));
		return;
	}

	for (const auto& actionPiece : actionPieceInitList)
	{
		AddActionPiece(actionPiece);
		totalDelay += actionPiece.delay;
	}
}

Action::Action(ActionPieceList* const actionPieceList) :actionPieceList(actionPieceList)
{
	totalDelay = 0;

	//�������Ƭ���б�Ϊ�գ���ö�������ʾ�κ�����
	if (!actionPieceList->size())
	{
		AddActionPiece(ACTION_PIECE(0, 0, 0, 0, 1));
		return;
	}

	//����totalDelay
	for (const auto& actionPiece : *actionPieceList)
	{
		totalDelay += actionPiece.delay;
	}
}

UINT Action::AddActionPiece(const ActionPiece& actionPiece) const
{
	if (actionPiece.delay == 0) return 0; //ʱ��������Ϊ0

	actionPieceList->push_back(actionPiece);

	//δ�����ж�
	return 0;
}

const ActionPiece& Action::GetActionPieceByTime(TimestampType time) const
{
	time %= totalDelay;
	for (const auto& actionPiece : *actionPieceList)
	{
		if (time >= actionPiece.delay) time -= actionPiece.delay; //��ʱ����ڵ��ڵ�ǰ����Ƭ�ε�ʱ��ʱ����ǰ����Ƭ���ѽ�����������һ������Ƭ��
		else return actionPiece;
	}

	//����ߵ���һ��˵���㷨������
	return ActionPiece();
}

const TimestampType Action::GetTotalDelay() const
{
	return totalDelay;
}

Action::~Action()
{
	actionPieceList->clear();
	delete actionPieceList;
}
