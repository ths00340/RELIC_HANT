#include "main.h"
#include "input.h"

BYTE Input::m_OldKeyState[256];
BYTE Input::m_KeyState[256];
POINT Input::po;
D3DXVECTOR2 Input::FixedPoint;
LPDIRECTINPUTDEVICE8	Input::pMouse;
DIMOUSESTATE2			Input::mouseState;
DIMOUSESTATE2			Input::mouseTrigger;
LPDIRECTINPUT8			Input::g_pDInput;
bool Input::pause;

void Input::Init()
{
	pause = false;
	memset(m_OldKeyState, 0, 256);
	memset(m_KeyState, 0, 256);
	GetCursorPos(&po);
	ShowPoint(false);
	InitMouse(GetWindow(), GetInstance());
}

void Input::Uninit()
{
	UninitMouse();
}

void Input::Update()
{
	memcpy(m_OldKeyState, m_KeyState, 256);

	GetKeyboardState(m_KeyState);

	FixedPoint = GetCursor();

	GetCursorPos(&po);

	if (!pause)
		SetCursorPos(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);

	UpdateMouse();
}

bool Input::GetKeyPress(BYTE KeyCode)
{
	return (m_KeyState[KeyCode] & 0x80);
}

bool Input::GetKeyTrigger(BYTE KeyCode)
{
	return ((m_KeyState[KeyCode] & 0x80) && !(m_OldKeyState[KeyCode] & 0x80));
}

D3DXVECTOR2 Input::GetCursor()
{
	return D3DXVECTOR2(po.x, po.y);
}

void Input::ShowPoint(bool Show)
{
	ShowCursor(Show);
}

HRESULT Input::InitMouse(HWND hWindow, HINSTANCE instance)
{
	HRESULT hr;
	FixedPoint = GetCursor();
	if (!g_pDInput)
	{
		// DirectInput�I�u�W�F�N�g�̍쐬
		hr = DirectInput8Create(instance, DIRECTINPUT_VERSION,
			IID_IDirectInput8, (void**)&g_pDInput, NULL);
	}

	HRESULT result;
	// �f�o�C�X�쐬
	result = g_pDInput->CreateDevice(GUID_SysMouse, &pMouse, NULL);
	if (FAILED(result) || pMouse == NULL)
	{
		MessageBox(hWindow, "No mouse", "Warning", MB_OK | MB_ICONWARNING);
		return result;
	}
	// �f�[�^�t�H�[�}�b�g�ݒ�
	result = pMouse->SetDataFormat(&c_dfDIMouse2);
	if (FAILED(result))
	{
		MessageBox(hWindow, "Can't setup mouse", "Warning", MB_OK | MB_ICONWARNING);
		return result;
	}
	// ���̃A�v���Ƌ������[�h�ɐݒ�
	result = pMouse->SetCooperativeLevel(hWindow, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE));
	if (FAILED(result))
	{
		MessageBox(hWindow, "Mouse mode error", "Warning", MB_OK | MB_ICONWARNING);
		return result;
	}

	// �f�o�C�X�̐ݒ�
	DIPROPDWORD prop;

	prop.diph.dwSize = sizeof(prop);
	prop.diph.dwHeaderSize = sizeof(prop.diph);
	prop.diph.dwObj = 0;
	prop.diph.dwHow = DIPH_DEVICE;
	prop.dwData = DIPROPAXISMODE_REL;		// �}�E�X�̈ړ��l�@���Βl

	result = pMouse->SetProperty(DIPROP_AXISMODE, &prop.diph);
	if (FAILED(result))
	{
		MessageBox(hWindow, "Mouse property error", "Warning", MB_OK | MB_ICONWARNING);
		return result;
	}

	// �A�N�Z�X���𓾂�
	pMouse->Acquire();
	return result;
}

D3DXVECTOR2 Input::MouseRelative()
{
	D3DXVECTOR2 pos = GetCursor() - FixedPoint;

	FixedPoint = GetCursor();
	return pos;
}