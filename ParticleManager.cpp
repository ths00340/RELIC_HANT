#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "model.h"
#include "Tools.h"
#include "ParticleManager.h"

#define MAX_PARTICLE 100

void ParticleManager::Init()
{
	m_model[(int)PARTICLE_S::ITEM1] = ResourceManager::AddModel("asset\\models\\Item01.obj");
	m_model[(int)PARTICLE_S::ITEM2] = ResourceManager::AddModel("asset\\models\\Item02.obj");
	m_model[(int)PARTICLE_S::ITEM3] = ResourceManager::AddModel("asset\\models\\Item03.obj");
	m_model[(int)PARTICLE_S::GIA] = ResourceManager::AddModel("asset\\models\\neji.obj");
	m_model[(int)PARTICLE_S::DEBRIS] = ResourceManager::AddModel("asset\\models\\hahen.obj");
	m_model[(int)PARTICLE_S::CUBE] = ResourceManager::AddModel("asset\\models\\tolitoli.obj");

	ResourceManager::GetShaderState(&m_VertexShader, &m_PixelShader, &m_VertexLayout, SHADER_S::INSTANCE_LIGHT_ON);

	HRESULT hr;
	D3DXMATRIX* inWorld = new D3DXMATRIX[MAX_PARTICLE];
	Float3* inPos = new Float3[MAX_PARTICLE];

	for (int i = 0; i < MAX_PARTICLE; i++)
	{
		inWorld[i] *= 0.f;
		inPos[i] *= 0.f;
	}

	//Matrix
	{
		// 頂点バッファ生成
		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(D3DXMATRIX) * MAX_PARTICLE;
		bd.StructureByteStride = sizeof(D3DXMATRIX);
		bd.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		bd.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;

		D3D11_SUBRESOURCE_DATA sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.pSysMem = inWorld;

		hr = Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_pWorldBuffer);
		if (FAILED(hr))
		{
			delete[] inWorld;
			int failed = 1;
			return;
		}
		delete[] inWorld;

		D3D11_SHADER_RESOURCE_VIEW_DESC srvd;
		ZeroMemory(&srvd, sizeof(srvd));
		srvd.Format = DXGI_FORMAT_UNKNOWN;
		srvd.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
		srvd.Buffer.FirstElement = 0;
		srvd.Buffer.NumElements = MAX_PARTICLE;
		hr = Renderer::GetDevice()->CreateShaderResourceView(m_pWorldBuffer, &srvd, &m_pWorldSRV);
		if (FAILED(hr))
		{
			int failed = 1;
		}
	}

	name = "ParticleManager";
	Bstate = BLEND_S::OBJ_TRANS;
	Pstate = PARTICLE_S::ITEM1;
	m_BlendState = ResourceManager::GetBlend(Bstate);
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

	m_pWorldBuffer->Release();
	m_pWorldSRV->Release();
}

void ParticleManager::Update()
{
	D3DXMATRIX	inWorld[MAX_PARTICLE];
	Float3		inPos[MAX_PARTICLE];
	int i = 0;
	for (ParticleTest* object : Particle)//範囲forループ
	{
		object->Update();
		object->Draw();
		inWorld[i] = object->GetWorld();
		inPos[i] = object->Getpos();
		i++;
	}
	Particle.remove_if([](GameObject* object) {return object->Destroy(); });

	if (Particle.size() <= 0)
	{
		SetDestroy();
		return;
	}
	Renderer::GetDeviceContext()->UpdateSubresource(m_pWorldBuffer, 0, NULL, inWorld, 0, 0);
}

void ParticleManager::Draw()
{
	//入力レイアウト設定
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	//シェーダー入力
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };

	Renderer::GetDeviceContext()->OMSetBlendState(m_BlendState, blendFactor, 0xffffffff);
	m_model[(int)Pstate]->InstanceDraw(Particle.size(), m_pWorldSRV);
}

void ParticleManager::Set(Float3 pos, Float3 rot, float vel, int particle_num, float inGravity, int livetime, Float3 scl, Float3 Rot)
{
	m_pos = pos;
	m_rot = rot;
	Particles = particle_num;
	Particles = TOOL::Limit(particle_num, 500, 0);

	for (int i = 0; i < Particles; i++)
	{
		ParticleTest* gameObject = new ParticleTest();
		gameObject->Init();
		gameObject->SetModel(m_model[(int)Pstate], Bstate);
		gameObject->Set(m_pos, inGravity, TOOL::FrameMulti((float)livetime), TOOL::Uniform(vel), scl, Rot);
		Particle.push_back(gameObject);
	}
}

void ParticleManager::SetModel(PARTICLE_S partmodel, BLEND_S blend)
{
	Bstate = blend;
	Pstate = partmodel;
	m_BlendState = ResourceManager::GetBlend(Bstate);
}

void ParticleManager::Load()
{
	ResourceManager::AddModel("asset\\models\\Item01.obj");
	ResourceManager::AddModel("asset\\models\\Item02.obj");
	ResourceManager::AddModel("asset\\models\\Item03.obj");
	ResourceManager::AddModel("asset\\models\\neji.obj");
	ResourceManager::AddModel("asset\\models\\hahen.obj");
	ResourceManager::AddModel("asset\\models\\tolitoli.obj");
}