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
	//根据sourcePath加载动作文件
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
	//补充执行一个空行，表示读取结束，方便回调函数处理
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
	//TODO: 这里的动态路径还是没弄好
	//wsprintf(filter, TEXT("允许的类型(%s)\0%s\0\0"), acceptStr.c_str(), acceptStr.c_str());
	//String filterStr = filter;

	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = nullptr;
	ofn.lpstrTitle = title.c_str();
	ofn.lpstrFilter = TEXT("允许的类型(*.*)\0*.*\0\0");
	//ofn.lpstrFilter = filterStr.c_str();
	ofn.lpstrInitialDir = directory.c_str();
	ofn.lpstrFile = path;
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
