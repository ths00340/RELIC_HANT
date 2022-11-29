#include "main.h"
#include "renderer.h"
#include "input.h"
#include "audio.h"
#include "Scene.h"
#include "manager.h"
#include "model.h"
#include "GameObject.h"
#include "Tools.h"
#include "Camera.h"
#include "ExplosiveBullet.h"
#include "Status.h"
#include "bullet.h"
#include "ShotGun_Physics.h"

Model* ShotGun_Physics::model;
Model* ShotGun_Physics::barrel;
Model* ShotGun_Physics::predictionLine;
ID3D11VertexShader* ShotGun_Physics::m_VertexShader;
ID3D11PixelShader* ShotGun_Physics::m_PixelShader;
ID3D11InputLayout* ShotGun_Physics::m_VertexLayout;

void ShotGun_Physics::Init()
{
	fire_time = TOOL::FrameMulti(0.45f);
#ifndef MUTE
	shot = Manager::GetScene()->AddGameObject<Audio>((int)OBJ_LAYER::System);
	shot->Load("asset\\SE\\ShotSound_2.wav");
#endif // MUTE
	range = 30;
}

void ShotGun_Physics::Uninit()
{
}

void ShotGun_Physics::Update()
{
	objS = object->LoadComponent<Status>();

	Camera* cam = object->LoadComponent<Camera>();
	m_scl = object->Getscl();
	time++;
	Scene* scene = Manager::GetScene();
	angle = cam->GetAngle().x;
	angle = TOOL::Limit(angle, 0.0f, -TOOL::AToR(60.0f));

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

		if (objS->GetShot() && time > fire_time)
		{
#ifndef MUTE
			int a;
			a = shot->Play();
			shot->SetVolume(0.05f, a);
#endif //MUTE
			time = 0;

			for (int i = 0; i < 10; i++)
			{
				randrot = Float3(0.0f, 0.0f, 0.0f);
				randrot.x = ((TOOL::RandF() * TOOL::AToR(7.f)) - TOOL::AToR(3.5f));
				randrot.y = ((TOOL::RandF() * TOOL::AToR(7.f)) - TOOL::AToR(3.5f));
				Bullet* blt = scene->AddGameObject<Bullet>((int)OBJ_LAYER::GameObject);
				blt->Set(shotpos, ShotAngle + randrot, TOOL::SecDiv(200.0f), dmg, range);
				blt->SetScl(TOOL::Uniform(4.0f * m_scl.z));

				if (object->LoadComponent<Camera>())
					object->LoadComponent<Camera>()->SetShake(10, 0.25f);
			}
			shotnum++;
		}
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

void ShotGun_Physics::Draw()
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

void ShotGun_Physics::Load()
{
	model = Manager::AddModel("asset\\models\\Tullet05ho.obj");
	barrel = Manager::AddModel("asset\\models\\Tullet06.obj");
	predictionLine = Manager::AddModel("asset\\models\\laser01.obj");
	//シェーダー関係
	Manager::GetShaderState(&m_VertexShader, &m_PixelShader, &m_VertexLayout, SHADER_S::NORMAL_FOG);
}