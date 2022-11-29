#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "model.h"
#include "Dodai.h"

void Dodai::Init()
{
	m_model = Manager::AddModel("asset\\models\\pedestalOBJ.obj");
	//シェーダー関係
	Manager::GetShaderState(&m_VertexShader, &m_PixelShader, &m_VertexLayout, SHADER_S::LIGHT_ON);
	blendState = Manager::GetBlend(BLEND_S::OBJ_OPAQUE);

	name = "Dodai";
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_scl = D3DXVECTOR3(0.25f, 0.25f, 0.25f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	minsize = m_model->Get_min();
	maxsize = m_model->Get_max();
}

void Dodai::Uninit()
{
}

void Dodai::Update()
{
}

void Dodai::Draw()
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