#include "main.h"
#include "renderer.h"
#include "Camera.h"
#include "GameObject.h"
#include "Tools.h"
#include "input.h"
#include "model.h"
#include "CarWheel.h"

Model* CarWheel::model;
ID3D11VertexShader* CarWheel::m_VertexShader;
ID3D11PixelShader* CarWheel::m_PixelShader;
ID3D11InputLayout* CarWheel::m_VertexLayout;

void CarWheel::Init()
{
}

void CarWheel::Uninit()
{
}

void CarWheel::Update()
{
	Float3 oldnew = oldPos - object->Getpos();
	oldnew.y = 0.0f;
	spd = TOOL::VectorLength(oldnew);

	float ad = 1.0f;
	if (TOOL::SubAngle(oldnew, object->GetForward()) < TOOL::AToR(90)
		&& TOOL::SubAngle(oldnew, object->GetForward()) > -TOOL::AToR(90))
		ad = -1.0f;
	else
		ad = 1.0f;

	if (move)
	{
		if (spd > 0.01f)
			angle += D3DX_PI * spd * ad;
	}

	if (angle >= TOOL::AToR(360) && angle <= -TOOL::AToR(360))
		angle = 0;

	oldPos = object->Getpos();

	addpos1 = object->GetUp() * (object->Getmin().y * object->Getscl().y);
	addpos2 = object->GetUp() * (object->Getmin().y * object->Getscl().y);

	addpos1 += object->GetForward() * (4.f * object->Getscl().z);
	addpos2 += object->GetForward() * (-4.2f * object->Getscl().z);
}

void CarWheel::Draw()
{
	wheel.x = angle;

	//入力レイアウト設定
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	//シェーダー入力
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	//マトリクス設定
	D3DXMATRIX world, scl, rot, trans;
	D3DXMatrixScaling(&scl, object->Getscl().x, object->Getscl().y, object->Getscl().z);

	D3DXMatrixRotationYawPitchRoll(&rot,
		object->Getrot().y + wheel.y,
		object->Getrot().x + wheel.x,
		object->Getrot().z + wheel.z);

	D3DXMatrixTranslation(&trans, object->Getpos().x + addpos1.x, object->Getpos().y + addpos1.y, object->Getpos().z + addpos1.z);
	world = scl * rot * trans;
	Renderer::SetWorldMatrix(&world);
	model->Draw();

	D3DXMatrixRotationYawPitchRoll(&rot,
		object->Getrot().y + wheel.y,
		object->Getrot().x + wheel.x,
		object->Getrot().z + wheel.z);

	D3DXMatrixTranslation(&trans, object->Getpos().x + addpos2.x, object->Getpos().y + addpos2.y, object->Getpos().z + addpos2.z);

	world = scl * rot * trans;
	Renderer::SetWorldMatrix(&world);
	model->Draw();
}

void CarWheel::Load()
{
	model = ResourceManager::AddModel("asset\\models\\taiya01.obj");
	//シェーダー関係
	ResourceManager::GetShaderState(&m_VertexShader, &m_PixelShader, &m_VertexLayout, SHADER_S::NORMAL_FOG);
}