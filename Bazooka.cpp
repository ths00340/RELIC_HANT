#include "main.h"
#include "renderer.h"
#include "Scene.h"
#include "manager.h"
#include "model.h"
#include "GameObject.h"
#include "Bazooka.h"
#include "Tools.h"
#include "Camera.h"
#include "ExplosiveBullet.h"
#include "Status.h"

void Bazooka::Predicted(Float3 spos, Float3 vel)
{
	Float3 pos = spos;
	while (pos.y > 0.75f)
	{
		pos += vel;
		vel.y -= GRAVITY;
	}
	pos.y = 0.0f;
	PredictedPoint = pos;
}

void Bazooka::Init()
{
	model = ResourceManager::AddModel("asset\\models\\Bazooka_01.obj");
	//シェーダー関係
	ResourceManager::GetShaderState(&m_VertexShader, &m_PixelShader, &m_VertexLayout, SHADER_S::NORMAL_FOG);
	fire_rate = 0.75f;
	dmg = 20;
}

void Bazooka::Uninit()
{
	if (model)
		model = nullptr;

	if (m_VertexLayout)
		m_VertexLayout = nullptr;

	if (m_VertexShader)
		m_VertexShader = nullptr;

	if (m_PixelShader)
		m_PixelShader = nullptr;
}

void Bazooka::Update()
{
	time += Renderer::GetDeltaTime();
	Camera* cam = object->LoadComponent<Camera>();
	m_scl = object->Getscl();

	Scene* scene = Manager::GetScene();

	objS = object->LoadComponent<Status>();

	if (objS)
	{
		float test = TOOL::AToR(80.f) * -((float)objS->GetWheel() / 50.f);

		angle = -TOOL::AToR(80.f) * test;
		angle = TOOL::Limit(angle, 0.f, -TOOL::AToR(80.0f));
	}

	ShotAngle = m_rot;

	if (cam->GetMode() == CamMode::FPP)
	{
		ShotAngle = object->Getrot();
		ShotAngle.x += TOOL::AToR(20);
	}

	Float3 add = object->Getaddrot();
	ShotAngle.x += angle + add.x;

	if (cam->GetMode() == CamMode::TPP)
		ShotAngle.y = object->LoadComponent<Camera>()->GetAngle().y;
	else
		ShotAngle.y += add.y;

	ShotAngle.z += add.z;

	shotpos = (object->Getpos() + TOOL::GetForward(ShotAngle) * (model->Get_max().z * m_scl.z));
	shotpos += TOOL::GetUp(ShotAngle) * ((model->Get_max().y * m_scl.y) + (2.5f * m_scl.y)) + object->Getvec();

	if (objS != NULL)
		if (objS->GetShot() && time > fire_rate)
		{
			time = 0.f;

			ExplosiveBullet* blt = scene->AddGameObject<ExplosiveBullet>((int)OBJ_LAYER::GameObject);
			blt->Set(shotpos, ShotAngle, TOOL::SecDiv(30.0f), dmg, 15.f * object->Getscl().z);
			blt->SetScl(TOOL::Uniform(2.0f * m_scl.z));
		}

	Predicted(shotpos, TOOL::GetForward(ShotAngle) * TOOL::SecDiv(30));
}

void Bazooka::Draw()
{
	Camera* cam = object->LoadComponent<Camera>();
	//入力レイアウト設定
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	//シェーダー入力
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	//マトリクス設定
	D3DXMATRIX world, scl, rot, trans;
	D3DXMatrixScaling(&scl, m_scl.x, m_scl.y, m_scl.z);

	if (cam->GetMode() == CamMode::TPP)
		D3DXMatrixRotationYawPitchRoll(&rot,
			cam->GetAngle().y,
			object->Getrot().x + object->Getaddrot().x + angle,
			object->Getrot().z + object->Getaddrot().z);
	else
		D3DXMatrixRotationYawPitchRoll(&rot,
			object->Getrot().y + object->Getaddrot().y,
			object->Getrot().x + object->Getaddrot().x,
			object->Getrot().z + object->Getaddrot().z);

	Float3 TrueF3 = TOOL::GetUp(
		Float3(object->Getrot().x + object->Getaddrot().x,
			object->Getrot().y + object->Getaddrot().y,
			object->Getrot().z + object->Getaddrot().z))
		* (object->Getmax().y * object->Getscl().y);

	D3DXMatrixTranslation(&trans, object->Getpos().x + TrueF3.x, object->Getpos().y + TrueF3.y, object->Getpos().z + TrueF3.z);
	world = scl * rot * trans;
	Renderer::SetWorldMatrix(&world);
	model->Draw();
}

void Bazooka::Load()
{
	ResourceManager::AddModel("asset\\models\\Bazooka_01.obj");
	ResourceManager::AddModel("asset\\models\\hiteff.obj");
}