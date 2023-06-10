//==============================================================================
// Filename: input.h
// Description: 入力用クラスの定義
//==============================================================================
#pragma once
#pragma warning( disable : 4244 )//警告対策
#include "main.h"

#define GAMEPAD_MAX (4)

/* game pad情報 */
#define BUTTON_UP		0x00000001l	// 方向キー上(.IY<0)
#define BUTTON_DOWN		0x00000002l	// 方向キー下(.IY>0)
#define BUTTON_LEFT		0x00000004l	// 方向キー左(.IX<0)
#define BUTTON_RIGHT	0x00000008l	// 方向キー右(.IX>0)

#define BUTTON_X		0x00000010l	// Ｘボタン(.rgbButtons[0]&0x80)
#define BUTTON_A		0x00000020l	// Ａボタン(.rgbButtons[1]&0x80)
#define BUTTON_B		0x00000040l	// Ｂボタン(.rgbButtons[2]&0x80)
#define BUTTON_Y		0x00000080l	// Ｙボタン(.rgbButtons[3]&0x80)
#define BUTTON_L		0x00000100l	// Ｌボタン(.rgbButtons[4]&0x80)
#define BUTTON_R		0x00000200l	// Ｒボタン(.rgbButtons[5]&0x80)
#define BUTTON_L2		0x00000400l	// Ｌボタン(.rgbButtons[6]&0x80)
#define BUTTON_R2		0x00000800l	// Ｒボタン(.rgbButtons[7]&0x80)
#define BUTTON_SELECT	0x00001000l	// Ｚボタン(.rgbButtons[8]&0x80)
#define BUTTON_START	0x00002000l	// ＳＴＡＲＴボタン(.rgbButtons[9]&0x80)

enum {	// ボタンに対するrgbButtons[?]配列の配列番号(DirectInput上でのXYABボタン位置に合わせた)
	rgbButtons_X,
	rgbButtons_A,
	rgbButtons_B,
	rgbButtons_Y,
	rgbButtons_L,
	rgbButtons_R,
	rgbButtons_L2,
	rgbButtons_R2,
	rgbButtons_SELECT,
	rgbButtons_START,
};

class Input
{
private:
	static LPDIRECTINPUT8	m_pDInput;		// IDirectInput8インターフェースへのポインタ

	//キーボード
	static LPDIRECTINPUTDEVICE8 m_pDIDevKeyboard;
	static BYTE m_KeyState[256];//現在の状態
	static BYTE m_KeyTrigger[256];//押されたかの取得
	static BYTE m_KeyRepeat[256];//長押し
	static BYTE m_KeyRelease[256];//離したか
	static BYTE m_KeyRepeatCnt[256];//リピート数

	//マウス
	static LPDIRECTINPUTDEVICE8 m_pMouse; // mouse
	static DIMOUSESTATE2   m_MouseState;	// マウスのダイレクトな状態
	static DIMOUSESTATE2   m_MouseTrigger;	// 押された瞬間だけON
	static POINT po;
	static BOOL m_isMidPointer;

	//パッド
	static LPDIRECTINPUTDEVICE8 m_pGamePad[GAMEPAD_MAX];
	static DWORD	m_PadState[GAMEPAD_MAX];	// パッド情報（複数対応）
	static DWORD	m_PadTrigger[GAMEPAD_MAX];
	static DIJOYSTATE2	m_JoyState2[GAMEPAD_MAX];
	static DIJOYSTATE	m_JoyState[GAMEPAD_MAX];
	static int		m_PadCount;			// 検出したパッドの数

	static BOOL CALLBACK SearchGamePadCallback(LPDIDEVICEINSTANCE lpddi, LPVOID);

	//初期化
	static HRESULT InitMouse(HWND hWindow);
	static HRESULT InitKeyboard(HWND hWindow);
	static HRESULT InitPad(HWND hWindow);

	//終了処理
	static void UninitMouse();
	static void UninitKeyboard();
	static void UninitPad();

	//更新処理
	static HRESULT UpdateMouse();
	static HRESULT UpadteKeyboard();
	static HRESULT UpdatePad();

public:
	static void Init();
	static void Uninit();
	static void Update();

	//キーボードが押されている間は正
	static bool GetKeyPress(BYTE KeyCode);

	//キーボードが押された瞬間のみ正
	static bool GetKeyTrigger(BYTE KeyCode);

	//キーボードの連続入力
	static bool GetKeyRepeat(BYTE KeyCode);

	//キーボードが離した瞬間のみ正
	static bool GetKeyRelease(BYTE KeyCode);

	//カーソルの現在位置の取得(左上が0,0)
	static D3DXVECTOR2 GetCursor();

	//カーソルの表示/非表示
	static void ShowPoint(bool Show);

	//マウスの左クリックが押されている間は正
	static BOOL IsMouseLeftPressed(void)
	{
		return (BOOL)(m_MouseState.rgbButtons[0] & 0x80);	// 押されたときに立つビットを検査
	}

	//マウスの左クリックが押されている瞬間のみ正
	static BOOL IsMouseLeftTriggered(void)
	{
		return (BOOL)(m_MouseTrigger.rgbButtons[0] & 0x80);
	}

	//マウスの右クリックが押されている間は正
	static BOOL IsMouseRightPressed(void)
	{
		return (BOOL)(m_MouseState.rgbButtons[1] & 0x80);
	}

	//マウスの右クリックが押されている瞬間のみ正
	static BOOL IsMouseRightTriggered(void)
	{
		return (BOOL)(m_MouseTrigger.rgbButtons[1] & 0x80);
	}

	//マウスのホイールが押されている間は正
	static BOOL IsMouseCenterPressed(void)
	{
		return (BOOL)(m_MouseState.rgbButtons[2] & 0x80);
	}

	//マウスのホイールが押されている瞬間のみ正
	static BOOL IsMouseCenterTriggered(void)
	{
		return (BOOL)(m_MouseTrigger.rgbButtons[2] & 0x80);
	}

	static D3DXVECTOR2 MouseRelative();

	static Float3 GetMouse()
	{
		return Float3(m_MouseTrigger.lX, m_MouseTrigger.lY, m_MouseTrigger.lZ);
	}

	static int MouseWheel() {
		return m_MouseTrigger.lZ / 120;
	}

	static void CangeFixedPointer()
	{
		m_isMidPointer = m_isMidPointer ? false : true;
	}

	static void EndFixedPointer()
	{
		m_isMidPointer = false;
	}

	static BOOL IsButtonPressed(int padNo, DWORD button);
	static BOOL IsButtonTriggered(int padNo, DWORD button);

	static int GetPadCount();

	static DIJOYSTATE GetJoyState(int padnum);
	static DIJOYSTATE2 GetJoyState2(int padnum);

	static D3DXVECTOR2 GetJoyPadRight(int padnum);
	static D3DXVECTOR2 GetJoyPadLeft(int padnum);
	static float GetL2(int padnum);
	static float GetR2(int padnum);
};
