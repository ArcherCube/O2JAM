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
	//���ܣ���������Ӷ���Ƭ��
	//����������Ƭ�Ρ�delayΪ0�Ķ���Ƭ�λᱻ���ԡ�
	UINT AddActionPiece(const ActionPiece& actionPiece) const;
public:
	explicit Action(const ActionPieceInitList& actionPieceInitList);
	explicit Action(ActionPieceList* const actionPieceList);


	//���ܣ���ȡ��ǰʱ��Ķ���Ƭ��
	//������timeָ������ʼ�󾭹���ʱ��
	const ActionPiece& GetActionPieceByTime(TimestampType time) const;

	const TimestampType GetTotalDelay() const;

	virtual ~Action();
};
typedef std::vector<const Action*> Actions;