#include "main.h"
#include "renderer.h"
#include "model.h"
#include "GameObject.h"
#include "Enemy.h"
#include "CarMove.h"
#include "Camera.h"
#include "audio.h"
#include "ViewCamera.h"
#include "manager.h"
#include "Player.h"
#include "ParticleManager.h"
#include "Leg_01.h"
#include "Bazooka.h"
#include "HitBox.h"
#include "Gravity.h"
#include "SphereShadow.h"

Model* Enemy::m_model;
ID3D11VertexShader* Enemy::m_VertexShader;
ID3D11PixelShader* Enemy::m_PixelShader;
ID3D11InputLayout* Enemy::m_VertexLayout;
ID3D11BlendState* Enemy::blendState = NULL;

void Enemy::Init()
{
	name = "Enemy";
	minsize = m_model->Get_min();
	maxsize = m_model->Get_max();
	Camera* cam;
	time = 0;
	cam = AddComponent<Camera>();
	cam->SetMode(CamMode::FPP);
	status = AddComponent<Status>();
	status->SetMAX(10);
	status->SetBreak(true);
	AddComponent<Gravity>();
	AddComponent<SphereShadow>();
	AddComponent<Leg_01>();
	AddComponent<HitBox>()->Set(HITBOX_TYPE::SPHERE);
	m_pos = D3DXVECTOR3(0.0f, 100.0f, 0.0f);
	m_scl = D3DXVECTOR3(0.25f, 0.25f, 0.25f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	cam->SetRange(320.0f + m_scl.z);
#ifndef MUTE
	hit = Manager::GetScene()->AddGameObject<Audio>(2);
	hit->Load("asset\\SE\\hit01.wav");
#endif //MUTE

	Scene* scene = Manager::GetScene();
	if (scene != NULL)
		tag = scene->GetGameObject<Player>();

	spd = TOOL::RandF() * 20.0f + 10.f;
}

void Enemy::Uninit()
{
#ifndef MUTE
	hit->SetDeath();
#endif //MUTE
}

void Enemy::Update()
{
	Camera* cam;
	cam = LoadComponent<Camera>();
	if (m_pos.y <= fabsf(m_model->Get_min().y * m_scl.y))
	{
		m_pos.y = fabsf(m_model->Get_min().y * m_scl.y);
	}

	time++;
	Scene* scene = Manager::GetScene();
	if (tag != NULL)
	{
		if (cam->GetTarget() != NULL)
		{
			Float3 a = cam->GetTarget()->Getpos() - m_pos;
			float angle = TOOL::SubAngle(a, GetForward());
			if (fabsf(angle) > TOOL::SecDiv(TOOL::AToR(180.0f)))
			{
				if (angle < 0.0f)
					angle = TOOL::SecDiv(-TOOL::AToR(30.0f));
				else
					angle = TOOL::SecDiv(TOOL::AToR(30.0f));
			}
			m_rot.y += angle;

			status->SetShot(time > TOOL::FrameMulti(FireRate));

			if (time > TOOL::FrameMulti(FireRate))
			{
				time = 0;
				FireRate = (TOOL::RandF() * 2.0f) + 1.0f;
			}
			else
				if (time < TOOL::FrameMulti(FireRate * 0.85f))
					m_pos += GetForward() * TOOL::SecDiv(m_scl.z * spd);
				else
				{
					if (fabsf(angle) > TOOL::SecDiv(TOOL::AToR(180.0f)))
					{
						if (angle < 0.0f)
							angle = TOOL::SecDiv(-TOOL::AToR(1800.0f));
						else
							angle = TOOL::SecDiv(TOOL::AToR(1800.0f));
					}
				}
		}
		else
			cam->SetTarget(tag);
	}
	else
	{
		if (scene != NULL)
			tag = scene->GetGameObject<Player>();
	}
}

void Enemy::Draw()
{
	//入力レイアウト設定
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	//シェーダー入力
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	//マトリクス設定
	D3DXMATRIX world, scl, rot, trans;
	D3DXMatrixScaling(&scl, m_scl.x, m_scl.y, m_scl.z);
	D3DXMatrixRotationYawPitchRoll(&rot, m_rot.y + m_addrot.y, m_rot.x + m_addrot.x, m_rot.z + m_addrot.z);
	D3DXMatrixTranslation(&trans, m_pos.x + m_addpos.x, m_pos.y + m_addpos.y, m_pos.z + m_addpos.z);
	world = scl * rot * trans;
	Renderer::SetWorldMatrix(&world);
	m_model->Draw();
}

void Enemy::Load()
{
	m_model = Manager::AddModel("asset\\models\\CamBot_E01.obj");
	//シェーダー関係
	Manager::GetShaderState(&m_VertexShader, &m_PixelShader, &m_VertexLayout, SHADER_S::NORMAL_FOG);

	blendState = Manager::GetBlend(BLEND_S::OBJ_OPAQUE);
}

void Enemy::Damage(int dmg)
{
	if (status)
		status->PullHP(dmg);
}

void Enemy::PlayHit()
{
#ifndef MUTE
	int a = 0;
	Player* pl = Manager::GetScene()->GetGameObject<Player>();
	Camera* cam = Manager::GetScene()->GetGameObject<ViewCamera>()->GetView();
	if (pl)
	{
		Float3 dir = m_pos - pl->Getpos();
		a = hit->Play(false, 0.1f);
		hit->SetPan(
			0.5f - (sinf(TOOL::SubAngle(cam->GetDir(), dir)) / 2.0f),
			0.5f + (sinf(TOOL::SubAngle(cam->GetDir(), dir)) / 2.0f)
			, a);
	}
#endif // !MUTE
}

void Enemy::Finish()
{
	ParticleManager* mana;

	Scene* scene = Manager::GetScene();
	scene->AddGameObject<ExplodeDome>((int)OBJ_LAYER::GameObject)->Set(m_pos, m_model->Get_max().z * m_scl.z, 0.2f, 0);

	mana = scene->AddGameObject<ParticleManager>(1);
	mana->SetModel(PARTICLE_S::ITEM2, BLEND_S::OBJ_TRANS);
	mana->Set(m_pos, Float3(0.0f, 0.0f, 0.0f), 2.0f * m_scl.z, TOOL::RandF() * 15 + 5 + (50 * m_scl.z), GRAVITY, 2, m_scl * 0.5f);

	mana = scene->AddGameObject<ParticleManager>(1);
	mana->SetModel(PARTICLE_S::ITEM3, BLEND_S::OBJ_TRANS);
	mana->Set(m_pos, Float3(0.0f, 0.0f, 0.0f), 3.0f * m_scl.z, TOOL::RandF() * 10 + (5 * m_scl.z), GRAVITY, 2, m_scl * 0.5f);

	mana = scene->AddGameObject<ParticleManager>(1);
	mana->SetModel(PARTICLE_S::GIA, BLEND_S::OBJ_OPAQUE);
	mana->Set(m_pos, Float3(0.0f, 0.0f, 0.0f), 0.5f * m_scl.z, TOOL::RandF() * 10 + (5 * m_scl.z), GRAVITY, 2, m_scl * 0.5f);

	mana = scene->AddGameObject<ParticleManager>(1);
	mana->SetModel(PARTICLE_S::CUBE, BLEND_S::OBJ_TRANS);
	mana->Set(m_pos,				//座標
		Float3(0.0f, 0.0f, 0.0f),	//角度
		2.0f * m_scl.z,				//サイズ
		TOOL::RandF() * 15 + 5 + (50 * m_scl.z),//数
		GRAVITY,					//重力値
		2,							//生存時間
		m_scl * 0.5f,
		{ 3.f,3.f,3.f });				//サイズ

	SetDestroy();
}