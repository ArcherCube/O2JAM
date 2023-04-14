#include "TextModel.h"


TextModel::TextModel(const String& text, const COLORREF& color, const UINT& size, const UINT& textAlign)
	:text(text), color(color), size(size), textAlign(textAlign)
	, Model(CreateFont((int)size, 0, 0, 0, 0, 0, 0, 0, GB2312_CHARSET, 0, 0, 0, 0, TEXT("Œ¢»Ì—≈∫⁄")), ModelType::TEXT)
{
}

const String& TextModel::GetText() const
{
	return text;
}

const COLORREF TextModel::GetColor() const
{
	return color;
}

const UINT TextModel::GetSize() const
{
	return size;
}

const UINT TextModel::GetTextAlign() const
{
	return textAlign;
}

const UINT TextModel::SetText(const String& text)
{
	this->text.assign(text);

	//Œ¥∆Ù”√≈–∂œ
	return 0;
}

const UINT TextModel::SetColor(const COLORREF& color)
{
	this->color = color;

	//Œ¥∆Ù”√≈–∂œ
	return 0;
}

TextModel::~TextModel()
{
}
