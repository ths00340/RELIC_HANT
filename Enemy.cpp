#include "main.h"
#include "renderer.h"
#include "model.h"
#include "GameObject.h"
#include "Enemy.h"
#include "CarMove.h"
#include "Camera.h"
#include "audio.h"
#include "ViewCamera.h"
#include "manager.h"
#include "Player.h"
#include "ParticleManager.h"
#include "Leg_01.h"
#include "Bazooka.h"
#include "HitBox.h"
#include "Gravity.h"
#include "SphereShadow.h"
#include "ExplodeDome.h"

void Enemy::Init()
{
	//���f���̌Ăяo��
	m_model = ResourceManager::AddModel("asset\\models\\CamBot_E01.obj");

	//�V�F�[�_�[�֌W
	ResourceManager::GetShaderState(&m_VertexShader, &m_PixelShader, &m_VertexLayout, SHADER_S::LIGHT_OFF);

	//�u�����h�X�e�[�g�̐ݒ�1
	blendState = ResourceManager::GetBlend(BLEND_S::OBJ_OPAQUE);

	//���O�̓o�^
	name = "Enemy";

	//�I�u�W�F�N�g�̎��ۂ̃T�C�Y�̓o�^
	minsize = m_model->Get_min();//�ŏ��|�C���g
	maxsize = m_model->Get_max();//�ő�|�C���g

	//�J�����R���|�[�l���g�̓o�^
	Camera* cam;
	cam = AddComponent<Camera>();
	cam->SetMode(CamMode::FPP);
	//�J�����̎��F�����̓o�^
	cam->SetRange(320.0f);

	//�X�e�[�^�X�R���|�[�l���g�̓o�^
	status = AddComponent<Status>();
	status->SetMAX(10);
	status->SetBreak(true);
	time = 0;

	//���̑��R���|�[�l���g�̓o�^&�����ݒ�
	AddComponent<Gravity>();
	AddComponent<Leg_01>();
	AddComponent<HitBox>()->Set(HITBOX_TYPE::SPHERE);

	//�I�u�W�F�N�g�̏����ݒ�
	m_pos = Float3(0.f, 100.f, 0.f);
	m_scl = Float3(0.25f, 0.25f, 0.25f);
	m_rot = Float3(0.f, 0.f, 0.f);

#ifndef MUTE
	hit = Manager::GetScene()->AddGameObject<Audio>(2);
	hit->Load("asset\\SE\\hit01.wav");
#endif //MUTE

	//�v���C���[�̒T��
	Scene* scene = Manager::GetScene();
	if (scene != NULL)
		tag = scene->GetGameObject<Player>();

	//�ړ����x�������ŗ����Ōv�Z
	spd = TOOL::RandF() * 20.0f + 10.f;
}

void Enemy::Uninit()
{
#ifndef MUTE
	if (hit)
		hit->SetDeath();
#endif //MUTE
}

void Enemy::Update()
{
	Camera* cam = NULL;
	cam = LoadComponent<Camera>();
	if (m_pos.y <= fabsf(m_model->Get_min().y * m_scl.y))
	{
		m_pos.y = fabsf(m_model->Get_min().y * m_scl.y);
	}

	time++;
	Scene* scene = Manager::GetScene();
	if (tag != NULL)
	{
		if (cam->GetTarget() != NULL)
		{
			Float3 a = cam->GetTarget()->Getpos() - m_pos;
			float angle = TOOL::SubAngle(a, GetForward());
			if (fabsf(angle) > TOOL::SecDiv(TOOL::AToR(180.0f)))
			{
				if (angle < 0.0f)
					angle = TOOL::SecDiv(-TOOL::AToR(30.0f));
				else
					angle = TOOL::SecDiv(TOOL::AToR(30.0f));
			}
			m_rot.y += angle;

			status->SetShot(time > TOOL::FrameMulti(FireRate));

			if (time > TOOL::FrameMulti(FireRate))
			{
				time = 0;
				FireRate = (TOOL::RandF() * 2.0f) + 1.0f;
			}
			else
				if (time < TOOL::FrameMulti(FireRate * 0.85f))
					m_pos += GetForward() * TOOL::SecDiv(m_scl.z * spd);
				else
				{
					if (fabsf(angle) > TOOL::SecDiv(TOOL::AToR(180.0f)))
					{
						if (angle < 0.0f)
							angle = TOOL::SecDiv(-TOOL::AToR(1800.0f));
						else
							angle = TOOL::SecDiv(TOOL::AToR(1800.0f));
					}
				}
		}
		else
			cam->SetTarget(tag);
	}
	else
	{
		if (scene != NULL)
			tag = scene->GetGameObject<Player>();
	}
}

