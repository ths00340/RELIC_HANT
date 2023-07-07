#include "main.h"
#include "renderer.h"
#include "manager.h"
#include "GameObject.h"
#include "Tools.h"
#include "Camera.h"
#include "Status.h"
#include "ChargeLaser.h"
#include "LaserBullet.h"
#include "ChargeEffect.h"

void ChargeLaser::Init()
{
	model = ResourceManager::AddModel("asset\\models\\laserTullet.obj");
	predictionLine = ResourceManager::AddModel("asset\\models\\laser01.obj");
	//�V�F�[�_�[�֌W
	ResourceManager::GetShaderState(&m_VertexShader, &m_PixelShader, &m_VertexLayout, SHADER_S::NORMAL_FOG);
	charge_rate = 0.8f;

	eff = nullptr;

	ShotAngle = TOOL::Uniform();
	shotpos = TOOL::Uniform();
	range = 80.f;
}

void ChargeLaser::Uninit()
{
	if (model)
		model = nullptr;

	if (predictionLine)
		predictionLine = nullptr;

	if (eff)
		eff = nullptr;

	if (m_VertexLayout)
		m_VertexLayout = nullptr;

	if (m_VertexShader)
		m_VertexShader = nullptr;

	if (m_PixelShader)
		m_PixelShader = nullptr;
}

void ChargeLaser::Update()
{
	objS = object->LoadComponent<Status>();

	Camera* cam = object->LoadComponent<Camera>();
	m_scl = object->Getscl();
	Scene* scene = Manager::GetScene();
	angle = cam->GetAngle().x;
	angle = TOOL::Limit(angle, 0.0f, -TOOL::AToR(60.0f));

	ShotAngle = m_rot;

	if (cam->GetMode() == CamMode::FPP)
		ShotAngle = object->Getrot();

	Float3 add = object->Getaddrot();
	ShotAngle.x += angle;// +add.x;

	if (cam->GetMode() == CamMode::TPP)
		ShotAngle.y = object->LoadComponent<Camera>()->GetAngle().y;
	else
	{
		ShotAngle = object->Getrot();
		ShotAngle.y += add.y;
	}

	ShotAngle.z += add.z;

	shotpos = (object->Getpos() + TOOL::GetForward(ShotAngle) * (model->Get_max().z * m_scl.z));
	shotpos += TOOL::GetUp(ShotAngle) * ((model->Get_max().y * m_scl.y) + (3.5f * m_scl.y)) + object->Getvec();

	if (objS)
	{
		if (time > charge_rate)
		{
			time = 0.f;

			LaserBullet* blt = scene->AddGameObject<LaserBullet>((int)OBJ_LAYER::GameObject);

			blt->Set(shotpos, ShotAngle, 5, range);
			blt->SetScl(Float3(4.0f * m_scl.z, 4.0f * m_scl.z, blt->Getscl().z));

			eff = nullptr;
		}

		if (objS->GetShot())
		{
			time+=Renderer::GetDeltaTime();
			if (eff == nullptr)
			{
				eff = scene->AddGameObject<ChargeEffect>((int)OBJ_LAYER::Billboard);
				eff->Set(0.8f);
			}
			eff->SetScl(TOOL::Uniform(4.0f * m_scl.z));
			eff->SetPos(shotpos);
		}
		else
		{
			time = 0.f;
			if (eff != nullptr)
			{
				eff->SetDestroy();
				eff = nullptr;
			}
		}
	}
}

void ChargeLaser::Draw()
{
	Camera* cam = object->LoadComponent<Camera>();
	//���̓��C�A�E�g�ݒ�
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	//�V�F�[�_�[����
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	//�}�g���N�X�ݒ�
	D3DXMATRIX world, scl, rot, trans;
	D3DXMatrixScaling(&scl, m_scl.x, m_scl.y, m_scl.z);

	if (cam->GetMode() == CamMode::TPP)
		D3DXMatrixRotationYawPitchRoll(&rot,
			cam->GetAngle().y,
			object->Getrot().x + object->Getaddrot().x + angle,
			object->Getrot().z + object->Getaddrot().z);
	else
		D3DXMatrixRotationYawPitchRoll(&rot,
			object->Getrot().y + object->Getaddrot().y,
			object->Getrot().x + object->Getaddrot().x,
			object->Getrot().z + object->Getaddrot().z);

	Float3 TrueF3 = TOOL::GetUp(
		Float3(object->Getrot().x + object->Getaddrot().x,
			object->Getrot().y + object->Getaddrot().y,
			object->Getrot().z + object->Getaddrot().z))
		* (object->Getmax().y * object->Getscl().y);

	D3DXMatrixTranslation(&trans, object->Getpos().x + TrueF3.x, object->Getpos().y + TrueF3.y, object->Getpos().z + TrueF3.z);
	world = scl * rot * trans;
	Renderer::SetWorldMatrix(&world);
	model->Draw();

	D3DXMatrixScaling(&scl, m_scl.x, m_scl.y, TOOL::Magni_one(predictionLine->Get_max().z, range));
	D3DXMatrixRotationYawPitchRoll(&rot, ShotAngle.y, ShotAngle.x, ShotAngle.z);
	D3DXMatrixTranslation(&trans, shotpos.x, shotpos.y, shotpos.z);
	world = scl * rot * trans;
	Renderer::SetWorldMatrix(&world);

	if (objS)
		predictionLine->Draw();
}

void ChargeLaser::Load()
{
	ResourceManager::AddModel("asset\\models\\laserTullet.obj");
	ResourceManager::AddModel("asset\\models\\laser01.obj");
}