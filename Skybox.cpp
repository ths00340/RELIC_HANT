#include "main.h"
#include "renderer.h"
#include "model.h"
#include "input.h"
#include "Scene.h"
#include "manager.h"
#include "Tools.h"
#include "Player.h"
#include "Skybox.h"

void Skybox::Init()
{
	m_model = ResourceManager::AddModel("asset\\models\\Skybox_001.obj");
	//シェーダー関係
	ResourceManager::GetShaderState(&m_VertexShader, &m_PixelShader, &m_VertexLayout, SHADER_S::LIGHT_OFF);

	blendState = ResourceManager::GetBlend(BLEND_S::OBJ_OPAQUE);

	name = "Skybox";
	Scene* scene = Manager::GetScene();
	m_pPl = scene->GetGameObject<Player>();

	m_pos = Float3(0.f, 0.f, 0.f);
	m_scl = Float3(80.f, 80.f, 80.f);
	m_rot = Float3(0.f, 0.f, 0.f);
}

void Skybox::Uninit()
{
}

void Skybox::Update()
{
	if (m_pPl != NULL)
		m_pos = m_pPl->Getpos();
	else
	{
		Scene* scene = Manager::GetScene();
		if (scene != NULL)
			m_pPl = scene->GetGameObject<Player>();
	}
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
	ResourceManager::AddModel("asset\\models\\Skybox_001.obj");
}