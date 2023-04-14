#include "Actor.h"

#include "Renderer.h"

Actor::Actor(const String& sourcePath, const Position& position, const UINT& z, const Size& fullSize, const DWORD& renderType)
	:model(new BitmapModel(sourcePath, fullSize, renderType)), area(AREA(position.x, position.y, 0, 0)), z(z)
	, renderTaskSetCiter(Renderer::AddRenderTask(this))
{
}

Actor::Actor(const String& text, const COLORREF& color, const UINT& size, const Area& area, const UINT& z, const UINT& textAlign)
	: model(new TextModel(text, color, size, textAlign)), area(area), z(z)
	, renderTaskSetCiter(Renderer::AddRenderTask(this))
{
}

const Model* const Actor::GetModel() const
{
	return model;
}


UINT Actor::PlayAction(const UINT& no) const
{
	((BitmapModel* const)model)->ChangeCurrentAction(no);

	//Î´ÆôÓÃÅÐ¶Ï
	return 0;
}


const Area& Actor::GetArea() const
{
	return area;
}

const UINT Actor::SetArea(const Area& area)
{
	this->area = area;

	//Î´ÆôÓÃÅÐ¶Ï
	return 0;
}

const UINT Actor::GetZ() const
{
	return z;
}

const UINT Actor::MoveTo(const Position& position)
{
	area.ePosition.x += area.sPosition.x - position.x;
	area.ePosition.y += area.sPosition.y - position.y;
	area.sPosition.x = position.x;
	area.sPosition.y = position.y;

	//Î´ÆôÓÃÅÐ¶Ï
	return 0;
}

Actor::~Actor()
{
	//´ÓäÖÈ¾¶ÓÁÐÖÐÒÆ³ý
	Renderer::DelRenderTask(renderTaskSetCiter);

	delete model;

}



bool ActorRenderNOLess::operator()(const Actor* const _Left, const Actor* const _Right) const
{
	return _Left->GetZ() <= _Right->GetZ();
}