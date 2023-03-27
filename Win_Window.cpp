//==============================================================================
// Filename: Win_Window.cpp
// Description: �E�B���h�E�쐬
//==============================================================================

#include "Win_Window.h"

const char* CLASS_NAME = "AppClass";
const char* WINDOW_NAME = "RELIC HANT";

/// �E�B���h�E�n���h��
HWND g_Window = NULL;

/// �I���ʒm�����Ă��邩�H
bool g_IsQuitMessage = false;

WNDCLASSEX wcex;

HINSTANCE g_Instance;

// �E�C���h�E�v���V�[�W���[
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

/// �V���v���ȃE�B���h�E�̍쐬
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

	/// �E�C���h�E�̕\��
	//ShowWindow(g_Window, nCmdShow);
	ShowWindow(g_Window, SW_SHOWMAXIMIZED);
	UpdateWindow(g_Window);

	return true;
}

/// ���b�Z�[�W�̍X�V
bool WinUpdateWindowMessage(void)
{
	MSG Msg;
	/// ���b�Z�[�W�����݂��邩�m�F
	if (PeekMessage(&Msg, NULL, 0, 0, PM_REMOVE))
	{
		/// �I���ʒm�����Ă���ꍇ�͔�����
		if (Msg.message == WM_QUIT)
		{
			g_IsQuitMessage = true;
		}
		else
		{
			/// ���b�Z�[�W���E�C���h�E�v���V�[�W���ɓ]��
			TranslateMessage(&Msg);
			DispatchMessage(&Msg);
		}
		return true;
	}
	return false;
}

/// �I���ʒm�����Ă��邩�H
bool WinIsQuitMessage(void)
{
	return g_IsQuitMessage;
}

/// �E�B���h�E�n���h���̎擾
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