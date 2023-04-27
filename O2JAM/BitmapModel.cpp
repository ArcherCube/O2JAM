#include "BitmapModel.h"

#include <iterator>

#include "FileUtil.h"
#include "CentralTimer.h"
#include "Path.h"

#include "Message.h"

BitmapModel::BitmapModel(const String& modelFilename, const Size& fullSize, const DWORD& renderType)
	: fullSize(fullSize), renderType(renderType), actionChangeTime(CentralTimer::GetTimeStamp()), actionMap(new ActionMap())
	, Model(LoadImage(nullptr, (Path::modelPath + modelFilename + TEXT(".bmp")).c_str(), IMAGE_BITMAP, (int)fullSize.width, (int)fullSize.height, LR_LOADFROMFILE), ModelType::BITMAP)
{
	this->LoadSize(modelFilename);
	this->LoadAction(modelFilename);

	//默认切换到默认动作
	ChangeCurrentAction(TEXT("default"));
}

const UINT BitmapModel::LoadSize(const String& modelFilename)
{
	BITMAP bmp;

	//若传入的fullSize使用默认处理，则根据位图句柄来取得位图的真实尺寸；否则，按传入的数值处理
	GetObject(handle, sizeof(BITMAP), (LPBYTE)&bmp);
	//参数中的fullSize为0时表示取位图的实际大小。所以在位图加载后通过句柄得到的尺寸数据才是其加载到内存中的实际大小
	this->fullSize.width = (UINT)bmp.bmWidth;
	this->fullSize.height = (UINT)bmp.bmHeight;

	//通过再次按默认宽高加载位图、使用BITMAP来获取其源尺寸，并立刻销毁这个加载得到的句柄
	HANDLE tmp = LoadImage(nullptr, (Path::modelPath + modelFilename + TEXT(".bmp")).c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	GetObject(tmp, sizeof(BITMAP), (LPBYTE)&bmp);
	this->sourceSize.width = (UINT)bmp.bmWidth;
	this->sourceSize.height = (UINT)bmp.bmHeight;
	DeleteObject(tmp);

	return SUCCESS;
}

const UINT BitmapModel::LoadAction(const String& modelFilename)
{
	ActionPieceList* currentActionPieceList = new ActionPieceList();
	String currentActionName = TEXT("");

	//添加默认动作。其表示位图模型的整体，单帧。
	this->actionMap->insert({ TEXT("default"), new Action({ACTION_PIECE(0, 0,this->sourceSize.width,this->sourceSize.height, 1)}) });

	//根据sourcePath加载动作文件
	const auto& result = FileUtil::Read(Path::modelPath + modelFilename + TEXT(".txt"), [&](const String& line)
		{
			//读到非活动行，处理“是否存在上一个动作加载完毕”后跳过本行
			if (line.empty() || line[0] == ':')
			{
				//记录当前动作
				if (!currentActionName.empty() && currentActionPieceList->size())
				{
					//加载一个动作后，把动作片段队列对象的指针用于初始化一个动作
					this->actionMap->insert({ currentActionName,new Action(*currentActionPieceList) });

					//随后创建一个新的动作片段队列对象以进行之后的加载
					currentActionPieceList = new ActionPieceList();
				}
				//纪录下一个动作名
				if (line[0] == ':')
				{
					currentActionName = line.substr(1);
				}
			}
			else
			{
				//记录动作片段
				double x, y;
				UINT width, height, delay;
				//以字符串流的形式将当前行的数据传到5个变量中；若数据不完整或异常（例如delay为0），则忽略当前动作片段
				if (Istringstream(line) >> x >> y >> width >> height >> delay && delay > 0)
				{
					currentActionPieceList->push_back(ACTION_PIECE(x, y, width, height, delay));
				}
			}
		}
	);
	delete currentActionPieceList;

	return result;
}


const UINT BitmapModel::ChangeCurrentAction(const String& actionName)
{
	const auto& iter = this->actionMap->find(actionName);
	if (iter == this->actionMap->cend()) return FAIL;

	this->currentAction = iter->second;
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
	for (const auto& pair : *(this->actionMap))
	{
		delete pair.second;
	}
	actionMap->clear();
	delete actionMap;
}
