#include "DebugCamMove.h"
#include "main.h"
#include "renderer.h"
#include "input.h"
#include "manager.h"
#include "Tools.h"
#include "Camera.h"
#include "ViewCamera.h"

void DebugCamMove::Init()
{
	Camera* cam;
	cam = AddComponent<Camera>();
	cam->SetMode(CamMode::FPP);
	Scene* scene = Manager::GetScene();
	scene->GetGameObject<ViewCamera>()->SetView(cam);
	scene->SetAllStop();
	SetStop(false);
}

void DebugCamMove::Uninit()
{
	Scene* scene = Manager::GetScene();

	if (parent)
		if (parent->LoadComponent<Camera>())
			if (scene->GetGameObject<ViewCamera>())
				scene->GetGameObject<ViewCamera>()->SetView(parent->LoadComponent<Camera>());

	scene->SetAllStop(false);
}

void DebugCamMove::Update()
{
	if (Input::GetKeyPress(VK_SPACE))
	{
		m_pos.y += TOOL::SecDiv(4.0f);
	}
	else if (Input::GetKeyPress(VK_SHIFT))
	{
		m_pos.y -= TOOL::SecDiv(4.0f);
	}
	if (Input::GetKeyPress('W'))
	{
		m_pos += GetForward() * TOOL::SecDiv(4.0f);
	}
	else if (Input::GetKeyPress('S'))
	{
		m_pos -= GetForward() * TOOL::SecDiv(4.0f);
	}

	if (Input::GetKeyPress('A'))
	{
		m_pos -= GetSide() * TOOL::SecDiv(4.0f);
	}
	else if (Input::GetKeyPress('D'))
	{
		m_pos += GetSide() * TOOL::SecDiv(4.0f);
	}
	m_rot.y += ((Input::GetCursor().x - (SCREEN_WIDTH / 2)) * 0.045f) / 60.0f;
	m_rot.x += ((Input::GetCursor().y - (SCREEN_HEIGHT / 2)) * 0.045f) / 60.0f;

	//角度制限
	if (fabsf(m_rot.x) > TOOL::AToR(60))
		if (m_rot.x > 0)
			m_rot.x = TOOL::AToR(60);
		else
			m_rot.x = -TOOL::AToR(60);
}

void DebugCamMove::FixedUpdate()
{
}

void DebugCamMove::Draw()
{
	//マトリクス設定
	D3DXMATRIX world, scl, rot, trans;
	D3DXMatrixScaling(&scl, m_scl.x, m_scl.y, m_scl.z);
	D3DXMatrixRotationYawPitchRoll(&rot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixTranslation(&trans, m_pos.x, m_pos.y, m_pos.z);
	world = scl * rot * trans;
	Renderer::SetWorldMatrix(&world);
}