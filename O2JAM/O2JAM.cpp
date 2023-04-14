// O2JAM.cpp : 定义应用程序的入口点。
//

#include "O2JAM.h"

#include "Game.h"
#include "Renderer.h"
#include "FileUtil.h"
#include "CentralTimer.h"
#include "CommonType.h"
#include "Recorder.h"

#include "Message.h"


int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
	//不使用的参数
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	//注册窗口类
	RegisterWindowClass(hInstance);

	// 执行应用程序初始化:
	HWND hWnd;
	if (!(hWnd = InitInstance(hInstance, nCmdShow))) return FALSE;

	//必要的工具类初始化（Renderer和PhysicsEngine必须在game之前）
	Renderer::Init(hWnd);
	FileUtil::Init(hWnd);

	//加载游戏实体
	if (!(game = new Game())) return FALSE;

	// 主消息循环:
	MSG msg = { 0 };
	while (msg.message != WM_QUIT)
	{
		CentralTimer::Tick();

		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		game->Update();

		Renderer::Render();
	}

	delete game;

	//工具类的必要清理。（Renderer必须在game之后）
	CentralTimer::Clean();
	Renderer::Clean();
	Recorder::Clean();


	UnregisterClass(szWindowClass, hInst);

	return (int)msg.wParam;
}

//
//  函数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目标: 处理主窗口的消息。
//
//  WM_COMMAND  - 处理应用程序菜单
//  WM_PAINT    - 绘制主窗口
//  WM_DESTROY  - 发送退出消息并返回
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		// 分析菜单选择:

		switch (wmId)
		{
		case CMD_OPEN:
		case CMD_CLOSE:
		case CMD_PLAY:
		case CMD_PAUSE:
		case CMD_RECORD:
			if (game) game->ProcessCommand(wmId);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
	break;
	case WM_KEYDOWN:
		if (game) game->ProcessKeyDown(KeyName(wParam));
		break;
	case WM_KEYUP:
		if (game) game->ProcessKeyUp(KeyName(wParam));
		break;
	case WM_PAINT:
	{
		//PAINTSTRUCT ps;
		//HDC hdc = BeginPaint(hWnd, &ps);

		if (game)
		{
			CentralTimer::Tick();
			game->Update();
			Renderer::Render();
		}

		//EndPaint(hWnd, &ps);

		ValidateRect(hWnd, NULL);
	}
	break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

//
//  函数: MyRegisterClass()
//
//  目标: 注册窗口类。
//
ATOM RegisterWindowClass(HINSTANCE hInstance)
{
	InitString(hInstance);

	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_O2JAM));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(GetStockObject(BLACK_BRUSH));
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_O2JAM);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//  函数: InitString()
//
//  目标: 加载所有字符串。
//
void InitString(HINSTANCE hInstance)
{
	// 初始化字符串
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_O2JAM, szWindowClass, MAX_LOADSTRING);
}

//
//   函数: InitInstance(HINSTANCE, int)
//
//   目标: 保存实例句柄并创建主窗口
//
//   注释: 在此函数中，我们在全局变量中保存实例句柄并
//         创建和显示主程序窗口。
//
HWND InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; // 将实例句柄存储在全局变量中

	//通过AdjustWindowRect得到实际的客户区尺寸。windowRect为目标尺寸所代表的区域，通过AdjustWindowRect后得到参入了各种其他信息（菜单栏、非客户区等）后的尺寸的真实区域
	RECT windowRect = { 0,0,WINDOW_WIDTH, WINDOW_HEIGHT };
	AdjustWindowRect(&windowRect, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX, true);

	HWND hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
		(GetSystemMetrics(SM_CXFULLSCREEN) - WINDOW_WIDTH) / 2, (GetSystemMetrics(SM_CYFULLSCREEN) - WINDOW_HEIGHT) / 2,
		windowRect.right - windowRect.left, windowRect.bottom - windowRect.top,
		nullptr, nullptr, hInstance, nullptr);

	if (!hWnd)
	{
		return nullptr;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return hWnd;
}