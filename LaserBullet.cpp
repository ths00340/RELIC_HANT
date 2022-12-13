#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "model.h"
#include "Player.h"
#include "input.h"
#include "Game.h"
#include "Enemy.h"
#include "Tools.h"
#include "Status.h"
#include "LaserBullet.h"
#include "ParticleManager.h"

Model* LaserBullet::m_model;

ID3D11VertexShader* LaserBullet::m_VertexShader = NULL;
ID3D11PixelShader* LaserBullet::m_PixelShader = NULL;
ID3D11InputLayout* LaserBullet::m_VertexLayout = NULL;
ID3D11BlendState* LaserBullet::blendState = NULL;

void LaserBullet::Init()
{
	m_pos = Float3(-3.0f, 1.0f, 0.0f);
	m_scl = Float3(0.25f, 0.25f, 0.25f);
	m_rot = Float3(0.0f, 0.0f, 0.0f);
	minsize = m_model->Get_min();
	maxsize = m_model->Get_max();
	time = 0;
	livetime = TOOL::FrameMulti(0.35f);
	m_scl.z = TOOL::Magni_one(maxsize.z, 60.0f);
	startsize = m_scl.y;
	name = "LaserBullet";
}

void LaserBullet::Uninit()
{
}

void LaserBullet::Update()
{
	if (time == 0)
	{
		Scene* scene = Manager::GetScene();
		std::vector<GameObject*> hitlist = scene->GetGameObjCmp<Status>();
		hitlist = TOOL::WithinObj(m_pos, Range, hitlist);

		Float3 pos;
		for (GameObject* obj : hitlist)
		{
			if (TOOL::HitRaySphere(m_pos, GetForward(), obj->Getpos(), obj->Getmax().z * obj->Getscl().z, pos))
			{
				obj->LoadComponent<Status>()->PullHP(atk);
				ParticleManager* mana = scene->AddGameObject<ParticleManager>((int)OBJ_LAYER::GameObject);
				mana->SetModel(PARTICLE_S::ITEM1, BLEND_S::OBJ_TRANS);
				mana->Set(pos, TOOL::Uniform(), 1.f * m_scl.x, TOOL::RandF() * 15, 0.f, 1, TOOL::Uniform(0.05f));
			}
		}
	}

	time++;
	m_scl.x = startsize - (startsize * ((float)time / livetime));
	m_scl.y = startsize - (startsize * ((float)time / livetime));

	if (time > livetime)
		Finish();
}

void LaserBullet::Draw()
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

void LaserBullet::Set(Float3 pos, Float3 rot, int dmg, float range)
{
	m_pos = pos;
	m_rot = rot;
	atk = dmg;
	Range = range;
	m_scl.z = TOOL::Magni_one(maxsize.z, Range);
}

void LaserBullet::Finish()
{
	SetDestroy();
}

void LaserBullet::Load()
{
	m_model = ResourceManager::AddModel("asset\\models\\laser02.obj");
	ResourceManager::GetShaderState(&m_VertexShader, &m_PixelShader, &m_VertexLayout, SHADER_S::LIGHT_ON);
	blendState = ResourceManager::GetBlend(BLEND_S::OBJ_OPAQUE);
}