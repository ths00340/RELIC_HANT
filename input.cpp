#include "main.h"
#include "input.h"
#include "Win_Window.h"

// game pad�p�ݒ�l
#define DEADZONE		2500			// �e����25%�𖳌�
#define RANGE_MAX		1000			// �L���͈͂̍ő�l
#define RANGE_MIN		-1000			// �L���͈͂̍ŏ��l

LPDIRECTINPUTDEVICE8	Input::m_pDIDevKeyboard = NULL;
BYTE					Input::m_KeyState[256];//���݂̏��
BYTE					Input::m_KeyTrigger[256];//�����ꂽ���̎擾
BYTE					Input::m_KeyRepeat[256];//������
BYTE					Input::m_KeyRelease[256];//��������
BYTE					Input::m_KeyRepeatCnt[256];//���s�[�g��

POINT					Input::po;
LPDIRECTINPUTDEVICE8	Input::m_pMouse = NULL;
DIMOUSESTATE2			Input::m_MouseState;
DIMOUSESTATE2			Input::m_MouseTrigger;
LPDIRECTINPUT8			Input::m_pDInput = NULL;

LPDIRECTINPUTDEVICE8	Input::m_pGamePad[GAMEPAD_MAX] = { NULL,NULL,NULL,NULL };
DWORD					Input::m_PadState[GAMEPAD_MAX] = { NULL,NULL,NULL,NULL };	// �p�b�h���i�����Ή��j
DWORD					Input::m_PadTrigger[GAMEPAD_MAX] = { NULL,NULL,NULL,NULL };
DIJOYSTATE2				Input::m_JoyState2[GAMEPAD_MAX] = { NULL,NULL,NULL,NULL };
DIJOYSTATE				Input::m_JoyState[GAMEPAD_MAX] = { NULL,NULL,NULL,NULL };
int						Input::m_PadCount = 0;			// ���o�����p�b�h�̐�

BOOL Input::m_isMidPointer = false;

void Input::Init()
{
	HWND Window = WinGetWindowHandle();
	HINSTANCE Instance = WinGetInstance();

	HRESULT hr;
	if (!m_pDInput)
	{
		// DirectInput�I�u�W�F�N�g�̍쐬
		hr = DirectInput8Create(Instance, DIRECTINPUT_VERSION,
			IID_IDirectInput8, (void**)&m_pDInput, NULL);
	}

	InitKeyboard(Window); //�L�[�{�[�h�̏�����
	InitMouse(Window);	//�}�E�X�̏�����
	InitPad(Window);

	GetCursorPos(&po);
	ShowPoint(false);

	m_isMidPointer = false;
}

void Input::Uninit()
{
	UninitKeyboard();
	UninitMouse();
	UninitPad();

	if (m_pDInput)
	{
		m_pDInput->Release();
		m_pDInput = NULL;
	}
}

void Input::Update()
{
	GetCursorPos(&po);

	if (m_isMidPointer)
	{
		HWND hDesktop = GetDesktopWindow();
		HWND hWindow = WinGetWindowHandle();

		WINDOWINFO windowInfo;
		windowInfo.cbSize = sizeof(WINDOWINFO);

		GetWindowInfo(hDesktop, &windowInfo);

		RECT m_rect;
		GetWindowRect(hWindow, &m_rect);

		SetCursorPos(windowInfo.rcWindow.left + (m_rect.right / 2), windowInfo.rcWindow.top + (m_rect.bottom / 2));
	}

	UpadteKeyboard();
	UpdateMouse();
	UpdatePad();
}

D3DXVECTOR2 Input::GetCursor()
{
	return D3DXVECTOR2(po.x, po.y);
}

void Input::ShowPoint(bool Show)
{
	ShowCursor(Show);
}

///�L�[�{�[�h
HRESULT Input::InitKeyboard(HWND hWindow)
{
	HRESULT hr;

	// �f�o�C�X�I�u�W�F�N�g���쐬
	hr = m_pDInput->CreateDevice(GUID_SysKeyboard, &m_pDIDevKeyboard, NULL);
	if (FAILED(hr) || m_pDIDevKeyboard == NULL)
	{
		MessageBox(hWindow, "input::No keyboard", "Warning", MB_ICONWARNING);
		return hr;
	}

	// �f�[�^�t�H�[�}�b�g��ݒ�
	hr = m_pDIDevKeyboard->SetDataFormat(&c_dfDIKeyboard);
	if (FAILED(hr))
	{
		MessageBox(hWindow, "input::Can't setup keyboard", "Warning", MB_ICONWARNING);
		return hr;
	}

	// �������[�h��ݒ�i�t�H�A�O���E���h����r�����[�h�j
	hr = m_pDIDevKeyboard->SetCooperativeLevel(hWindow, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE));
	if (FAILED(hr))
	{
		MessageBox(hWindow, "input::Keyboard mode error", "Warning", MB_ICONWARNING);
		return hr;
	}

	// �L�[�{�[�h�ւ̃A�N�Z�X�����擾
	m_pDIDevKeyboard->Acquire();

	return S_OK;
}

