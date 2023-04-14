#pragma once

#include <windows.h>

#include "Model.h"


class TextModel :public Model
{
private:
	//表示内容的字符串
	String text;

	//字体颜色
	COLORREF color;

	//字体大小
	UINT size;

	//对齐方式
	UINT textAlign;
public:
	explicit TextModel(const String& text, const COLORREF& color, const UINT& size, const UINT& textAlign = DT_LEFT);

	const String& GetText() const;
	const COLORREF GetColor() const;
	const UINT GetSize() const;
	const UINT GetTextAlign() const;

	const UINT SetText(const String& text);
	const UINT SetColor(const COLORREF& color);

	virtual ~TextModel();
};

