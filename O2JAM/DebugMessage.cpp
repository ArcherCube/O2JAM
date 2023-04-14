#include "Message.h"

#include "Actor.h"
#include "CentralTimer.h"

void Message::DisplayText(const String& text, const Area& area, const UINT& size, const COLORREF& color, const UINT& textAlign, const TimestampType& duration)
{
	const Text* const displayText = new Text(text, color, (int)size, area, MESSAGE_TEXT_Z, textAlign);
	CentralTimer::Add(TO_STRING((int)displayText), duration,
		[displayText]
		{
			delete displayText;
		});
}

void Message::ShowMessage(const String& text)
{
	MessageBox(nullptr, text.c_str(), TEXT("message"), MB_ICONEXCLAMATION);
}

void Message::ShowMCIError(const DWORD& dwError)
{
	WCHAR szErrorBuf[MAXERRORLENGTH];
	MessageBeep(MB_ICONEXCLAMATION);
	if (mciGetErrorString(dwError, (LPWSTR)szErrorBuf, MAXERRORLENGTH))
	{
		MessageBox(nullptr, szErrorBuf, TEXT("MCI Error"),
			MB_ICONEXCLAMATION);
	}
	else
	{
		MessageBox(nullptr, TEXT("Unknown Error"), TEXT("MCI Error"),
			MB_ICONEXCLAMATION);
	}
}