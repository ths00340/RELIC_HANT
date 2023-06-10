//==============================================================================
// Filename: input.h
// Description: ���͗p�N���X�̒�`
//==============================================================================
#pragma once
#pragma warning( disable : 4244 )//�x���΍�
#include "main.h"

#define GAMEPAD_MAX (4)

/* game pad��� */
#define BUTTON_UP		0x00000001l	// �����L�[��(.IY<0)
#define BUTTON_DOWN		0x00000002l	// �����L�[��(.IY>0)
#define BUTTON_LEFT		0x00000004l	// �����L�[��(.IX<0)
#define BUTTON_RIGHT	0x00000008l	// �����L�[�E(.IX>0)

#define BUTTON_X		0x00000010l	// �w�{�^��(.rgbButtons[0]&0x80)
#define BUTTON_A		0x00000020l	// �`�{�^��(.rgbButtons[1]&0x80)
#define BUTTON_B		0x00000040l	// �a�{�^��(.rgbButtons[2]&0x80)
#define BUTTON_Y		0x00000080l	// �x�{�^��(.rgbButtons[3]&0x80)
#define BUTTON_L		0x00000100l	// �k�{�^��(.rgbButtons[4]&0x80)
#define BUTTON_R		0x00000200l	// �q�{�^��(.rgbButtons[5]&0x80)
#define BUTTON_L2		0x00000400l	// �k�{�^��(.rgbButtons[6]&0x80)
#define BUTTON_R2		0x00000800l	// �q�{�^��(.rgbButtons[7]&0x80)
#define BUTTON_SELECT	0x00001000l	// �y�{�^��(.rgbButtons[8]&0x80)
#define BUTTON_START	0x00002000l	// �r�s�`�q�s�{�^��(.rgbButtons[9]&0x80)

enum {	// �{�^���ɑ΂���rgbButtons[?]�z��̔z��ԍ�(DirectInput��ł�XYAB�{�^���ʒu�ɍ��킹��)
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
	static LPDIRECTINPUT8	m_pDInput;		// IDirectInput8�C���^�[�t�F�[�X�ւ̃|�C���^

	//�L�[�{�[�h
	static LPDIRECTINPUTDEVICE8 m_pDIDevKeyboard;
	static BYTE m_KeyState[256];//���݂̏��
	static BYTE m_KeyTrigger[256];//�����ꂽ���̎擾
	static BYTE m_KeyRepeat[256];//������
	static BYTE m_KeyRelease[256];//��������
	static BYTE m_KeyRepeatCnt[256];//���s�[�g��

	//�}�E�X
	static LPDIRECTINPUTDEVICE8 m_pMouse; // mouse
	static DIMOUSESTATE2   m_MouseState;	// �}�E�X�̃_�C���N�g�ȏ��
	static DIMOUSESTATE2   m_MouseTrigger;	// �����ꂽ�u�Ԃ���ON
	static POINT po;
	static BOOL m_isMidPointer;

	//�p�b�h
	static LPDIRECTINPUTDEVICE8 m_pGamePad[GAMEPAD_MAX];
	static DWORD	m_PadState[GAMEPAD_MAX];	// �p�b�h���i�����Ή��j
	static DWORD	m_PadTrigger[GAMEPAD_MAX];
	static DIJOYSTATE2	m_JoyState2[GAMEPAD_MAX];
	static DIJOYSTATE	m_JoyState[GAMEPAD_MAX];
	static int		m_PadCount;			// ���o�����p�b�h�̐�

	static BOOL CALLBACK SearchGamePadCallback(LPDIDEVICEINSTANCE lpddi, LPVOID);

	//������
	static HRESULT InitMouse(HWND hWindow);
	static HRESULT InitKeyboard(HWND hWindow);
	static HRESULT InitPad(HWND hWindow);

	//�I������
	static void UninitMouse();
	static void UninitKeyboard();
	static void UninitPad();

	//�X�V����
	static HRESULT UpdateMouse();
	static HRESULT UpadteKeyboard();
	static HRESULT UpdatePad();

public:
	static void Init();
	static void Uninit();
	static void Update();

	//�L�[�{�[�h��������Ă���Ԃ͐�
	static bool GetKeyPress(BYTE KeyCode);

	//�L�[�{�[�h�������ꂽ�u�Ԃ̂ݐ�
	static bool GetKeyTrigger(BYTE KeyCode);

	//�L�[�{�[�h�̘A������
	static bool GetKeyRepeat(BYTE KeyCode);

	//�L�[�{�[�h���������u�Ԃ̂ݐ�
	static bool GetKeyRelease(BYTE KeyCode);

	//�J�[�\���̌��݈ʒu�̎擾(���オ0,0)
	static D3DXVECTOR2 GetCursor();

	//�J�[�\���̕\��/��\��
	static void ShowPoint(bool Show);

	//�}�E�X�̍��N���b�N��������Ă���Ԃ͐�
	static BOOL IsMouseLeftPressed(void)
	{
		return (BOOL)(m_MouseState.rgbButtons[0] & 0x80);	// �����ꂽ�Ƃ��ɗ��r�b�g������
	}

	//�}�E�X�̍��N���b�N��������Ă���u�Ԃ̂ݐ�
	static BOOL IsMouseLeftTriggered(void)
	{
		return (BOOL)(m_MouseTrigger.rgbButtons[0] & 0x80);
	}

	//�}�E�X�̉E�N���b�N��������Ă���Ԃ͐�
	static BOOL IsMouseRightPressed(void)
	{
		return (BOOL)(m_MouseState.rgbButtons[1] & 0x80);
	}

	//�}�E�X�̉E�N���b�N��������Ă���u�Ԃ̂ݐ�
	static BOOL IsMouseRightTriggered(void)
	{
		return (BOOL)(m_MouseTrigger.rgbButtons[1] & 0x80);
	}

	//�}�E�X�̃z�C�[����������Ă���Ԃ͐�
	static BOOL IsMouseCenterPressed(void)
	{
		return (BOOL)(m_MouseState.rgbButtons[2] & 0x80);
	}

	//�}�E�X�̃z�C�[����������Ă���u�Ԃ̂ݐ�
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
