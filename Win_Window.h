//==============================================================================
// Filename: Win_Window.h
// Description: �E�B���h�E�쐬�w�b�_�[
//==============================================================================

#pragma once

#include <windows.h>

//------------------------------------------------------------------------------
/// �V���v���ȃE�B���h�E�̍쐬
/// param [in] hInstance	�C���X�^���X�n���h��
/// param [in] nCmdShow		�E�B���h�E�̕\�����@
/// param [in] Width		�E�B���h�E�̉���
/// param [in] Height		�E�B���h�E�̏c��
///
/// /return void
//------------------------------------------------------------------------------
bool WinCreateSimpleWindow(
	/*[in]*/ HINSTANCE hInstance,
	/*[in]*/ int nCmdShow,
	/*[in]*/ int Width,
	/*[in]*/ int Height
);

/// ���b�Z�[�W�̍X�V
bool WinUpdateWindowMessage(void);

/// �I���ʒm�����Ă��邩�H
bool WinIsQuitMessage(void);

/// �E�B���h�E�n���h���̎擾
HWND WinGetWindowHandle(void);

/// �C���X�^���X�̎擾
HINSTANCE WinGetInstance();

bool WinUninitWindow(void);
