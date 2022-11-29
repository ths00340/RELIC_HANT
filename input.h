//==============================================================================
// Filename: input.h
// Description: ���͗p�N���X�̒�`
//==============================================================================
#pragma once
#pragma warning( disable : 4244 )//�x���΍�
#include "main.h"
class Input
{
private:
	static BYTE m_OldKeyState[256];
	static BYTE m_KeyState[256];
	static POINT po;
	static LPDIRECTINPUTDEVICE8 pMouse; // mouse
	static DIMOUSESTATE2   mouseState;		// �}�E�X�̃_�C���N�g�ȏ��
	static DIMOUSESTATE2   mouseTrigger;	// �����ꂽ�u�Ԃ���ON
	static LPDIRECTINPUT8	g_pDInput;		// IDirectInput8�C���^�[�t�F�[�X�ւ̃|�C���^
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
		// �O��̒l�ۑ�
		DIMOUSESTATE2 lastMouseState = mouseState;
		// �f�[�^�擾
		result = pMouse->GetDeviceState(sizeof(mouseState), &mouseState);
		if (SUCCEEDED(result))
		{
			mouseTrigger.lX = mouseState.lX;
			mouseTrigger.lY = mouseState.lY;
			mouseTrigger.lZ = mouseState.lZ;
			// �}�E�X�̃{�^�����
			for (int i = 0; i < 8; i++)
			{
				mouseTrigger.rgbButtons[i] = ((lastMouseState.rgbButtons[i] ^
					mouseState.rgbButtons[i]) & mouseState.rgbButtons[i]);
			}
		}
		else	// �擾���s
		{
			// �A�N�Z�X���𓾂Ă݂�
			result = pMouse->Acquire();
		}
		return result;
	}

public:
	static void Init();
	static void Uninit();
	static void Update();

	//�L�[�{�[�h��������Ă���Ԃ͐�
	static bool GetKeyPress(BYTE KeyCode);

	//�L�[�{�[�h�������ꂽ�u�Ԃ̂ݐ�
	static bool GetKeyTrigger(BYTE KeyCode);

	//�J�[�\���̌��݈ʒu�̎擾(���オ0,0)
	static D3DXVECTOR2 GetCursor();

	//�J�[�\���̕\��/��\��
	static void ShowPoint(bool Show);

	//�}�E�X�̍��N���b�N��������Ă���Ԃ͐�
	static BOOL IsMouseLeftPressed(void)
	{
		return (BOOL)(mouseState.rgbButtons[0] & 0x80);	// �����ꂽ�Ƃ��ɗ��r�b�g������
	}

	//�}�E�X�̍��N���b�N��������Ă���u�Ԃ̂ݐ�
	static BOOL IsMouseLeftTriggered(void)
	{
		return (BOOL)(mouseTrigger.rgbButtons[0] & 0x80);
	}

	//�}�E�X�̉E�N���b�N��������Ă���Ԃ͐�
	static BOOL IsMouseRightPressed(void)
	{
		return (BOOL)(mouseState.rgbButtons[1] & 0x80);
	}

	//�}�E�X�̉E�N���b�N��������Ă���u�Ԃ̂ݐ�
	static BOOL IsMouseRightTriggered(void)
	{
		return (BOOL)(mouseTrigger.rgbButtons[1] & 0x80);
	}

	//�}�E�X�̃z�C�[����������Ă���Ԃ͐�
	static BOOL IsMouseCenterPressed(void)
	{
		return (BOOL)(mouseState.rgbButtons[2] & 0x80);
	}

	//�}�E�X�̃z�C�[����������Ă���u�Ԃ̂ݐ�
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
