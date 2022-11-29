//==============================================================================
// Filename: input.h
// Description: 入力用クラスの定義
//==============================================================================
#pragma once
#pragma warning( disable : 4244 )//警告対策
#include "main.h"
class Input
{
private:
	static BYTE m_OldKeyState[256];
	static BYTE m_KeyState[256];
	static POINT po;
	static LPDIRECTINPUTDEVICE8 pMouse; // mouse
	static DIMOUSESTATE2   mouseState;		// マウスのダイレクトな状態
	static DIMOUSESTATE2   mouseTrigger;	// 押された瞬間だけON
	static LPDIRECTINPUT8	g_pDInput;		// IDirectInput8インターフェースへのポインタ
	static D3DXVECTOR2 FixedPoint;
	static bool pause;
	static HRESULT InitMouse(HWND hWindow, HINSTANCE instance);
	static void UninitMouse() {
		if (pMouse)
		{
			pMouse->Unacquire();
			pMouse->Release();
			pMouse = NULL;
		}
	}
	static HRESULT UpdateMouse()
	{
		HRESULT result;
		// 前回の値保存
		DIMOUSESTATE2 lastMouseState = mouseState;
		// データ取得
		result = pMouse->GetDeviceState(sizeof(mouseState), &mouseState);
		if (SUCCEEDED(result))
		{
			mouseTrigger.lX = mouseState.lX;
			mouseTrigger.lY = mouseState.lY;
			mouseTrigger.lZ = mouseState.lZ;
			// マウスのボタン状態
			for (int i = 0; i < 8; i++)
			{
				mouseTrigger.rgbButtons[i] = ((lastMouseState.rgbButtons[i] ^
					mouseState.rgbButtons[i]) & mouseState.rgbButtons[i]);
			}
		}
		else	// 取得失敗
		{
			// アクセス権を得てみる
			result = pMouse->Acquire();
		}
		return result;
	}

public:
	static void Init();
	static void Uninit();
	static void Update();

	//キーボードが押されている間は正
	static bool GetKeyPress(BYTE KeyCode);

	//キーボードが押された瞬間のみ正
	static bool GetKeyTrigger(BYTE KeyCode);

	//カーソルの現在位置の取得(左上が0,0)
	static D3DXVECTOR2 GetCursor();

	//カーソルの表示/非表示
	static void ShowPoint(bool Show);

	//マウスの左クリックが押されている間は正
	static BOOL IsMouseLeftPressed(void)
	{
		return (BOOL)(mouseState.rgbButtons[0] & 0x80);	// 押されたときに立つビットを検査
	}

	//マウスの左クリックが押されている瞬間のみ正
	static BOOL IsMouseLeftTriggered(void)
	{
		return (BOOL)(mouseTrigger.rgbButtons[0] & 0x80);
	}

	//マウスの右クリックが押されている間は正
	static BOOL IsMouseRightPressed(void)
	{
		return (BOOL)(mouseState.rgbButtons[1] & 0x80);
	}

	//マウスの右クリックが押されている瞬間のみ正
	static BOOL IsMouseRightTriggered(void)
	{
		return (BOOL)(mouseTrigger.rgbButtons[1] & 0x80);
	}

	//マウスのホイールが押されている間は正
	static BOOL IsMouseCenterPressed(void)
	{
		return (BOOL)(mouseState.rgbButtons[2] & 0x80);
	}

	//マウスのホイールが押されている瞬間のみ正
	static BOOL IsMouseCenterTriggered(void)
	{
		return (BOOL)(mouseTrigger.rgbButtons[2] & 0x80);
	}

	static D3DXVECTOR2 MouseRelative();

	static void SetPause(bool inPause)
	{
		pause = inPause;
		return;
	}
	static bool GetPause()
	{
		return pause;
	}

	static Float3 GetMouse()
	{
		return Float3(mouseTrigger.lX, mouseTrigger.lY, mouseTrigger.lZ);
	}

	static int MouseWheel() {
		return mouseTrigger.lZ / 120;
	}
};