void Input::UninitKeyboard()
{
	if (m_pDIDevKeyboard)
	{
		m_pDIDevKeyboard->Release();
		m_pDIDevKeyboard = NULL;
	}
}

HRESULT Input::UpadteKeyboard()
{
	HRESULT hr;
	BYTE keyStateOld[256];

	// �O��̃f�[�^��ۑ�
	memcpy(keyStateOld, m_KeyState, 256);

	// �f�o�C�X����f�[�^���擾
	hr = m_pDIDevKeyboard->GetDeviceState(sizeof(m_KeyState), m_KeyState);
	if (SUCCEEDED(hr))
	{
		for (int cnt = 0; cnt < 256; cnt++)
		{
			m_KeyTrigger[cnt] = (keyStateOld[cnt] ^ m_KeyState[cnt]) & m_KeyState[cnt];
			m_KeyRelease[cnt] = (keyStateOld[cnt] ^ m_KeyState[cnt]) & ~m_KeyState[cnt];
			m_KeyRepeat[cnt] = m_KeyTrigger[cnt];

			if (m_KeyState[cnt])
			{
				m_KeyRepeatCnt[cnt]++;
				if (m_KeyRepeatCnt[cnt] >= 20)
				{
					m_KeyRepeatCnt[cnt] = m_KeyState[cnt];
				}
			}
			else
			{
				m_KeyRepeatCnt[cnt] = 0;
				m_KeyRepeat[cnt] = 0;
			}
		}
	}
	else
	{
		// �L�[�{�[�h�ւ̃A�N�Z�X�����擾
		m_pDIDevKeyboard->Acquire();
	}

	return S_OK;
}

bool Input::GetKeyPress(BYTE KeyCode)
{
	return (m_KeyState[KeyCode] & 0x80) ? true : false;
}

bool Input::GetKeyTrigger(BYTE KeyCode)
{
	return (m_KeyTrigger[KeyCode] & 0x80) ? true : false;
}

bool Input::GetKeyRepeat(BYTE KeyCode)
{
	return (m_KeyRepeat[KeyCode] & 0x80) ? true : false;
}

bool Input::GetKeyRelease(BYTE KeyCode)
{
	return (m_KeyRelease[KeyCode] & 0x80) ? true : false;
}

BOOL Input::SearchGamePadCallback(LPDIDEVICEINSTANCE lpddi, LPVOID)
{
	HRESULT result;

	result = m_pDInput->CreateDevice(lpddi->guidInstance, &m_pGamePad[m_PadCount++], NULL);
	return DIENUM_CONTINUE;	// ���̃f�o�C�X���
}

///�}�E�X
HRESULT Input::InitMouse(HWND hWindow)
{
	HRESULT result;
	// �f�o�C�X�쐬
	result = m_pDInput->CreateDevice(GUID_SysMouse, &m_pMouse, NULL);
	if (FAILED(result) || m_pMouse == NULL)
	{
		MessageBox(hWindow, "input::No mouse", "Warning", MB_OK | MB_ICONWARNING);
		return result;
	}
	// �f�[�^�t�H�[�}�b�g�ݒ�
	result = m_pMouse->SetDataFormat(&c_dfDIMouse2);
	if (FAILED(result))
	{
		MessageBox(hWindow, "input::Can't setup mouse", "Warning", MB_OK | MB_ICONWARNING);
		return result;
	}
	// ���̃A�v���Ƌ������[�h�ɐݒ�
	result = m_pMouse->SetCooperativeLevel(hWindow, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE));
	if (FAILED(result))
	{
		MessageBox(hWindow, "input::Mouse mode error", "Warning", MB_OK | MB_ICONWARNING);
		return result;
	}

	// �f�o�C�X�̐ݒ�
	DIPROPDWORD prop;

	prop.diph.dwSize = sizeof(prop);
	prop.diph.dwHeaderSize = sizeof(prop.diph);
	prop.diph.dwObj = 0;
	prop.diph.dwHow = DIPH_DEVICE;
	prop.dwData = DIPROPAXISMODE_REL;		// �}�E�X�̈ړ��l�@���Βl

	result = m_pMouse->SetProperty(DIPROP_AXISMODE, &prop.diph);
	if (FAILED(result))
	{
		MessageBox(hWindow, "input::Mouse property error", "Warning", MB_OK | MB_ICONWARNING);
		return result;
	}

	// �}�E�X�ւ̃A�N�Z�X�����擾
	m_pMouse->Acquire();
	return result;
}

