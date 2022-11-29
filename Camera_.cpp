#include "Tools.h"
#include "input.h"
#include "Camera_.h"

void Camera_::Init()
{
	m_pos = Float3(-3.0f, 3.0f, 8.0f);
	m_tag = Float3(0.0f, -1.0f, 0.0f);
	m_up = Float3(0.0f, 1.0f, 0.0f);
	Dir = Float3(0.0f, 0.0f, 0.0f);
	Angle = Float3(0.0f, 0.0f, 0.0f);
	ViewAng = 1.0f;
	Mode = CamMode::TPP;
	name = "Camera";
	{
		{
			Nextpos[(int)MENU::GARAGE] = Float3(3.f, 3.f, 8.0f);
			Nexttag[(int)MENU::GARAGE] = Float3(11.f, -2.f, 0.0f);
		}

		{
			Nextpos[(int)MENU::STAGE] = Float3(-3.f, 3.f, 8.0f);
			Nexttag[(int)MENU::STAGE] = Float3(0.f, -2.f, 0.0f);
		}
		{
			Nextpos[(int)MENU::SHOP] = Float3(-9.f, 3.f, 8.0f);
			Nexttag[(int)MENU::SHOP] = Float3(-6.f, -2.f, 0.0f);
		}
	}
}

void Camera_::Uninit()
{
}

void Camera_::Update()
{
	time++;
	m_pos = TOOL::Float3_Larp(m_pos, Nextpos[menu], (float)time / 60.f);
	m_tag = TOOL::Float3_Larp(m_tag, Nexttag[menu], (float)time / 60.f);

	time = TOOL::Limit(time, 60, 0);
}

void Camera_::Draw()
{
	D3DXMatrixLookAtLH(&m_ViewMatrix, &m_pos, &m_tag, &m_up);

	Renderer::SetViewMatrix(&m_ViewMatrix);

	D3DXMATRIX proMatrix;
	D3DXMatrixPerspectiveFovLH(&proMatrix, ViewAng, (float)SCREEN_WIDTH / SCREEN_HEIGHT, 1.0f, 1000.0f);
	Renderer::SetProjectionMatrix(&proMatrix);
	Renderer::SetCameraPosition(m_pos);
}

void Camera_::Addmenu(int in)
{
	menu += in;
	menu = TOOL::Limit(menu, (int)MENU::SHOP - 1, (int)MENU::GARAGE);
	time = 0;
}