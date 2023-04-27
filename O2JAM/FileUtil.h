#pragma once

#include <windows.h>
#include <iostream>
#include <functional>
#include <vector>
#include <functional>
#include "CommonType.h"
#include "Constant.h"

#include "Action.h"
#include "BitmapModel.h"


class FileUtil
{
public:
	static UINT Create(const String& content, const String& path);

	static UINT Read(const String& path, const std::function<void(const String&)>& exc);

	static UINT Select(const LPTSTR& path, const String& directory, const String& title, const std::vector<String>& accept);

	static bool IsSuffix(const String& path, const String& suffix);

	static bool IsExist(const String& path);

	static const String GetFilename(const LPCTSTR& path);
};

