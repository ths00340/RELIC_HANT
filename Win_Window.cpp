//==============================================================================
// Filename: Win_Window.cpp
// Description: ウィンドウ作成
//==============================================================================

#include "Win_Window.h"

const char* CLASS_NAME = "AppClass";
const char* WINDOW_NAME = "RELIC HANT";

/// ウィンドウハンドル
HWND g_Window = NULL;

/// 終了通知が来ているか？
bool g_IsQuitMessage = false;

WNDCLASSEX wcex;

HINSTANCE g_Instance;

// ウインドウプロシージャー
static LRESULT CALLBACK WinWndProc(HWND hWnd, UINT uiMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uiMsg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_ESCAPE:
			DestroyWindow(hWnd);
			break;
		}
		break;

	default:
		break;
	}

	return DefWindowProc(hWnd, uiMsg, wParam, lParam);
}

/// シンプルなウィンドウの作成
bool WinCreateSimpleWindow(HINSTANCE hInstance, int nCmdShow, int Width, int Height)
{
	wcex =
	{
		sizeof(WNDCLASSEX),
		CS_CLASSDC,
		WinWndProc,
		0,
		0,
		hInstance,
		NULL,
		LoadCursor(NULL, IDC_ARROW),
		(HBRUSH)(COLOR_WINDOW + 1),
		NULL,
		CLASS_NAME,
		NULL
	};

	RegisterClassEx(&wcex);

	g_Instance = wcex.hInstance;

	g_Window = CreateWindowEx(0,
		CLASS_NAME,
		WINDOW_NAME,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		(Width + GetSystemMetrics(SM_CXDLGFRAME) * 2),
		(Height + GetSystemMetrics(SM_CXDLGFRAME) * 2 + GetSystemMetrics(SM_CYCAPTION)),
		NULL,
		NULL,
		hInstance,
		NULL);

	if (g_Window == NULL)
	{
		return false;
	}

	/// ウインドウの表示
	//ShowWindow(g_Window, nCmdShow);
	ShowWindow(g_Window, SW_SHOWMAXIMIZED);
	UpdateWindow(g_Window);

	return true;
}

/// メッセージの更新
bool WinUpdateWindowMessage(void)
{
	MSG Msg;
	/// メッセージが存在するか確認
	if (PeekMessage(&Msg, NULL, 0, 0, PM_REMOVE))
	{
		/// 終了通知が来ている場合は抜ける
		if (Msg.message == WM_QUIT)
		{
			g_IsQuitMessage = true;
		}
		else
		{
			/// メッセージをウインドウプロシージャに転送
			TranslateMessage(&Msg);
			DispatchMessage(&Msg);
		}
		return true;
	}
	return false;
}

/// 終了通知が来ているか？
bool WinIsQuitMessage(void)
{
	return g_IsQuitMessage;
}

/// ウィンドウハンドルの取得
HWND WinGetWindowHandle(void)
{
	return g_Window;
}

bool WinUninitWindow(void)
{
	timeEndPeriod(1);

	UnregisterClass(CLASS_NAME, wcex.hInstance);

	return false;
}

HINSTANCE WinGetInstance() {
	return g_Instance;
}