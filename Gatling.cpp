#include "main.h"
#include "renderer.h"
#include "audio.h"
#include "manager.h"
#include "GameObject.h"
#include "Tools.h"
#include "Camera.h"
#include "Status.h"
#include "bullet.h"
#include "Gatling.h"

void Gatling::Init()
{
	maxspednum = 50;
	model = ResourceManager::AddModel("asset\\models\\Tullet05ho.obj");
	barrel = ResourceManager::AddModel("asset\\models\\Tullet05barrel.obj");
	predictionLine = ResourceManager::AddModel("asset\\models\\laser01.obj");
	//シェーダー関係
	ResourceManager::GetShaderState(&m_VertexShader, &m_PixelShader, &m_VertexLayout, SHADER_S::NORMAL_FOG);
	fire_time = 0.30f;
	roll_max = TOOL::AToR(60);

#ifndef MUTE
	shot = Manager::GetScene()->AddGameObject<Audio>((int)OBJ_LAYER::System);
	shot->Load("asset\\SE\\ShotSound_2.wav");
#endif // MUTE
}

void Gatling::Uninit() 
{
	if (model)
		model = nullptr;

	if (barrel)
		barrel = nullptr;

	if (predictionLine)
		predictionLine = nullptr;

	if (m_VertexLayout)
		m_VertexLayout = nullptr;

	if (m_VertexShader)
		m_VertexShader = nullptr;

	if (m_PixelShader)
		m_PixelShader = nullptr;

#ifndef MUTE
	shot->Destroy();
	shot = nullptr;
#endif // MUTE
}

void Gatling::Update()
{
	objS = object->LoadComponent<Status>();

	Camera* cam = object->LoadComponent<Camera>();
	m_scl = object->Getscl();
	time += Renderer::GetDeltaTime();
	Scene* scene = Manager::GetScene();

	angle = cam->GetAngle().x;
	angle = TOOL::Limit(angle, 0.0f, -TOOL::AToR(60.0f));

	int fire = fire_time * fire_rate;

	if (objS)
	{
		ShotAngle = m_rot;

		if (cam->GetMode() == CamMode::FPP)
			ShotAngle = object->Getrot();

		Float3 add = object->Getaddrot();
		ShotAngle.x += angle + add.x;

		if (cam->GetMode() == CamMode::TPP)
			ShotAngle.y = object->LoadComponent<Camera>()->GetAngle().y;
		else
			ShotAngle.y += add.y;

		ShotAngle.z += add.z;

		shotpos = (object->Getpos() + TOOL::GetForward(ShotAngle) * ((barrel->Get_max().z * m_scl.z) + (model->Get_max().z * m_scl.z)));
		shotpos += TOOL::GetUp(ShotAngle) * ((model->Get_max().y * m_scl.y) + (3.5f * m_scl.y)) + object->Getvec();

		if (objS->GetShot() && time > fire)
		{
#ifndef MUTE
			int a;
			a = shot->Play();
			shot->SetVolume(0.05f, a);
#endif
			time = 0.f;

			randrot = Float3(0.0f, 0.0f, 0.0f);
			randrot.x = ((TOOL::RandF() * TOOL::AToR(5.f)) - TOOL::AToR(2.5f)) * (1.f - fire_rate);
			randrot.y = ((TOOL::RandF() * TOOL::AToR(5.f)) - TOOL::AToR(2.5f)) * (1.f - fire_rate);
			Bullet* blt = scene->AddGameObject<Bullet>((int)OBJ_LAYER::GameObject);
			blt->Set(shotpos, ShotAngle + randrot, TOOL::SecDiv(200.0f), 1, range);
			blt->SetScl(TOOL::Uniform(2.0f * m_scl.z));

			shotnum++;
		}

		if (!objS->GetShot())
		{
			shotnum--;
			shotnum = max(0, shotnum);
		}

		shotnum = (int)TOOL::Limit(shotnum, maxspednum, 0);
		fire_rate = (float)shotnum / (float)maxspednum;

		fire_rate = 1.0f - (rate_min * TOOL::Limit(fire_rate, 1.0f, 0.0f));

		float roll = roll_max * (1.f - fire_rate);
		barrelRoll += roll;
		if (barrelRoll >= TOOL::AToR(360))
			barrelRoll = 0.0f;
	}

	Float3 ShotAngle = m_rot;
	Float3 add = object->Getaddrot();
	ShotAngle.x += angle + add.x;

	if (cam->GetMode() == CamMode::TPP)
		ShotAngle.y = object->LoadComponent<Camera>()->GetAngle().y;
	else
		ShotAngle.y += add.y;

	ShotAngle.z += add.z;
	Float3 bpos = (object->Getpos() + TOOL::GetForward(ShotAngle) * (model->Get_max().z * m_scl.z));
	bpos += TOOL::GetUp(ShotAngle) * ((model->Get_max().y * m_scl.y) - (1.f * m_scl.y)) + object->Getvec();
	m_barrelpos = bpos;
}

void Gatling::Draw()
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

	D3DXMatrixRotationYawPitchRoll(&rot,
		cam->GetAngle().y,
		object->Getrot().x + object->Getaddrot().x + angle,
		object->Getrot().z + object->Getaddrot().z + barrelRoll);

	TrueF3 = TOOL::GetUp(
		Float3(object->Getrot().x + object->Getaddrot().x,
			object->Getrot().y + object->Getaddrot().y,
			object->Getrot().z + object->Getaddrot().z))
		* (object->Getmax().y * object->Getscl().y);

	D3DXMatrixTranslation(&trans, m_barrelpos.x + TrueF3.x, m_barrelpos.y + TrueF3.y, m_barrelpos.z + TrueF3.z);
	world = scl * rot * trans;
	Renderer::SetWorldMatrix(&world);
	barrel->Draw();

	D3DXMatrixScaling(&scl, m_scl.x, m_scl.y, TOOL::Magni_one(predictionLine->Get_max().z, range));
	D3DXMatrixRotationYawPitchRoll(&rot, ShotAngle.y, ShotAngle.x, ShotAngle.z);
	D3DXMatrixTranslation(&trans, shotpos.x, shotpos.y, shotpos.z);
	world = scl * rot * trans;
	Renderer::SetWorldMatrix(&world);

	if (objS)
		predictionLine->Draw();
}

void Gatling::Load()
{
	ResourceManager::AddModel("asset\\models\\Tullet05ho.obj");
	ResourceManager::AddModel("asset\\models\\Tullet05barrel.obj");
	ResourceManager::AddModel("asset\\models\\laser01.obj");
}