void Input::UninitMouse()
{
	if (m_pMouse)
	{
		m_pMouse->Unacquire();
		m_pMouse->Release();
		m_pMouse = NULL;
	}
}

HRESULT Input::UpdateMouse()
{
	HRESULT result;
	// �O��̒l�ۑ�
	DIMOUSESTATE2 lastMouseState = m_MouseState;
	// �f�[�^�擾
	result = m_pMouse->GetDeviceState(sizeof(m_MouseState), &m_MouseState);
	if (SUCCEEDED(result))
	{
		m_MouseTrigger.lX = m_MouseState.lX;
		m_MouseTrigger.lY = m_MouseState.lY;
		m_MouseTrigger.lZ = m_MouseState.lZ;
		// �}�E�X�̃{�^�����
		for (int i = 0; i < 8; i++)
		{
			m_MouseTrigger.rgbButtons[i] = ((lastMouseState.rgbButtons[i] ^
				m_MouseState.rgbButtons[i]) & m_MouseState.rgbButtons[i]);
		}
	}
	else	// �擾���s
	{
		// �A�N�Z�X���𓾂�
		result = m_pMouse->Acquire();
	}
	return result;
}

D3DXVECTOR2 Input::MouseRelative()
{
	D3DXVECTOR2 pos = { (float)m_MouseState.lX,(float)m_MouseState.lY };
	pos.x = pos.x / 100.f;
	pos.y = pos.y / 100.f;
	return pos;
}

