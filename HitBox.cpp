#include "main.h"
#include "manager.h"
#include "Tools.h"
#include "GameObject.h"
#include "HitBox.h"

Model* HitBox::model[(int)HITBOX_TYPE::TYPE_END];
ID3D11VertexShader* HitBox::m_VertexShader;
ID3D11PixelShader* HitBox::m_PixelShader;
ID3D11InputLayout* HitBox::m_VertexLayout;
ID3D11BlendState* HitBox::blendState = NULL;

void HitBox::Init()
{}

void HitBox::Uninit()
{}

void HitBox::Update()
{
	Float3 TrueSize_max = TOOL::Uniform(), TrueSize_min = TOOL::Uniform();

	TrueSize_max.x = object->Getmax().x * object->Getscl().x;
	TrueSize_max.y = object->Getmax().y * object->Getscl().y;
	TrueSize_max.z = object->Getmax().z * object->Getscl().z;

	TrueSize_min.x = object->Getmin().x * object->Getscl().x;
	TrueSize_min.y = object->Getmin().y * object->Getscl().y;
	TrueSize_min.z = object->Getmin().z * object->Getscl().z;

	addpos.x = (TrueSize_min.x + TrueSize_max.x);
	addpos.y = (TrueSize_min.y + TrueSize_max.y);
	addpos.z = (TrueSize_min.z + TrueSize_max.z);

	m_size = TOOL::Magni_uni(model[(int)type]->Get_max(), TrueSize_max);
	m_size.y *= 0.5f;
	addpos.y *= 0.5f;
}

void HitBox::Draw()
{
	if (!draw)
		return;

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

	switch (type)
	{
	case HITBOX_TYPE::CUBE:
		D3DXMatrixScaling(&scl, m_size.x, m_size.y, m_size.z);
		D3DXMatrixTranslation(&trans, object->Getpos().x + addpos.x, object->Getpos().y + addpos.y, object->Getpos().z + addpos.z);
		break;
	case HITBOX_TYPE::SPHERE:
		D3DXMatrixScaling(&scl, m_size.z, m_size.z, m_size.z);
		D3DXMatrixTranslation(&trans, object->Getpos().x + addpos.x, object->Getpos().y, object->Getpos().z + addpos.z);
		break;
	}

	D3DXMatrixRotationYawPitchRoll(&rot, object->Getrot().y, object->Getrot().x, object->Getrot().z);

	world = scl * rot * trans;
	Renderer::SetWorldMatrix(&world);
	model[(int)type]->Draw();

	Renderer::GetDeviceContext()->OMSetBlendState(Manager::GetBlend(BLEND_S::OBJ_OPAQUE), blendFactor, 0xffffffff);
}

void HitBox::Set(HITBOX_TYPE intype, bool indraw)
{
	type = intype;
	draw = indraw;
}

void HitBox::Load()
{
	model[(int)HITBOX_TYPE::CUBE] = Manager::AddModel("asset\\models\\Hitbox_Cube.obj");
	model[(int)HITBOX_TYPE::SPHERE] = Manager::AddModel("asset\\models\\Hitbox_Sphere.obj");
	Manager::GetShaderState(&m_VertexShader, &m_PixelShader, &m_VertexLayout, SHADER_S::LIGHT_OFF);
	blendState = Manager::GetBlend(BLEND_S::OBJ_TRANS);
}