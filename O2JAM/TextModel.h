#pragma once

#include <windows.h>

#include "Model.h"


class TextModel :public Model
{
private:
	//��ʾ���ݵ��ַ���
	String text;

	//������ɫ
	COLORREF color;

	//�����С
	UINT size;

	//���뷽ʽ
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

