#pragma once

#include <windows.h>
#include <iostream>
#include <functional>
#include <list>
#include "CommonType.h"
#include "Constant.h"

#include "Action.h"
#include "BitmapModel.h"


class FileUtil
{
private:
	static HWND hWnd;
public:
	//FileUtil(HWND hWnd);
	static UINT Init(const HWND& hWnd);

	static UINT LoadActionsFromFile(const String& modelFilename, Actions* const actions);

	static UINT LoadMelodyListFromFile(const String& musicFilename, MelodyList* const melodyList);

	static UINT CreateMelodyFile(const String& musicFilename, const MelodyList* const melodyList);

	static UINT SelectFile(const LPTSTR& path);

	static bool FileIsSuffix(const String& path, const String&& suffix);

	static UINT GetFilenameFromPath(const TCHAR* const path, String& filename);

	//~FileUtil();
};

