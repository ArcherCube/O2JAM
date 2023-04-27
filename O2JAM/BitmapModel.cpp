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

	//Ĭ���л���Ĭ�϶���
	ChangeCurrentAction(TEXT("default"));
}

const UINT BitmapModel::LoadSize(const String& modelFilename)
{
	BITMAP bmp;

	//�������fullSizeʹ��Ĭ�ϴ��������λͼ�����ȡ��λͼ����ʵ�ߴ磻���򣬰��������ֵ����
	GetObject(handle, sizeof(BITMAP), (LPBYTE)&bmp);
	//�����е�fullSizeΪ0ʱ��ʾȡλͼ��ʵ�ʴ�С��������λͼ���غ�ͨ������õ��ĳߴ����ݲ�������ص��ڴ��е�ʵ�ʴ�С
	this->fullSize.width = (UINT)bmp.bmWidth;
	this->fullSize.height = (UINT)bmp.bmHeight;

	//ͨ���ٴΰ�Ĭ�Ͽ�߼���λͼ��ʹ��BITMAP����ȡ��Դ�ߴ磬����������������صõ��ľ��
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

	//���Ĭ�϶��������ʾλͼģ�͵����壬��֡��
	this->actionMap->insert({ TEXT("default"), new Action({ACTION_PIECE(0, 0,this->sourceSize.width,this->sourceSize.height, 1)}) });

	//����sourcePath���ض����ļ�
	const auto& result = FileUtil::Read(Path::modelPath + modelFilename + TEXT(".txt"), [&](const String& line)
		{
			//�����ǻ�У������Ƿ������һ������������ϡ�����������
			if (line.empty() || line[0] == ':')
			{
				//��¼��ǰ����
				if (!currentActionName.empty() && currentActionPieceList->size())
				{
					//����һ�������󣬰Ѷ���Ƭ�ζ��ж����ָ�����ڳ�ʼ��һ������
					this->actionMap->insert({ currentActionName,new Action(*currentActionPieceList) });

					//��󴴽�һ���µĶ���Ƭ�ζ��ж����Խ���֮��ļ���
					currentActionPieceList = new ActionPieceList();
				}
				//��¼��һ��������
				if (line[0] == ':')
				{
					currentActionName = line.substr(1);
				}
			}
			else
			{
				//��¼����Ƭ��
				double x, y;
				UINT width, height, delay;
				//���ַ���������ʽ����ǰ�е����ݴ���5�������У������ݲ��������쳣������delayΪ0��������Ե�ǰ����Ƭ��
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