///�p�b�h
HRESULT Input::InitPad(HWND hWindow)
{
	HRESULT		result;
	int			i;

	m_PadCount = 0;
	// �W���C�p�b�h��T��
	m_pDInput->EnumDevices(DI8DEVCLASS_GAMECTRL, (LPDIENUMDEVICESCALLBACK)SearchGamePadCallback, NULL, DIEDFL_ATTACHEDONLY);
	// �Z�b�g�����R�[���o�b�N�֐����A�p�b�h�𔭌������������Ă΂��B

	for (i = 0; i < m_PadCount; i++) {
		// �W���C�X�e�B�b�N�p�̃f�[�^�E�t�H�[�}�b�g��ݒ�
		result = m_pGamePad[i]->SetDataFormat(&c_dfDIJoystick);
		if (FAILED(result))
		{
			MessageBox(hWindow, "input::Can't setup Pad", "Warning", MB_OK | MB_ICONWARNING);
			return result;
		}

		// ���[�h��ݒ�i�t�H�A�O���E���h����r�����[�h�j
		result = m_pGamePad[i]->SetCooperativeLevel(hWindow, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
		if (FAILED(result) || m_pGamePad[i] == NULL)
		{
			MessageBox(hWindow, "input::Pad mode error", "Warning", MB_ICONWARNING);
			return result;
		}

		// ���̒l�͈̔͂�ݒ�
		// X���AY���̂��ꂼ��ɂ��āA�I�u�W�F�N�g���񍐉\�Ȓl�͈̔͂��Z�b�g����B
		// (max-min)�́A�ő�10,000(?)�B(max-min)/2�������l�ɂȂ�B
		DIPROPRANGE				diprg;
		ZeroMemory(&diprg, sizeof(diprg));
		diprg.diph.dwSize = sizeof(diprg);
		diprg.diph.dwHeaderSize = sizeof(diprg.diph);
		diprg.diph.dwHow = DIPH_BYOFFSET;
		diprg.lMin = RANGE_MIN;
		diprg.lMax = RANGE_MAX;
		// X���͈̔͂�ݒ�
		diprg.diph.dwObj = DIJOFS_X;
		m_pGamePad[i]->SetProperty(DIPROP_RANGE, &diprg.diph);
		// Y���͈̔͂�ݒ�
		diprg.diph.dwObj = DIJOFS_Y;
		m_pGamePad[i]->SetProperty(DIPROP_RANGE, &diprg.diph);

		// �e�����ƂɁA�����̃]�[���l��ݒ�
		DIPROPDWORD				dipdw;
		dipdw.diph.dwSize = sizeof(DIPROPDWORD);
		dipdw.diph.dwHeaderSize = sizeof(dipdw.diph);
		dipdw.diph.dwHow = DIPH_BYOFFSET;
		dipdw.dwData = DEADZONE;
		//X���̖����]�[����ݒ�
		dipdw.diph.dwObj = DIJOFS_X;
		m_pGamePad[i]->SetProperty(DIPROP_DEADZONE, &dipdw.diph);
		//Y���̖����]�[����ݒ�
		dipdw.diph.dwObj = DIJOFS_Y;
		m_pGamePad[i]->SetProperty(DIPROP_DEADZONE, &dipdw.diph);

		//�W���C�X�e�B�b�N�ւ̃A�N�Z�X�����擾
		m_pGamePad[i]->Acquire();
	}

	return true;
}

void Input::UninitPad()
{
	for (int i = 0; i < GAMEPAD_MAX; i++)
	{
		if (m_pGamePad[i])
		{
			m_pGamePad[i]->Unacquire();
			m_pGamePad[i]->Release();
			m_pGamePad[i] = NULL;
		}
	}
}

HRESULT Input::UpdatePad()
{
	HRESULT			result;
	DIJOYSTATE2		dijs;
	int				i;

	for (i = 0; i < m_PadCount; i++)
	{
		DWORD lastPadState;
		lastPadState = m_PadState[i];
		m_PadState[i] = 0x00000000l;	// ������

		result = m_pGamePad[i]->Poll();	// �W���C�X�e�B�b�N�Ƀ|�[����������
		if (FAILED(result)) {
			result = m_pGamePad[i]->Acquire();
			while (result == DIERR_INPUTLOST)
				result = m_pGamePad[i]->Acquire();
		}

		result = m_pGamePad[i]->GetDeviceState(sizeof(DIJOYSTATE), &dijs);	// �f�o�C�X��Ԃ�ǂݎ��
		if (result == DIERR_INPUTLOST || result == DIERR_NOTACQUIRED) {
			result = m_pGamePad[i]->Acquire();
			while (result == DIERR_INPUTLOST)
				result = m_pGamePad[i]->Acquire();
		}

		// �R�Q�̊e�r�b�g�ɈӖ����������A�{�^�������ɉ����ăr�b�g���I���ɂ���
		//* y-axis (forward)
		if (dijs.lY < 0)					m_PadState[i] |= BUTTON_UP;
		//* y-axis (backward)
		if (dijs.lY > 0)					m_PadState[i] |= BUTTON_DOWN;
		//* x-axis (left)
		if (dijs.lX < 0)					m_PadState[i] |= BUTTON_LEFT;
		//* x-axis (right)
		if (dijs.lX > 0)					m_PadState[i] |= BUTTON_RIGHT;
		//* �w�{�^��
		if (dijs.rgbButtons[rgbButtons_X] & 0x80)	m_PadState[i] |= BUTTON_X;
		//* �`�{�^��
		if (dijs.rgbButtons[rgbButtons_A] & 0x80)	m_PadState[i] |= BUTTON_A;
		//* �a�{�^��
		if (dijs.rgbButtons[rgbButtons_B] & 0x80)	m_PadState[i] |= BUTTON_B;
		//* �x�{�^��
		if (dijs.rgbButtons[rgbButtons_Y] & 0x80)	m_PadState[i] |= BUTTON_Y;
		//* �k�{�^��
		if (dijs.rgbButtons[rgbButtons_L] & 0x80)	m_PadState[i] |= BUTTON_L;
		//* �q�{�^��
		if (dijs.rgbButtons[rgbButtons_R] & 0x80)	m_PadState[i] |= BUTTON_R;
		//* �k2�{�^��
		if (dijs.rgbButtons[rgbButtons_L2] & 0x80)	m_PadState[i] |= BUTTON_L2;
		//* �q2�{�^��
		if (dijs.rgbButtons[rgbButtons_R2] & 0x80)	m_PadState[i] |= BUTTON_R2;
		//* Select�{�^��
		if (dijs.rgbButtons[rgbButtons_SELECT] & 0x80)	m_PadState[i] |= BUTTON_SELECT;
		//* �r�s�`�q�s�{�^��
		if (dijs.rgbButtons[rgbButtons_START] & 0x80)	m_PadState[i] |= BUTTON_START;

		// Trigger�ݒ�
		m_PadTrigger[i] = ((lastPadState ^ m_PadState[i])	// �O��ƈ���Ă���
			& m_PadState[i]);					// ��������ON�̂��
	}

	return S_OK;
}

BOOL Input::IsButtonPressed(int padNo, DWORD button)
{
	if (padNo >= 0 && padNo <= GAMEPAD_MAX)
		return (button & m_PadState[padNo]);

	return false;
}

BOOL Input::IsButtonTriggered(int padNo, DWORD button)
{
	if (padNo >= 0 && padNo <= GAMEPAD_MAX)
		return (button & m_PadTrigger[padNo]);

	return false;
}

int Input::GetPadCount()
{
	return m_PadCount;
}

DIJOYSTATE Input::GetJoyState(int padnum)
{
	if (m_PadCount > 0)
	{
		if (padnum >= 0 && padnum <= GAMEPAD_MAX)
			if (m_pGamePad[padnum] != NULL)
			{
				m_pGamePad[padnum]->GetDeviceState(sizeof(DIJOYSTATE), &m_JoyState[padnum]);
				return m_JoyState[padnum];
			}
	}
	return DIJOYSTATE();
}

DIJOYSTATE2 Input::GetJoyState2(int padnum)
{
	if (m_PadCount > 0)
	{
		if (padnum >= 0 && padnum <= GAMEPAD_MAX)
			if (m_pGamePad[padnum] != NULL)
			{
				m_pGamePad[padnum]->GetDeviceState(sizeof(DIJOYSTATE), &m_JoyState2[padnum]);
				return m_JoyState2[padnum];
			}
	}
	return DIJOYSTATE2();
}

D3DXVECTOR2 Input::GetJoyPadRight(int padnum)
{
	if (m_PadCount > 0)
	{
		if (padnum >= 0 && padnum <= GAMEPAD_MAX)
			if (m_pGamePad[padnum] != NULL)
			{
				m_pGamePad[padnum]->GetDeviceState(sizeof(DIJOYSTATE), &m_JoyState[padnum]);
				D3DXVECTOR2 JoyStick(m_JoyState[padnum].lZ - (65535 / 2), m_JoyState[padnum].lRz - (65535 / 2));
				float unresponsive_range = 6553.6;

				// �X���̔䗦�𒲂ׂ���@
				float length = 65535 / 2; // ���_����ŏ��A�ő�܂ł̒���

				if (JoyStick.y<unresponsive_range && JoyStick.y >-unresponsive_range)
				{
					JoyStick.y = 0.0f;
				}
				else
				{
					JoyStick.y = JoyStick.y / length;
				}

				if (JoyStick.x <unresponsive_range && JoyStick.x > -unresponsive_range)
				{
					JoyStick.x = 0.0f;
				}
				else
				{
					JoyStick.x = JoyStick.x / length;
				}

				return JoyStick;
			}
	}
	return Float2(0.f, 0.f);
};
D3DXVECTOR2 Input::GetJoyPadLeft(int padnum)
{
	if (m_PadCount > 0)
	{
		if (padnum >= 0 && padnum <= GAMEPAD_MAX)
			if (m_pGamePad[padnum] != NULL)
			{
				m_pGamePad[padnum]->GetDeviceState(sizeof(DIJOYSTATE), &m_JoyState[padnum]);
				D3DXVECTOR2 JoyStick(m_JoyState[padnum].lX, m_JoyState[padnum].lY);
				float unresponsive_range = 200;

				// �X���̔䗦�𒲂ׂ���@
				float length = 1000; // ���_����ŏ��A�ő�܂ł̒���

				if (JoyStick.y<unresponsive_range && JoyStick.y >-unresponsive_range)
				{
					JoyStick.y = 0.0f;
				}
				else
				{
					JoyStick.y = JoyStick.y / length;
				}

				if (JoyStick.x <unresponsive_range && JoyStick.x > -unresponsive_range)
				{
					JoyStick.x = 0.0f;
				}
				else
				{
					JoyStick.x = JoyStick.x / length;
				}

				return JoyStick;
			}
	}
	return Float2(0.f, 0.f);
};

float		Input::GetL2(int padnum)
{
	if (m_PadCount > 0)
	{
		if (padnum >= 0 && padnum <= GAMEPAD_MAX)
			if (m_pGamePad[padnum] != NULL)
			{
				float L2 = GetJoyState(padnum).lRx;
				float Return = 0.0f;

				Return = L2 / 65535;

				return Return;
			}
	}
};

float		Input::GetR2(int padnum)
{
	if (m_PadCount > 0)
	{
		if (padnum >= 0 && padnum <= GAMEPAD_MAX)
			if (m_pGamePad[padnum] != NULL)
			{
				float R2 = GetJoyState(padnum).lRy;
				float Return = 0.0f;

				Return = R2 / 65535;

				return Return;
			}
	}
};