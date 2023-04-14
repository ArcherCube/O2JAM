#include "FileUtil.h"
#include "CommDlg.h"

#include "Path.h"

using std::getline;

HWND FileUtil::hWnd = nullptr;

UINT FileUtil::Init(const HWND& _hWnd)
{
	hWnd = _hWnd;

	return 0;
}

UINT FileUtil::LoadActionsFromFile(const String& modelFilename, Actions* const actions)
{
	Ifstream ifStream;
	ifStream.open(MODEL_ROOT_PATH + modelFilename + TEXT(".txt"));
	if (!ifStream.is_open()) return FAIL;

	String lineOfFile;
	double x, y;
	UINT width, height, delay;
	ActionPieceList* actionPieceList = new ActionPieceList();
	while (getline(ifStream, lineOfFile))
	{
		//读到非活动行，处理“是否存在上一个动作加载完毕”后跳过本行
		if (lineOfFile.empty() || lineOfFile[0] == ':')
		{
			//若队列中有元素则代表有加载动作；否则直接忽略当前行
			if (actionPieceList->size())
			{
				//加载一个动作后，把动作片段队列对象的指针用于初始化一个动作
				actions->push_back(new Action(actionPieceList));

				//随后创建一个新的动作片段队列对象以进行之后的加载
				actionPieceList = new ActionPieceList();
			}
			continue;
		}
		//若为活动行，则加载数据
		else
		{
			//以字符串流的形式将当前行的数据传到5个变量中；若数据不完整或异常（例如delay为0），则忽略当前动作片段
			if (Istringstream(lineOfFile) >> x >> y >> width >> height >> delay && delay > 0)
			{
				actionPieceList->push_back(ACTION_PIECE(x, y, width, height, delay));
			}
			else
			{
				continue;
			}
		}
	}
	//当读不到下一行后，可能存在上一个动作加载完毕
	if (actionPieceList->size())
	{
		actions->push_back(new Action(actionPieceList));
	}
	//若不存在，则表示当前为最后创建的一个多余的动作片段队列，delete掉
	else
	{
		delete actionPieceList;
	}

	ifStream.close();

	return SUCCESS;
}

UINT FileUtil::LoadMelodyListFromFile(const String& musicFilename, MelodyList* const melodyList)
{
	Ifstream ifStream;
	ifStream.open(MUSIC_ROOT_PATH + musicFilename + TEXT(".txt"));
	if (!ifStream.is_open()) return FAIL;

	String lineOfFile;
	UINT delayA, delayS, delayD, delaySPACE, delayJ, delayK, delayL;
	TimestampType timestamp;
	while (getline(ifStream, lineOfFile))
	{
		//忽略空行
		if (lineOfFile.empty()) continue;
		//非活动行则加载取数据
		else
		{
			if (Istringstream(lineOfFile) >> timestamp >> delayA >> delayS >> delayD >> delaySPACE >> delayJ >> delayK >> delayL)
			{
				melodyList->push_back(MELODY_FULL(timestamp, delayA, delayS, delayD, delaySPACE, delayJ, delayK, delayL));
			}
		}
	}

	ifStream.close();

	return SUCCESS;
}

UINT FileUtil::CreateMelodyFile(const String& musicFilename, const MelodyList* const melodyList)
{
	Ofstream ofStream;
	ofStream.open(MUSIC_ROOT_PATH + musicFilename + TEXT(".txt"), std::ios::app);
	if (!ofStream.is_open()) return FAIL;

	for (const auto& melody : *melodyList)
	{
		//把记录了按键时长的hashmap提取出来
		const auto& delayHashmap = melody.delayHashmap;
		//寻找其对应的按键时长记录元素的迭代器
		const auto& iterA = delayHashmap.find(KeyName::A);
		const auto& iterS = delayHashmap.find(KeyName::S);
		const auto& iterD = delayHashmap.find(KeyName::D);
		const auto& iterSPACE = delayHashmap.find(KeyName::SPACE);
		const auto& iterJ = delayHashmap.find(KeyName::J);
		const auto& iterK = delayHashmap.find(KeyName::K);
		const auto& iterL = delayHashmap.find(KeyName::L);

		//按格式写入一行
		ofStream << melody.timestamp << " "
			<< (iterA == delayHashmap.cend() ? 0 : iterA->second) << " "
			<< (iterS == delayHashmap.cend() ? 0 : iterS->second) << " "
			<< (iterD == delayHashmap.cend() ? 0 : iterD->second) << " "
			<< (iterSPACE == delayHashmap.cend() ? 0 : iterSPACE->second) << " "
			<< (iterJ == delayHashmap.cend() ? 0 : iterJ->second) << " "
			<< (iterK == delayHashmap.cend() ? 0 : iterK->second) << " "
			<< (iterL == delayHashmap.cend() ? 0 : iterL->second) << std::endl;
	}
	ofStream.close();

	return SUCCESS;
}

UINT FileUtil::SelectFile(const LPTSTR& fileName)
{
	OPENFILENAME ofn = { 0 };

	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = nullptr;
	ofn.lpstrTitle = TEXT("选择音乐文件\0");
	ofn.lpstrFilter = TEXT("允许的音乐媒体(*.mp3, *.wav)\0*.mp3;*.wav\0\0");
	ofn.lpstrInitialDir = TEXT(".\\src\\music\\\0");
	ofn.lpstrFile = fileName;
	ofn.nMaxFile = MAX_PATH;
	ofn.nFilterIndex = 0;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_EXPLORER;

	//这个API执行后，会创建一个文件选择框；
	//用户在选择一个文件后，“当前进程的当前目录”会切换到文件所在的目录，导致原本的相对路径没法使用
	//所以在执行之前先保存“当前进程的当前目录”，在执行后，也就时用户选择文件后，恢复。
	wchar_t currentDirectory[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, currentDirectory);

	auto result = GetOpenFileName(&ofn); 
	
	SetCurrentDirectory(currentDirectory);

	return result;
}

bool FileUtil::FileIsSuffix(const String& path, const String&& suffix)
{
	size_t pos = path.find('.');
	if (pos == String::npos) return false;
	return path.substr(pos + 1, path.size() - (pos + 1)).compare(suffix) == 0;
}

UINT FileUtil::GetFilenameFromPath(const TCHAR* const path, String& filename)
{
	String* tmp = new String(path);
	size_t posOfSeparator = tmp->rfind('\\');
	size_t posOfPoint = tmp->rfind('.');
	filename = tmp->substr(posOfSeparator + 1, posOfPoint - (posOfSeparator + 1));
	delete tmp;

	return SUCCESS;
}
