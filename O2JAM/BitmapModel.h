#pragma once

#include <windows.h>
#include <initializer_list>
#include <list>
#include <vector>
#include "Constant.h"
#include "CommonType.h"
#include "Model.h"
#include "Action.h"


class BitmapModel :public Model
{
private:
	//�����б�
	ActionMap* const actionMap;

	//��ǰ������Ϊָ������ָ��
	const Action* currentAction;

	//λͼ���ص��ڴ��������С
	Size fullSize;

	//λͼ�ļ���ԭʼ�ߴ�
	Size sourceSize;

	//��Ⱦ����
	DWORD renderType;

	//�ϴ��л�������ʱ��
	TimestampType actionChangeTime;
private:
	const UINT LoadSize(const String& modelFilename);
	const UINT LoadAction(const String& modelFilename);
public:
	explicit BitmapModel(const String& modelFilename, const Size& fullSize = SIZE(0, 0), const DWORD& renderType = SRCCOPY);

	const DWORD GetRenderType() const;

	const Size& GetFullSize() const;

	const Size& GetSourceSize() const;

	const Action* const GetCurrentAction() const;

	const ActionPiece& GetCurrentActionPiece() const;

	const TimestampType GetLastChangeTime() const = delete;

	const UINT ChangeCurrentAction(const String& actionName);

	virtual ~BitmapModel();
};

