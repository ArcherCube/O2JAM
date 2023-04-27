#include "FileUtil.h"
#include "CommDlg.h"
#include "Message.h"
#include "Path.h"

UINT FileUtil::Create(const String& content, const String& fullPath)
{
	Ofstream ofStream;
	ofStream.open(fullPath, std::ios::app);
	if (!ofStream.is_open()) return FAIL;
	ofStream << content;
	ofStream.close();

	return SUCCESS;
}

UINT FileUtil::Read(const String& directory, const std::function<void(const String&)>& exc)
{
	//����sourcePath���ض����ļ�
	Ifstream ifStream;
	ifStream.open(directory);
	if (!ifStream.is_open())
	{
		return FAIL;
	}

	String line;
	while (std::getline(ifStream, line))
	{
		exc(line);
	}
	//����ִ��һ�����У���ʾ��ȡ����������ص���������
	exc(TEXT(""));

	ifStream.close();

	return SUCCESS;
}

UINT FileUtil::Select(const LPTSTR& path, const String& directory, const String& title, const std::vector<String>& accept)
{
	OPENFILENAME ofn = { 0 };

	String acceptStr = TEXT("");
	for (const auto& suffix : accept)
	{
		acceptStr.append(TEXT("*") + suffix + TEXT(";"));
	}
	acceptStr.erase(acceptStr.end() - 1);

	//LPTSTR filter = new TCHAR[1024];
	//TODO: ����Ķ�̬·������ûŪ��
	//wsprintf(filter, TEXT("���������(%s)\0%s\0\0"), acceptStr.c_str(), acceptStr.c_str());
	//String filterStr = filter;

	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = nullptr;
	ofn.lpstrTitle = title.c_str();
	ofn.lpstrFilter = TEXT("���������(*.*)\0*.*\0\0");
	//ofn.lpstrFilter = filterStr.c_str();
	ofn.lpstrInitialDir = directory.c_str();
	ofn.lpstrFile = path;
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

bool FileUtil::IsSuffix(const String& path, const String& suffix)
{
	size_t pos = path.find('.');
	if (pos == String::npos) return false;
	return path.substr(pos + 1, path.size() - (pos + 1)).compare(suffix) == 0;
}

bool FileUtil::IsExist(const String& path)
{
	Ifstream ifstream(path);
	return ifstream.good();
}

const String FileUtil::GetFilename(const LPCTSTR& path)
{
	String tmp = path;
	size_t posOfSeparator = tmp.rfind('\\');
	size_t posOfPoint = tmp.rfind('.');
	const auto result = tmp.substr(posOfSeparator + 1, posOfPoint - (posOfSeparator + 1));

	return result;
}