void Enemy::Draw()
{
	//���̓��C�A�E�g�ݒ�
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	//�V�F�[�_�[����
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	//�}�g���N�X�ݒ�
	D3DXMATRIX world, scl, rot, trans;
	D3DXMatrixScaling(&scl, m_scl.x, m_scl.y, m_scl.z);
	D3DXMatrixRotationYawPitchRoll(&rot, m_rot.y + m_addrot.y, m_rot.x + m_addrot.x, m_rot.z + m_addrot.z);
	D3DXMatrixTranslation(&trans, m_pos.x + m_addpos.x, m_pos.y + m_addpos.y, m_pos.z + m_addpos.z);
	world = scl * rot * trans;
	Renderer::SetWorldMatrix(&world);
	m_model->Draw();
}

void Enemy::Load()
{
	ResourceManager::AddModel("asset\\models\\CamBot_E01.obj");
}

void Enemy::Damage(int dmg)
{
	if (status)
		status->PullHP(dmg);
}

void Enemy::PlayHit()
{
#ifndef MUTE
	int a = 0;
	Player* pl = Manager::GetScene()->GetGameObject<Player>();
	Camera* cam = Manager::GetScene()->GetGameObject<ViewCamera>()->GetView();
	if (pl)
	{
		Float3 dir = m_pos - pl->Getpos();
		a = hit->Play(false, 0.1f);
		hit->SetPan(
			0.5f - (sinf(TOOL::SubAngle(cam->GetDir(), dir)) / 2.0f),
			0.5f + (sinf(TOOL::SubAngle(cam->GetDir(), dir)) / 2.0f)
			, a);
	}
#endif // !MUTE
}

void Enemy::Finish()
{
	ParticleManager* mana;

	Scene* scene = Manager::GetScene();
	scene->AddGameObject<ExplodeDome>((int)OBJ_LAYER::GameObject)->Set(m_pos, m_model->Get_max().z * m_scl.z, 0.2f, 0);

	mana = scene->AddGameObject<ParticleManager>(1);
	mana->SetModel(PARTICLE_S::ITEM2, BLEND_S::OBJ_TRANS);
	mana->Set(m_pos, TOOL::Uniform(), 2.0f * m_scl.z, TOOL::RandF() * 15 + 5 + (50 * m_scl.z), GRAVITY, 2, m_scl * 0.5f);

	mana = scene->AddGameObject<ParticleManager>(1);
	mana->SetModel(PARTICLE_S::ITEM3, BLEND_S::OBJ_TRANS);
	mana->Set(m_pos, TOOL::Uniform(), 3.0f * m_scl.z, TOOL::RandF() * 10 + (5 * m_scl.z), GRAVITY, 2, m_scl * 0.5f);

	mana = scene->AddGameObject<ParticleManager>(1);
	mana->SetModel(PARTICLE_S::GIA, BLEND_S::OBJ_OPAQUE);
	mana->Set(m_pos, TOOL::Uniform(), 0.5f * m_scl.z, TOOL::RandF() * 10 + (5 * m_scl.z), GRAVITY, 2, m_scl * 0.5f);

	mana = scene->AddGameObject<ParticleManager>(1);
	mana->SetModel(PARTICLE_S::CUBE, BLEND_S::OBJ_TRANS);
	mana->Set(m_pos,				//���W
		TOOL::Uniform(),	//�p�x
		2.0f * m_scl.z,				//�T�C�Y
		TOOL::RandF() * 15 + 5 + (50 * m_scl.z),//��
		GRAVITY,					//�d�͒l
		2,							//��������
		m_scl * 0.5f,
		{ 3.f,3.f,3.f });				//�T�C�Y

	SetDestroy();
}