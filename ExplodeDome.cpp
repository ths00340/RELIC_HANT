#include "main.h"
#include "renderer.h"
#include "manager.h"
#include "Scene.h"
#include "Tools.h"
#include "GameObject.h"
#include "ExplodeDome.h"
#include "Status.h"
#include "Fissure.h"

void ExplodeDome::Finish()
{
	Scene* scene = Manager::GetScene();

	for (GameObject* obj : TOOL::WithinObj(m_pos, Max, scene->GetGameObjCmp<Status>()))
	{
		int dmg = maxdmg;

		if (Decay)
			dmg = maxdmg * (TOOL::PointRange(obj->Getpos(), m_pos) / Max);

		obj->Damage(dmg);
	}
	Fissure* fis = scene->AddGameObject<Fissure>((int)OBJ_LAYER::GameObject);
	fis->Set(0.5f);
	fis->SetPos(m_pos);
	fis->SetScl(TOOL::Uniform(Max));

	SetDestroy();
}

void ExplodeDome::Load()
{
	ResourceManager::AddModel("asset\\models\\bomdome.obj");
}

void ExplodeDome::Init()
{
	ResourceManager::GetShaderState(&m_VertexShader, &m_PixelShader, &m_VertexLayout, SHADER_S::LIGHT_OFF);
	m_model = ResourceManager::AddModel("asset\\models\\bomdome.obj");
	blendState = ResourceManager::GetBlend(BLEND_S::OBJ_OPAQUE);
	minsize = m_model->Get_min();
	maxsize = m_model->Get_max();
	m_pos = TOOL::Uniform();
	m_scl = TOOL::Uniform(1.f);
	m_rot = TOOL::Uniform();
	frame = 0;
	name = "ExplodeDome";
}

void ExplodeDome::Uninit()
{
}

void ExplodeDome::Update()
{
	frame++;

	float time = (float)frame / (float)TOOL::FrameMulti(endtime);

	m_scl = TOOL::Magni_uni(maxsize, TOOL::Uniform(Max)) * time;
	m_rot += GetUp() * TOOL::AToR(10.f);

	if (frame > TOOL::FrameMulti(endtime))
		Finish();
}

void ExplodeDome::Draw()
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

void ExplodeDome::Set(Float3 pos, float maxrange, float inend, int inmaxdmg, bool inde)
{
	Max = maxrange;
	endtime = inend;
	maxdmg = inmaxdmg;
	Decay = inde;
	m_pos = pos;
}