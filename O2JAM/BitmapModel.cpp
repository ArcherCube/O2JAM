#include "BitmapModel.h"

#include <iterator>

#include "FileUtil.h"
#include "CentralTimer.h"
#include "Path.h"

#include "Message.h"

BitmapModel::BitmapModel(const String& modelFilename, const Size& fullSize, const DWORD& renderType)
	: fullSize(fullSize), renderType(renderType), actionChangeTime(CentralTimer::GetTimeStamp()), actions(new Actions())
	, Model(LoadImage(nullptr, (MODEL_ROOT_PATH + modelFilename + TEXT(".bmp")).c_str(), IMAGE_BITMAP, (int)fullSize.width, (int)fullSize.height, LR_LOADFROMFILE), ModelType::BITMAP)
{
	BITMAP bmp;

	//���Ĭ�϶��������ʾλͼģ�͵����壬��֡��
	//�������fullSizeʹ��Ĭ�ϴ��������λͼ�����ȡ��λͼ����ʵ�ߴ磻���򣬰��������ֵ����
	GetObject(handle, sizeof(BITMAP), (LPBYTE)&bmp);
	actions->push_back(new Action({ ACTION_PIECE(0, 0,(UINT)bmp.bmWidth,(UINT)bmp.bmHeight, 1) }));

	//�����е�fullSizeΪ0ʱ��ʾȡλͼ��ʵ�ʴ�С��������λͼ���غ�ͨ������õ��ĳߴ����ݲ�������ص��ڴ��е�ʵ�ʴ�С
	this->fullSize.width = (UINT)bmp.bmWidth;
	this->fullSize.height = (UINT)bmp.bmHeight;

	//ͨ���ٴΰ�Ĭ�Ͽ�߼���λͼ��ʹ��BITMAP����ȡ��Դ�ߴ磬����������������صõ��ľ��
	HANDLE tmp = LoadImage(nullptr, (MODEL_ROOT_PATH + modelFilename + TEXT(".bmp")).c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	GetObject(tmp, sizeof(BITMAP), (LPBYTE)&bmp);
	this->sourceSize.width = (UINT)bmp.bmWidth;
	this->sourceSize.height = (UINT)bmp.bmHeight;
	DeleteObject(tmp);

	//����sourcePath���ض����ļ�
	FileUtil::LoadActionsFromFile(modelFilename, actions);

	//Ĭ���л���Ĭ�϶���
	ChangeCurrentAction(0);
}

const UINT BitmapModel::ChangeCurrentAction(const UINT& no)
{
	if (no >= actions->size()) return FAIL;

	currentAction = actions->at(no);
	actionChangeTime = CentralTimer::GetTimeStamp();

	return SUCCESS;
}

const Action* const BitmapModel::GetCurrentAction() const
{
	return currentAction;
}

const ActionPiece& BitmapModel::GetCurrentActionPiece() const
{
	return currentAction ? currentAction->GetActionPieceByTime(CentralTimer::GetTimeStamp() - actionChangeTime) : ActionPiece();
}

const Size& BitmapModel::GetFullSize() const
{
	return fullSize;
}

const Size& BitmapModel::GetSourceSize() const
{
	return sourceSize;
}

const DWORD BitmapModel::GetRenderType() const
{
	return renderType;
}

BitmapModel::~BitmapModel()
{
	for (const auto& action : *actions)
	{
		delete action;
	}
	actions->clear();
	delete actions;
}
