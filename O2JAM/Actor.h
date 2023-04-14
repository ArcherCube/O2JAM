#pragma once

#include <windows.h>
#include <set>
#include <unordered_set>
#include <iterator>
#include "CommonType.h"

#include "Model.h"
#include "BitmapModel.h"
#include "TextModel.h"


class Actor;

struct ActorRenderNOLess
{
	bool operator()(const Actor* const  _Left, const Actor* const _Right) const;
};
typedef std::set<const Actor*, ActorRenderNOLess> RenderTaskSet;
typedef RenderTaskSet::const_iterator RenderTaskSetCiter;

class Actor
{
protected:
	//在场景中的范围
	Area area;

	//渲染高度（越大越在上方，越靠后渲染）
	UINT z;
private:
	//内部模型
	Model* const model;

	//加入渲染队列后得到的“句柄”
	const RenderTaskSetCiter renderTaskSetCiter;
protected:
	UINT PlayAction(const UINT& no) const;
public:
	explicit Actor(const String& sourcePath, const Position& position = POSITION(0, 0), const UINT& z = 0, const Size& fullSize = SIZE(0, 0), const DWORD& renderType = SRCCOPY);
	explicit Actor(const String& text, const COLORREF& color, const UINT& size, const Area& area = AREA(0, 0, 0, 0), const UINT& z = 0, const UINT& textAlign = DT_LEFT);

	const Model* const GetModel() const;
	const Area& GetArea() const;
	const UINT SetArea(const Area&);
	const UINT GetZ() const;

	const UINT MoveTo(const Position& position);

	virtual ~Actor();
};