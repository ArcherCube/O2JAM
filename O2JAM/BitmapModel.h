#pragma once

#include <windows.h>
#include <initializer_list>
#include <list>
#include <vector>
#include "Constant.h"
#include "CommonType.h"
#include "Model.h"

//能在此处包含engine级别，是因为从action到bitmapModel是单向的，即bitmapModel的实现需要用到action，而action的实现完全与bitmapModel无关
#include "Action.h"


class BitmapModel :public Model
{
private:
	//动作列表
	Actions* const actions;

	//当前动作，为指向常量的指针
	const Action* currentAction;

	//位图加载到内存的完整大小
	Size fullSize;

	//位图文件的原始尺寸
	Size sourceSize;

	//渲染类型
	DWORD renderType;

	//上次切换动作的时间
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

