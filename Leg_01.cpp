#include "main.h"
#include "renderer.h"
#include "Camera.h"
#include "GameObject.h"
#include "Tools.h"
#include "input.h"
#include "model.h"
#include "Leg_01.h"

void Leg_01::Init()
{
	flont = ResourceManager::AddModel("asset\\models\\Bot01_Leg_Flont.obj");
	Back = ResourceManager::AddModel("asset\\models\\Bot01_Leg_Back.obj");
	//シェーダー関係
	ResourceManager::GetShaderState(&m_VertexShader, &m_PixelShader, &m_VertexLayout, SHADER_S::NORMAL_FOG);
}

void Leg_01::Uninit()
{
}

void Leg_01::Update()
{
	Float3 oldnew = oldPos - object->Getpos();
	oldnew.y = 0.0f;
	spd = TOOL::VectorLength(oldnew);
	if (move)
	{
		angle += D3DX_PI * spd;
	}
	oldPos = object->Getpos();

	addpos.y = object->Getmin().y * object->Getscl().y;

	if (angle >= TOOL::AToR(360) || angle <= -TOOL::AToR(360))
	{
		angle = 0;
	}
}

void Leg_01::Draw()
{
	//入力レイアウト設定
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	//シェーダー入力
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	//マトリクス設定
	D3DXMATRIX world, scl, rot, trans;
	D3DXMatrixScaling(&scl, object->Getscl().x, object->Getscl().y, object->Getscl().z);

	D3DXMatrixRotationYawPitchRoll(&rot,
		object->Getrot().y + object->Getaddrot().y + (sinf(angle) * TOOL::AToR(10.0f)),
		object->Getrot().x + object->Getaddrot().x,
		object->Getrot().z + object->Getaddrot().z + (sinf(angle) * TOOL::AToR(2.0f)));

	D3DXMatrixTranslation(&trans, object->Getpos().x + addpos.x, object->Getpos().y + addpos.y, object->Getpos().z + addpos.z);
	world = scl * rot * trans;
	Renderer::SetWorldMatrix(&world);
	flont->Draw();

	D3DXMatrixRotationYawPitchRoll(&rot,
		object->Getrot().y + object->Getaddrot().y - (sinf(angle) * TOOL::AToR(10.0f)),
		object->Getrot().x + object->Getaddrot().x,
		object->Getrot().z + object->Getaddrot().z - (sinf(angle) * TOOL::AToR(2.0f)));

	world = scl * rot * trans;
	Renderer::SetWorldMatrix(&world);
	Back->Draw();
}

void Leg_01::Load()
{
	ResourceManager::AddModel("asset\\models\\Bot01_Leg_Flont.obj");
	ResourceManager::AddModel("asset\\models\\Bot01_Leg_Back.obj");
}