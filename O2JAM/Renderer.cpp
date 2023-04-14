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

	//����mdc�ġ�������
	SelectObject(mdc, bmp);

	//��mdcд��ʱ���ֱ���͸��
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

	//δ�����ж�
	return 0;
}

UINT Renderer::RenderObject(const Actor* const actor)
{
	//ȡ��actor�ڲ�ģ��
	const Model* const model = actor->GetModel();
	if (!model) return 0;

	//ȡ��actor���ھ���
	const Area& area = actor->GetArea();

	//��¼��ͼλ��
	const Position& paintPosition = area.sPosition;

	//��¼��ͼ�յ㣨������ΪDrawtext����ʾ��������½ǣ�
	const Position& paintPositionEnd = area.ePosition;


	//��model���Ͳ�ͬ����ͼ������ͬ
	switch (model->GetModelType())
	{
	case Model::ModelType::BITMAP:
	{
		//����λͼ���趨bufdc����Դ
		SelectObject(bufdc, model->GetHandle());

		const BitmapModel* const bitmapModel = (const BitmapModel* const)model;

		//��ȡ��ǰ����������
		const ActionPiece& currentActionPiece = bitmapModel->GetCurrentActionPiece();
		const Position& sourcePosition = currentActionPiece.sourcePosition;
		const Size& paintSize = currentActionPiece.size;

		//��ȡλͼģ�ͼ���ʱ�����ű���
		const double cWidth = (double)bitmapModel->GetFullSize().width / (double)bitmapModel->GetSourceSize().width;
		const double cHeight = (double)bitmapModel->GetFullSize().height / (double)bitmapModel->GetSourceSize().height;

		//��ͼ
		BitBlt(mdc, (int)paintPosition.x, (int)paintPosition.y, (int)(paintSize.width * cWidth), (int)(paintSize.height * cHeight), bufdc, (int)sourcePosition.x, (int)sourcePosition.y, bitmapModel->GetRenderType());
	}
	break;
	case Model::ModelType::TEXT:
	{
		//�������֣��趨mdc����Դ
		SelectObject(mdc, model->GetHandle());

		const TextModel* const textModel = (const TextModel* const)model;

		//�趨��ɫ
		SetTextColor(mdc, textModel->GetColor());

		//�趨����
		RECT&& rect = { (long)paintPosition.x,(long)paintPosition.y,(long)paintPositionEnd.x ,(long)paintPositionEnd.y };

		//��ͼ���֣�
		DrawText(mdc, textModel->GetText().c_str(), (int)textModel->GetText().size(), &rect, DT_SINGLELINE | DT_VCENTER | textModel->GetTextAlign());
	}
	break;
	default:
		break;
	}

	//��Ⱦ���е�component


	//�ж�δ����
	return 1;
}

UINT Renderer::Render()
{
	//�������е�actor���λ��Ƶ�mdc��
	for (const auto& actor : (*renderTaskSet))
	{
		RenderObject(actor);
	}

	//���mdcһ���Ի��Ƶ�hdc��
	BitBlt(hdc, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, mdc, 0, 0, SRCCOPY);

	//δ�����ж�
	return 0;
}

UINT Renderer::Clean()
{
	//actor��component�����йܶ��󣬲���Ҫ�Լ�ɾ����ԭ���ĸ���new���ĸ���delete
	renderTaskSet->clear();
	delete renderTaskSet;

	if (!DeleteObject(bmp)) return 0;
	if (!DeleteDC(bufdc))return 0;
	if (!DeleteDC(mdc))return 0;
	if (!ReleaseDC(hWnd, hdc))return 0;
	return 1;
}
