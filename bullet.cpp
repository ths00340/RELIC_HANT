#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "GameObject.h"
#include "model.h"
#include "Player.h"
#include "input.h"
#include "Game.h"
#include "bullet.h"
#include "Enemy.h"
#include "Tools.h"
#include "Status.h"
#include "ParticleManager.h"

Model* Bullet::m_model;

ID3D11VertexShader* Bullet::m_VertexShader = NULL;
ID3D11PixelShader* Bullet::m_PixelShader = NULL;
ID3D11InputLayout* Bullet::m_VertexLayout = NULL;
ID3D11BlendState* Bullet::blendState = NULL;

void Bullet::Init()
{
	m_pos = Float3(-3.0f, 1.0f, 0.0f);
	m_scl = Float3(0.25f, 0.25f, 0.25f);
	m_rot = Float3(0.0f, 0.0f, 0.0f);
	minsize = m_model->Get_min();
	maxsize = m_model->Get_max();
	name = "Bullet";
}

void Bullet::Uninit() {}

void Bullet::Update()
{
	Scene* scene = Manager::GetScene();
	if (TOOL::CanRange(m_pos, startpos, 1.f))
	{
		std::vector<GameObject*>enemyList = scene->GetGameObjCmp<Status>();

		for (GameObject* enemy : enemyList)
		{
			Float3 enemyPos = enemy->Getpos();
			float size_z_e = enemy->Getmax().z * enemy->Getscl().z;
			float size_z_m = m_model->Get_max().z * m_scl.z;

			if (TOOL::CanHit(m_pos, enemyPos, size_z_e + size_z_m))

			{
				Status* sta;
				SetDestroy();
				enemy->PlayHit();
				sta = enemy->LoadComponent<Status>();
				sta->PullHP(atk);
				scene->AddGameObject<Explosion>((int)OBJ_LAYER::Billboard)->Set(m_pos);

				ParticleManager* mana = Manager::GetScene()->AddGameObject<ParticleManager>(1);
				mana->SetModel(PARTICLE_S::DEBRIS, BLEND_S::OBJ_OPAQUE);
				mana->Set(m_pos, Float3(0.0f, 0.0f, 0.0f), 0.3f * m_scl.z, TOOL::RandF() * 5, GRAVITY, 1, TOOL::Uniform(0.25) * m_scl.z, Float3(3.0f, 3.0f, 3.0f));
				return;
			}
		}
	}

	if (TOOL::CanRange(m_pos, startpos, range))
	{
		SetDestroy();
	}
}

void Bullet::Draw()
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

void Bullet::Set(Float3 pos, Float3 rot, float vel, int dmg, float maxrange)
{
	m_pos = pos;
	startpos = pos;
	m_rot = rot;
	m_vec = TOOL::GetForward(rot) * vel;
	atk = dmg;
	range = maxrange;
}

void Bullet::Load()
{
	m_model = ResourceManager::AddModel("asset\\models\\bul.obj");
	ResourceManager::GetShaderState(&m_VertexShader, &m_PixelShader, &m_VertexLayout, SHADER_S::LIGHT_OFF);
	blendState = ResourceManager::GetBlend(BLEND_S::OBJ_OPAQUE);
}