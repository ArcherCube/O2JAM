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
	//���ܣ���������Ӷ���Ƭ��
	//����������Ƭ�Ρ�delayΪ0�Ķ���Ƭ�λᱻ���ԡ�
	UINT AddActionPiece(const ActionPiece& actionPiece) const;
public:
	explicit Action(const ActionPieceList& actionPieceList);


	//���ܣ���ȡ��ǰʱ��Ķ���Ƭ��
	//������timeָ������ʼ�󾭹���ʱ��
	const ActionPiece& GetActionPieceByTime(TimestampType time) const;

	const TimestampType GetDelay() const;

	virtual ~Action();
};
typedef std::unordered_map<String, const Action*> ActionMap;