#include "main.h"
#include "renderer.h"
#include "model.h"
#include "Platform.h"
#include "CarMove.h"
#include "Camera.h"
#include "ViewCamera.h"

Model* Platform::m_model;
ID3D11VertexShader* Platform::m_VertexShader;
ID3D11PixelShader* Platform::m_PixelShader;
ID3D11InputLayout* Platform::m_VertexLayout;
ID3D11BlendState* Platform::blendState;

void Platform::Init()
{
	name = "Plarform";
	AddComponent<Camera>();
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_scl = D3DXVECTOR3(0.05f, 0.05f, 0.05f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
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
	m_model->Draw();
}

void Platform::Load()
{
	m_model = ResourceManager::AddModel("asset\\models\\settingroom.obj");
	//シェーダー関係
	ResourceManager::GetShaderState(&m_VertexShader, &m_PixelShader, &m_VertexLayout, SHADER_S::LIGHT_OFF);
	blendState = ResourceManager::GetBlend(BLEND_S::OBJ_OPAQUE);
}