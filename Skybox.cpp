#include "main.h"
#include "renderer.h"
#include "model.h"
#include "input.h"
#include "Scene.h"
#include "manager.h"
#include "Tools.h"
#include "Player.h"
#include "Skybox.h"

Model* Skybox::m_model;
Player* pl;
ID3D11VertexShader* Skybox::m_VertexShader;
ID3D11PixelShader* Skybox::m_PixelShader;
ID3D11InputLayout* Skybox::m_VertexLayout;
ID3D11BlendState* Skybox::blendState;

void Skybox::Init()
{
	name = "Skybox";
	Scene* scene = Manager::GetScene();
	pl = scene->GetGameObject<Player>();

	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_scl = D3DXVECTOR3(80.0f, 80.0f, 80.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

void Skybox::Uninit()
{
}

void Skybox::Update()
{
	if (pl != NULL)
		m_pos = pl->Getpos();
	else
	{
		Scene* scene = Manager::GetScene();
		if (scene != NULL)
			pl = scene->GetGameObject<Player>();
	}

	m_rot.y += D3DX_PI * 0.0001f;
}

void Skybox::Draw()
{
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

void Skybox::Load()
{
	m_model = ResourceManager::AddModel("asset\\models\\Skybox_001.obj");
	//シェーダー関係
	ResourceManager::GetShaderState(&m_VertexShader, &m_PixelShader, &m_VertexLayout, SHADER_S::LIGHT_OFF);

	blendState = ResourceManager::GetBlend(BLEND_S::OBJ_OPAQUE);
}