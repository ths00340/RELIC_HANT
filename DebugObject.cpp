#include "main.h"
#include "manager.h"
#include "Tools.h"
#include "GameObject.h"
#include "DebugObject.h"

void DebugObject::Init()
{
	DebugOBJ = ResourceManager::AddModel("asset\\models\\cube.obj");
	m_size = TOOL::Uniform(1.f);
	ResourceManager::GetShaderState(&m_VertexShader, &m_PixelShader, &m_VertexLayout, SHADER_S::LIGHT_OFF);
	blendState = ResourceManager::GetBlend(BLEND_S::OBJ_TRANS);
}

void DebugObject::Update()
{}

void DebugObject::Uninit()
{}

void DebugObject::Draw()
{
	float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };

	if (blendState != NULL)
		Renderer::GetDeviceContext()->OMSetBlendState(blendState, blendFactor, 0xffffffff);

	//入力レイアウト設定
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	//シェーダー入力
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	//マトリクス設定
	D3DXMATRIX world, scl, rot, trans;

	D3DXMatrixScaling(&scl, m_size.z, m_size.z, m_size.z);
	D3DXMatrixTranslation(&trans, object->Getpos().x, object->Getpos().y, object->Getpos().z);

	D3DXMatrixRotationYawPitchRoll(&rot, object->Getrot().y, object->Getrot().x, object->Getrot().z);

	world = scl * rot * trans;
	Renderer::SetWorldMatrix(&world);
	DebugOBJ->Draw();

	Renderer::GetDeviceContext()->OMSetBlendState(ResourceManager::GetBlend(BLEND_S::OBJ_OPAQUE), blendFactor, 0xffffffff);
}