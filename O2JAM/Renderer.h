#pragma once

#include <windows.h>
#include <iostream>
#include <set> //渲染队列元素之间没有线性关系、需要有序、需要随机删（可以提供对象指针）、不应该重复
#include <iterator>
#include "CommonType.h"

#include "Actor.h"

class Renderer
{
public:
	//此类中所有字段都不应该在Init以外被改变。不加const是因为，hWnd在Winain开始后才出现，而静态成员的初始化没有“时间”的概念，所以，必须通过Init来设置其值。
	static HWND hWnd;
	static HDC hdc, mdc, bufdc;
	static HBITMAP bmp;

	//actor被创建时就会被记进这个set
	static RenderTaskSet* renderTaskSet;
private:
	static UINT RenderObject(const Actor* const actor);
public:
	static UINT Init(HWND const hWnd);

	static RenderTaskSetCiter AddRenderTask(const Actor* const actor);
	static UINT DelRenderTask(RenderTaskSetCiter renderTaskSetCiter);
	static UINT Render();

	static UINT Clean();
};

