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

	//添加默认动作。其表示位图模型的整体，单帧。
	//若传入的fullSize使用默认处理，则根据位图句柄来取得位图的真实尺寸；否则，按传入的数值处理
	GetObject(handle, sizeof(BITMAP), (LPBYTE)&bmp);
	actions->push_back(new Action({ ACTION_PIECE(0, 0,(UINT)bmp.bmWidth,(UINT)bmp.bmHeight, 1) }));

	//参数中的fullSize为0时表示取位图的实际大小。所以在位图加载后通过句柄得到的尺寸数据才是其加载到内存中的实际大小
	this->fullSize.width = (UINT)bmp.bmWidth;
	this->fullSize.height = (UINT)bmp.bmHeight;

	//通过再次按默认宽高加载位图、使用BITMAP来获取其源尺寸，并立刻销毁这个加载得到的句柄
	HANDLE tmp = LoadImage(nullptr, (MODEL_ROOT_PATH + modelFilename + TEXT(".bmp")).c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	GetObject(tmp, sizeof(BITMAP), (LPBYTE)&bmp);
	this->sourceSize.width = (UINT)bmp.bmWidth;
	this->sourceSize.height = (UINT)bmp.bmHeight;
	DeleteObject(tmp);

	//根据sourcePath加载动作文件
	FileUtil::LoadActionsFromFile(modelFilename, actions);

	//默认切换到默认动作
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
