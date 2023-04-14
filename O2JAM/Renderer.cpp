#include "Renderer.h"

#include "Constant.h"
#include "Actor.h"
#include "Model.h"
#include "BitmapModel.h"
#include "TextModel.h"
#include "Action.h"

HWND Renderer::hWnd = nullptr;
HDC Renderer::hdc = nullptr;
HDC Renderer::mdc = nullptr;
HDC Renderer::bufdc = nullptr;
HBITMAP Renderer::bmp = nullptr;
RenderTaskSet* Renderer::renderTaskSet = new RenderTaskSet();

UINT Renderer::Init(HWND const _hWnd)
{
	hWnd = _hWnd;
	hdc = GetDC(hWnd);
	mdc = CreateCompatibleDC(hdc);
	bufdc = CreateCompatibleDC(mdc);
	bmp = CreateCompatibleBitmap(hdc, WINDOW_WIDTH, WINDOW_HEIGHT);

	//创建mdc的“画布”
	SelectObject(mdc, bmp);

	//在mdc写字时文字背景透明
	SetBkMode(mdc, TRANSPARENT);

	return 0;
}

RenderTaskSetCiter Renderer::AddRenderTask(const Actor* const actor)
{
	return renderTaskSet->insert(actor).first;
}

UINT Renderer::DelRenderTask(RenderTaskSetCiter renderTaskSetCiter)
{
	renderTaskSet->erase(renderTaskSetCiter);

	//未启用判断
	return 0;
}

UINT Renderer::RenderObject(const Actor* const actor)
{
	//取出actor内部模型
	const Model* const model = actor->GetModel();
	if (!model) return 0;

	//取出actor所在矩形
	const Area& area = actor->GetArea();

	//记录绘图位置
	const Position& paintPosition = area.sPosition;

	//记录绘图终点（本质上为Drawtext所表示区域的右下角）
	const Position& paintPositionEnd = area.ePosition;


	//按model类型不同，绘图方法不同
	switch (model->GetModelType())
	{
	case Model::ModelType::BITMAP:
	{
		//对于位图，设定bufdc的资源
		SelectObject(bufdc, model->GetHandle());

		const BitmapModel* const bitmapModel = (const BitmapModel* const)model;

		//获取当前动作的数据
		const ActionPiece& currentActionPiece = bitmapModel->GetCurrentActionPiece();
		const Position& sourcePosition = currentActionPiece.sourcePosition;
		const Size& paintSize = currentActionPiece.size;

		//获取位图模型加载时的缩放比例
		const double cWidth = (double)bitmapModel->GetFullSize().width / (double)bitmapModel->GetSourceSize().width;
		const double cHeight = (double)bitmapModel->GetFullSize().height / (double)bitmapModel->GetSourceSize().height;

		//绘图
		BitBlt(mdc, (int)paintPosition.x, (int)paintPosition.y, (int)(paintSize.width * cWidth), (int)(paintSize.height * cHeight), bufdc, (int)sourcePosition.x, (int)sourcePosition.y, bitmapModel->GetRenderType());
	}
	break;
	case Model::ModelType::TEXT:
	{
		//对于文字，设定mdc的资源
		SelectObject(mdc, model->GetHandle());

		const TextModel* const textModel = (const TextModel* const)model;

		//设定字色
		SetTextColor(mdc, textModel->GetColor());

		//设定区域
		RECT&& rect = { (long)paintPosition.x,(long)paintPosition.y,(long)paintPositionEnd.x ,(long)paintPositionEnd.y };

		//绘图（字）
		DrawText(mdc, textModel->GetText().c_str(), (int)textModel->GetText().size(), &rect, DT_SINGLELINE | DT_VCENTER | textModel->GetTextAlign());
	}
	break;
	default:
		break;
	}

	//渲染其中的component


	//判断未启用
	return 1;
}

UINT Renderer::Render()
{
	//将队列中的actor依次绘制到mdc中
	for (const auto& actor : (*renderTaskSet))
	{
		RenderObject(actor);
	}

	//最后将mdc一次性绘制到hdc中
	BitBlt(hdc, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, mdc, 0, 0, SRCCOPY);

	//未启用判断
	return 0;
}

UINT Renderer::Clean()
{
	//actor和component都是托管对象，不需要自己删除。原则：哪个类new，哪个类delete
	renderTaskSet->clear();
	delete renderTaskSet;

	if (!DeleteObject(bmp)) return 0;
	if (!DeleteDC(bufdc))return 0;
	if (!DeleteDC(mdc))return 0;
	if (!ReleaseDC(hWnd, hdc))return 0;
	return 1;
}
