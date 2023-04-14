#pragma once

#include <windows.h>
#include <initializer_list>
#include <list>
#include <vector>
#include "Constant.h"
#include "CommonType.h"
#include "Model.h"

//���ڴ˴�����engine��������Ϊ��action��bitmapModel�ǵ���ģ���bitmapModel��ʵ����Ҫ�õ�action����action��ʵ����ȫ��bitmapModel�޹�
#include "Action.h"


class BitmapModel :public Model
{
private:
	//�����б�
	Actions* const actions;

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
public:
	explicit BitmapModel(const String& modelFilename, const Size& fullSize = SIZE(0, 0), const DWORD& renderType = SRCCOPY);

	const DWORD GetRenderType() const;

	const Size& GetFullSize() const;

	const Size& GetSourceSize() const;

	const Action* const GetCurrentAction() const;

	const ActionPiece& GetCurrentActionPiece() const;

	const TimestampType GetLastChangeTime() const = delete;

	const UINT ChangeCurrentAction(const UINT& no);

	virtual ~BitmapModel();
};

