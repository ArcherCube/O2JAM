#pragma once

#include <windows.h>
#include <iostream>
#include <set> //��Ⱦ����Ԫ��֮��û�����Թ�ϵ����Ҫ������Ҫ���ɾ�������ṩ����ָ�룩����Ӧ���ظ�
#include <iterator>
#include "CommonType.h"

#include "Actor.h"

class Renderer
{
public:
	//�����������ֶζ���Ӧ����Init���ⱻ�ı䡣����const����Ϊ��hWnd��Winain��ʼ��ų��֣�����̬��Ա�ĳ�ʼ��û�С�ʱ�䡱�ĸ�����ԣ�����ͨ��Init��������ֵ��
	static HWND hWnd;
	static HDC hdc, mdc, bufdc;
	static HBITMAP bmp;

	//actor������ʱ�ͻᱻ�ǽ����set
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

