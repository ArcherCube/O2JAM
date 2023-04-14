#pragma once

#include <windows.h>
#include "CommonType.h"
#include "Constant.h"

class Actor;

typedef Actor Text;

class Message
{

public:
	static void DisplayText(const String& text, const Area& area = AREA(0, 0, WINDOW_WIDTH, 12), const UINT& size = 12, const COLORREF& color = RGB(255, 255, 255), const UINT& textAlign = DT_LEFT, const TimestampType& duration = 3*1000);
	static void ShowMessage(const String& text);
	static void ShowMCIError(const DWORD& dwError);
};

