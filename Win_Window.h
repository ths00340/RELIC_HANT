//==============================================================================
// Filename: Win_Window.h
// Description: ウィンドウ作成ヘッダー
//==============================================================================

#pragma once

#include <windows.h>

//------------------------------------------------------------------------------
/// シンプルなウィンドウの作成
/// param [in] hInstance	インスタンスハンドル
/// param [in] nCmdShow		ウィンドウの表示方法
/// param [in] Width		ウィンドウの横幅
/// param [in] Height		ウィンドウの縦幅
///
/// /return void
//------------------------------------------------------------------------------
bool WinCreateSimpleWindow(
	/*[in]*/ HINSTANCE hInstance,
	/*[in]*/ int nCmdShow,
	/*[in]*/ int Width,
	/*[in]*/ int Height
);

/// メッセージの更新
bool WinUpdateWindowMessage(void);

/// 終了通知が来ているか？
bool WinIsQuitMessage(void);

/// ウィンドウハンドルの取得
HWND WinGetWindowHandle(void);

/// インスタンスの取得
HINSTANCE WinGetInstance();

bool WinUninitWindow(void);
