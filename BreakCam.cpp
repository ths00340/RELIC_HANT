#include "BreakCam.h"
#include "main.h"
#include "renderer.h"
#include "input.h"
#include "manager.h"
#include "Tools.h"
#include "Camera.h"
#include "ViewCamera.h"

void BreakCam::Init()
{
	name = "BreakCam";

	cam = AddComponent<Camera>();
	cam->SetMode(CamMode::FREE);
	Scene* scene = Manager::GetScene();
	scene->GetGameObject<ViewCamera>()->SetView(cam);
}

void BreakCam::Uninit()
{}

void BreakCam::Update()
{}

void BreakCam::FixedUpdate()
{}

void BreakCam::Draw()
{
	//マトリクス設定
	D3DXMATRIX world, scl, rot, trans;
	D3DXMatrixScaling(&scl, m_scl.x, m_scl.y, m_scl.z);
	D3DXMatrixRotationYawPitchRoll(&rot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixTranslation(&trans, m_pos.x, m_pos.y, m_pos.z);
	world = scl * rot * trans;
	Renderer::SetWorldMatrix(&world);
}

void BreakCam::Set(Float3 inPos, Float3 inUp, Float3 inTag)
{
	cam->SetPos(inPos);
	cam->SetUp(inUp);
	cam->SetTag(inTag);
}
