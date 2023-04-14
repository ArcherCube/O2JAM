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
		//�����ǻ�У������Ƿ������һ������������ϡ�����������
		if (lineOfFile.empty() || lineOfFile[0] == ':')
		{
			//����������Ԫ��������м��ض���������ֱ�Ӻ��Ե�ǰ��
			if (actionPieceList->size())
			{
				//����һ�������󣬰Ѷ���Ƭ�ζ��ж����ָ�����ڳ�ʼ��һ������
				actions->push_back(new Action(actionPieceList));

				//��󴴽�һ���µĶ���Ƭ�ζ��ж����Խ���֮��ļ���
				actionPieceList = new ActionPieceList();
			}
			continue;
		}
		//��Ϊ��У����������
		else
		{
			//���ַ���������ʽ����ǰ�е����ݴ���5�������У������ݲ��������쳣������delayΪ0��������Ե�ǰ����Ƭ��
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
	//����������һ�к󣬿��ܴ�����һ�������������
	if (actionPieceList->size())
	{
		actions->push_back(new Action(actionPieceList));
	}
	//�������ڣ����ʾ��ǰΪ��󴴽���һ������Ķ���Ƭ�ζ��У�delete��
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
		//���Կ���
		if (lineOfFile.empty()) continue;
		//�ǻ�������ȡ����
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
		//�Ѽ�¼�˰���ʱ����hashmap��ȡ����
		const auto& delayHashmap = melody.delayHashmap;
		//Ѱ�����Ӧ�İ���ʱ����¼Ԫ�صĵ�����
		const auto& iterA = delayHashmap.find(KeyName::A);
		const auto& iterS = delayHashmap.find(KeyName::S);
		const auto& iterD = delayHashmap.find(KeyName::D);
		const auto& iterSPACE = delayHashmap.find(KeyName::SPACE);
		const auto& iterJ = delayHashmap.find(KeyName::J);
		const auto& iterK = delayHashmap.find(KeyName::K);
		const auto& iterL = delayHashmap.find(KeyName::L);

		//����ʽд��һ��
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
	ofn.lpstrTitle = TEXT("ѡ�������ļ�\0");
	ofn.lpstrFilter = TEXT("���������ý��(*.mp3, *.wav)\0*.mp3;*.wav\0\0");
	ofn.lpstrInitialDir = TEXT(".\\src\\music\\\0");
	ofn.lpstrFile = fileName;
	ofn.nMaxFile = MAX_PATH;
	ofn.nFilterIndex = 0;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_EXPLORER;

	//���APIִ�к󣬻ᴴ��һ���ļ�ѡ���
	//�û���ѡ��һ���ļ��󣬡���ǰ���̵ĵ�ǰĿ¼�����л����ļ����ڵ�Ŀ¼������ԭ�������·��û��ʹ��
	//������ִ��֮ǰ�ȱ��桰��ǰ���̵ĵ�ǰĿ¼������ִ�к�Ҳ��ʱ�û�ѡ���ļ��󣬻ָ���
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
