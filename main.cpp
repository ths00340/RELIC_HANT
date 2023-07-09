#include "main.h"
#include "manager.h"
#include "resource.h"
#include "Win_Window.h"

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	/// �E�B���h�E�̐���
	if (!WinCreateSimpleWindow(hInstance, nCmdShow, SCREEN_WIDTH, SCREEN_HEIGHT))
	{
		return false;
	}

	Manager::Init();

	DWORD dwExecLastTime;
	DWORD dwCurrentTime;
	timeBeginPeriod(1);
	dwExecLastTime = timeGetTime();
	dwCurrentTime = 0;

	while (!WinIsQuitMessage())
	{
		//���b�Z�[�W�̍X�V
		if (!WinUpdateWindowMessage())
		{
			dwCurrentTime = timeGetTime();

			if ((dwCurrentTime - dwExecLastTime) >= ((float)1000 / 60.f))
			{
				dwExecLastTime = dwCurrentTime;

				Manager::Update();
				Manager::Draw();
			}
		}
	}

	WinUninitWindow();

	Manager::Uninit();

	_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_DEBUG);
	//_CrtDumpMemoryLeaks();

	return 0;
}