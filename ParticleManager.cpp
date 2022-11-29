#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "model.h"
#include "Tools.h"
#include "ParticleManager.h"

ID3D11BlendState* ParticleManager::blendState;
Model* ParticleManager::m_model[(int)PARTICLE_S::END];
ID3D11PixelShader* ParticleManager::m_PixelShader;
ID3D11VertexShader* ParticleManager::m_VertexShader;
ID3D11InputLayout* ParticleManager::m_VertexLayout;

void ParticleManager::Init()
{
	name = "ParticleManager";
	Bstate = BLEND_S::OBJ_TRANS;
	Pstate = PARTICLE_S::ITEM1;
	for (GameObject* object : Particle)//範囲forループ
	{
		object->Init();
	}
}

void ParticleManager::Uninit()
{
	for (GameObject* object : Particle)
	{
		object->Uninit();
		delete object;
	}
}

void ParticleManager::Update()
{
	for (GameObject* object : Particle)//範囲forループ
	{
		object->Update();
	}
	Particle.remove_if([](GameObject* object) {return object->Destroy(); });

	if (Particle.size() <= 0)
		SetDestroy();
}

void ParticleManager::Draw()
{
	//入力レイアウト設定
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	//シェーダー入力
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	for (GameObject* object : Particle)//範囲forループ
	{
		if (object->GetBlendState())
			Renderer::GetDeviceContext()->OMSetBlendState(object->GetBlendState(), blendFactor, 0xffffffff);
		object->Draw();
	}
}

void ParticleManager::Set(Float3 pos, Float3 rot, float vel, int particle_num, float inGravity, int livetime, Float3 scl, Float3 Rot)
{
	m_pos = pos;
	m_rot = rot;
	Particles = particle_num;
	for (int i = 0; i < Particles; i++)
	{
		ParticleTest* gameObject = new ParticleTest();
		gameObject->Init();
		gameObject->SetModel(m_model[(int)Pstate], Bstate);
		gameObject->Set(m_pos, inGravity, TOOL::FrameMulti((float)livetime), TOOL::Uniform(vel), scl, Rot);
		Particle.push_back(gameObject);
	}
}

void ParticleManager::Load()
{
	m_model[(int)PARTICLE_S::ITEM1] = Manager::AddModel("asset\\models\\Item01.obj");
	m_model[(int)PARTICLE_S::ITEM2] = Manager::AddModel("asset\\models\\Item02.obj");
	m_model[(int)PARTICLE_S::ITEM3] = Manager::AddModel("asset\\models\\Item03.obj");
	m_model[(int)PARTICLE_S::GIA] = Manager::AddModel("asset\\models\\neji.obj");
	m_model[(int)PARTICLE_S::DEBRIS] = Manager::AddModel("asset\\models\\hahen.obj");
	m_model[(int)PARTICLE_S::CUBE] = Manager::AddModel("asset\\models\\tolitoli.obj");
	Manager::GetShaderState(&m_VertexShader, &m_PixelShader, &m_VertexLayout, SHADER_S::LIGHT_ON);
}