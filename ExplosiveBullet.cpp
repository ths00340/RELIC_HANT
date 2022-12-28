#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "model.h"
#include "Player.h"
#include "input.h"
#include "Game.h"
#include "ExplosiveBullet.h"
#include "Enemy.h"
#include "Tools.h"
#include "Status.h"
#include "Gravity.h"
#include "Camera.h"
#include "ExplodeDome.h"

Model* ExplosiveBullet::m_model;

ID3D11VertexShader* ExplosiveBullet::m_VertexShader = NULL;
ID3D11PixelShader* ExplosiveBullet::m_PixelShader = NULL;
ID3D11InputLayout* ExplosiveBullet::m_VertexLayout = NULL;
ID3D11BlendState* ExplosiveBullet::blendState = NULL;

void ExplosiveBullet::Finish()
{
	float maxshake = 120.f;
	Scene* scene = Manager::GetScene();
	scene->AddGameObject<ExplodeDome>((int)OBJ_LAYER::GameObject)->Set(m_pos, DmgRange, 0.2f, atk);
	SetDestroy();
	GameObject* pl = scene->GetGameObject<Player>();
	if (pl)
	{
		float rang = TOOL::PointRange(pl->Getpos(), m_pos);
		maxshake = rang / maxshake;
		maxshake = 1.f - maxshake;//離れた分だけ減少
		pl->LoadComponent<Camera>()->SetShakePos(30 * maxshake, 1.0f * maxshake);
		pl->LoadComponent<Camera>()->SetShakeRot(30 * maxshake, TOOL::AToR(2.5f) * maxshake);
	}
}

void ExplosiveBullet::Init()
{
	AddComponent<Gravity>();
	m_pos = Float3(-3.f, 1.f, 0.f);
	m_scl = Float3(0.25f, 0.25f, 0.25f);
	m_rot = Float3(0.f, 0.f, 0.f);
	minsize = m_model->Get_min();
	maxsize = m_model->Get_max();
	name = "ExplosiveBullet";
}

void ExplosiveBullet::Uninit() {}

void ExplosiveBullet::Update()
{
	Scene* scene = Manager::GetScene();
	std::vector<Enemy*>enemyList = scene->GetGameObjects<Enemy>();

	Float3 nextpos = m_pos + m_vec;

	m_rot.x = TOOL::LookatX(m_pos, nextpos).x;

	for (Enemy* enemy : enemyList)
	{
		Float3 enemyPos = enemy->Getpos();
		float size_z_e = enemy->GetModel()->Get_max().z * enemy->Getscl().z;
		float size_z_m = m_model->Get_max().z * m_scl.z;

		if (TOOL::CanHit(m_pos, enemyPos, size_z_e + size_z_m))
		{
			Status* sta;
			Finish();
			enemy->PlayHit();
			sta = enemy->LoadComponent<Status>();
			sta->PullHP(atk);
			return;
		}
	}

	float groundHeight = fabsf(Getmin().y) * Getscl().y;

	if (m_pos.y <= groundHeight + 0.1f)
		Finish();

	if (TOOL::CanRange(m_pos, startpos, 60.0f))
	{
		Finish();
	}
}

void ExplosiveBullet::Draw()
{
	//入力レイアウト設定
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	//シェーダー入力
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	//マトリクス設定
	D3DXMATRIX world, scl, rot, trans;
	D3DXMatrixScaling(&scl, m_scl.x, m_scl.y, m_scl.z);
	D3DXMatrixRotationYawPitchRoll(&rot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixTranslation(&trans, m_pos.x, m_pos.y, m_pos.z);
	world = scl * rot * trans;
	Renderer::SetWorldMatrix(&world);
	m_model->Draw();
}

void ExplosiveBullet::Set(Float3 pos, Float3 rot, float vel, int dmg, float dmgrange)
{
	m_pos = pos;
	startpos = pos;
	m_rot = rot;
	m_vec = TOOL::GetForward(rot) * vel;
	atk = dmg;
	DmgRange = dmgrange;
}

void ExplosiveBullet::Load()
{
	m_model = ResourceManager::AddModel("asset\\models\\Bazooka_bul.obj");
	ResourceManager::GetShaderState(&m_VertexShader, &m_PixelShader, &m_VertexLayout, SHADER_S::NORMAL_FOG);
	blendState = ResourceManager::GetBlend(BLEND_S::OBJ_OPAQUE);
}

void ExplosiveBullet::AddVel(Float3 add)
{
	m_vec += add;
}