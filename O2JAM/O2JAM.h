#pragma once

#include "resource.h"
#include "framework.h"

#include "Constant.h"

class Game;

HINSTANCE hInst;                                // ��ǰʵ��
WCHAR szTitle[MAX_LOADSTRING];                  // �������ı�
WCHAR szWindowClass[MAX_LOADSTRING];            // ����������
Game* game;                                     // ��Ϸʵ��

LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
ATOM                RegisterWindowClass(HINSTANCE hInstance);
HWND                InitInstance(HINSTANCE, int);
void                InitString(HINSTANCE hInstance);
