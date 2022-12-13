//==============================================================================
// Filename: main.h
// Description: ���C���w�b�_
//==============================================================================
#pragma once

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <windows.h>
#include <assert.h>

#pragma warning(push)
#pragma warning(disable:4005)

#include <d3d11.h>
#include <d3dx9.h>
#include <d3dx11.h>
#define DIRECTINPUT_VERSION 0x0800		// �x���Ώ�
#include "dinput.h"
#include "mmsystem.h"

#pragma warning(pop)

#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dcompiler.lib")
#pragma comment (lib, "d3dx11.lib")
#pragma comment (lib, "d3dx9.lib")
#pragma comment (lib, "winmm.lib")
#pragma comment (lib, "dxerr.lib")
#pragma comment (lib, "dxguid.lib")
#pragma comment (lib, "dinput8.lib")

#define SCREEN_WIDTH	(1920)
#define SCREEN_HEIGHT	(1080)

HWND GetWindow();
HINSTANCE GetInstance();

typedef D3DXCOLOR	COLOR;
typedef D3DXVECTOR3 Float3;
typedef D3DXVECTOR2 Float2;
typedef D3DXQUATERNION Qua;

#define MUTE//�R�����g�A�E�g����ƃ~���[�g����
