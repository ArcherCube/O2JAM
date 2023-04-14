#pragma once

#include "resource.h"
#include "framework.h"

#include "Constant.h"

class Game;

HINSTANCE hInst;                                // 当前实例
WCHAR szTitle[MAX_LOADSTRING];                  // 标题栏文本
WCHAR szWindowClass[MAX_LOADSTRING];            // 主窗口类名
Game* game;                                     // 游戏实例

LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
ATOM                RegisterWindowClass(HINSTANCE hInstance);
HWND                InitInstance(HINSTANCE, int);
void                InitString(HINSTANCE hInstance);
