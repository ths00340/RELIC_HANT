#include "main.h"
#include "renderer.h"
#include "model.h"
#include "Platform.h"
#include "CarMove.h"
#include "Camera.h"
#include "ViewCamera.h"

void Platform::Init()
{
	m_model = ResourceManager::AddModel("asset\\models\\settingroom.obj");
	//シェーダー関係
	ResourceManager::GetShaderState(&m_VertexShader, &m_PixelShader, &m_VertexLayout, SHADER_S::LIGHT_OFF);
	blendState = ResourceManager::GetBlend(BLEND_S::OBJ_OPAQUE);
	name = "Plarform";
	m_pos = Float3(0.f, 0.f, 0.f);
	m_scl = Float3(0.05f, 0.05f, 0.05f);
	m_rot = Float3(0.f, 0.f, 0.f);
	minsize = m_model->Get_min();
	maxsize = m_model->Get_max();
}

void Platform::Uninit()
{
}

void Platform::Update()
{
}

void Platform::Draw()
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

	ID3D11ShaderResourceView* DepthTexture = Renderer::GetShadowDepthTexture();
	Renderer::GetDeviceContext()->PSSetShaderResources(1, 1, &DepthTexture);

	m_model->Draw();
}

void Platform::Load()
{
	ResourceManager::AddModel("asset\\models\\settingroom.obj");
}