///system
#include "main.h"
#include "renderer.h"
#include "audio.h"
#include "input.h"
#include "manager.h"
#include "model.h"
#include "Tools.h"

///GameObject
#include "Player.h"
#include "ViewCamera.h"
#include "Camera_.h"

///Component
#include "Status.h"
#include "Camera.h"
#include "GameObject.h"
#include "CarMove.h"
#include "Gravity.h"
#include "SphereShadow.h"
#include "Bazooka.h"
#include "ExplodeDome.h"
#include "NormalMove.h"

static ViewCamera* ViewCam = NULL;
static Camera* Cam = NULL;
Scene* scene;

ID3D11VertexShader* Player::m_VertexShader;
ID3D11PixelShader* Player::m_PixelShader;
ID3D11InputLayout* Player::m_VertexLayout;
Model* Player::m_model;
static Float3 Fixdpos;

ID3D11BlendState* Player::blendState;

void Player::Load()
{
	m_model = Manager::AddModel("asset\\models\\hummer_ho.obj");
	Manager::GetShaderState(&m_VertexShader, &m_PixelShader, &m_VertexLayout, SHADER_S::NORMAL_FOG);
	blendState = Manager::GetBlend(BLEND_S::OBJ_OPAQUE);
}

void Player::Init()
{
	name = "Player";
	int hp = 100;

	minsize = m_model->Get_min();
	maxsize = m_model->Get_max();

	sta = AddComponent<Status>();
	sta->SetAutoHeal_ST(true, 20.f);
	Cam = AddComponent<Camera>();
	Cam->SetRange(100.0f);

	switch (Manager::GetWepontype())
	{
	case WEPON_TYPE::GATLING:
		SetWepon<Gatling>();
		break;
	case WEPON_TYPE::LASER:
		SetWepon<ChargeLaser>();
		hp -= 30;
		break;
	case WEPON_TYPE::BAZOOKA:
		SetWepon<Bazooka>();
		hp = 50;
		break;
	case WEPON_TYPE::SHOTGUN_1:
		SetWepon<ShotGun_Physics>();
		hp = 130;
	}

	switch (Manager::GetDSystem())
	{
	case MOVE_TYPE::CAR:
		SetDriveS<CarMove>();
		SetDrive<CarWheel>();
		break;
	case MOVE_TYPE::FREE:
		SetDriveS<NormalMove>();
		SetDrive<Leg_01>();
		hp -= 30;
		break;
	}

	sta->SetMAX(hp);

	AddComponent<Gravity>();
	AddComponent<SphereShadow>();
	AddComponent<HitBox>()->Set(HITBOX_TYPE::CUBE);

	scene = Manager::GetScene();
	m_pos = D3DXVECTOR3(-0.0f, 0.25f, 0.0f);
	m_scl = TOOL::Uniform(0.125f);
	m_rot = TOOL::Uniform();
	Fixdpos = m_pos;
	scene = Manager::GetScene();
	ViewCam = scene->GetGameObject<ViewCamera>();

	m_ShotSE = scene->AddGameObject<Audio>(0);
	m_ShotSE->Load("asset\\SE\\ShotSound_2.wav");

	if (ViewCam != NULL)
		ViewCam->SetView(Cam);
}

void Player::Uninit() {}

struct Floatt3 {
	float x;
	float y;
	float z;
};

void Player::Update()
{
	Idol += TOOL::AToR(30);
	if (Idol > TOOL::AToR(90))
		Idol = 0;

	sta->AddWheel(Input::MouseWheel());
	if (Input::IsMouseRightTriggered())
		if (Cam)
		{
			Cam->FlontTarget<Enemy>();
		}

	if (!m_Destoroy)
		sta->SetShot(Input::IsMouseLeftPressed());

	if (Input::GetKeyTrigger(VK_F5))
		DeleteWepon();

	if (Input::GetKeyTrigger('1'))
		SetWepon<Bazooka>();

	if (Input::GetKeyTrigger('2'))
		SetWepon<ChargeLaser>();

	if (Input::GetKeyTrigger('3'))
		SetWepon<Gatling>();

	if (Input::GetKeyTrigger('4'))
		SetWepon<ShotGun_Physics>();

	if (Input::GetKeyTrigger('5'))
		SetDrive<CarWheel>();

	if (Input::GetKeyTrigger('6'))
		SetDrive<Leg_01>();

	std::vector<GameObject*> enlist = Manager::GetScene()->GetGameObjCmp<Status>();
	enlist = TOOL::WithinTObj(m_pos, (maxsize.z * m_scl.z) * 2.f, enlist);

	for (GameObject* obj : enlist)
	{
		if (obj == this)
			continue;

		float model_Zp = m_model->Get_max().z * m_scl.z;
		float model_Ze = obj->Getmax().z * obj->Getscl().z;
		float model_Z_Total = model_Zp + model_Ze;

		if (TOOL::HitCheck(obj->Getpos(), m_pos, model_Z_Total))
		{
			Float3 vec = obj->Getpos() - m_pos;
			float ran = TOOL::PointRange(obj->Getpos(), m_pos);
			float diff = ran - model_Z_Total;
			vec = TOOL::VectorNormalize(vec);
			vec.y = 0.0f;
			vec = TOOL::VectorNormalize(vec);
			m_pos += vec * diff;
			Damage(1);
		}
	}

	if (LoadComponent<Status>())
	{
		if (Mp)
		{
			Mp->SetScore(sta->GetST());
		}
		if (Hp)
		{
			Hp->SetScore(sta->GetHP());
		}
	}
}

void Player::Draw()
{
	//入力レイアウト設定
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	//シェーダー入力
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	//マトリクス設定
	D3DXMATRIX world, scl, rot, trans;
	D3DXMatrixScaling(&scl, m_scl.x, m_scl.y, m_scl.z);
	D3DXMatrixRotationYawPitchRoll(&rot, m_rot.y + m_addrot.y, m_rot.x + m_addrot.x, m_rot.z + m_addrot.z);
	D3DXMatrixTranslation(&trans, m_pos.x + m_addpos.x, m_pos.y + m_addpos.y + (sinf(Idol) * 0.05f * m_scl.y), m_pos.z + m_addpos.z);
	world = scl * rot * trans;
	Renderer::SetWorldMatrix(&world);
	m_model->Draw();
}

void Player::Damage(int dmg)
{
	if (Cam)
		Cam->SetShake(15, 0.8f);

	if (sta)
		sta->AddHP(-dmg);

	if (Hp)
	{
		Hp->Shake(10, 100.f);
		Hp->SetScore(sta->GetHP());
	}
}

void Player::Finish()
{
	float maxshake = 60.f;
	Scene* scene = Manager::GetScene();
	scene->AddGameObject<ExplodeDome>((int)OBJ_LAYER::GameObject)->Set(m_pos, 7.5, 0.2f, 0);
	Cam->SetShake();

	SetDestroy();
}

void Player::SetHP(int Inhp)
{
	if (Hp == nullptr)
	{
		Hp = Manager::GetScene()->AddGameObject<NumberManager>((int)OBJ_LAYER::UI);
		Hp->SetStatus(Float2(80.f, 80.f), Float2(230.f, 40.f), true);
		Hp->SetScore(Inhp);
	}
}

void Player::DeleteHP()
{
	if (Hp)
	{
		Hp->SetDestroy();
		Hp = nullptr;
	}
}

void Player::SetMP()
{
	if (Mp == nullptr)
	{
		Mp = Manager::GetScene()->AddGameObject<NumberManager>((int)OBJ_LAYER::UI);
		Mp->SetStatus(Float2(80.f, 80.f), Float2(230.f, 130.f), true);
	}
}

void Player::FixedUpdate()
{
	GameObject::FixedUpdate();

	GameObject* obj = Cam->GetTarget();

	if (obj != NULL)
		if (!obj->LoadComponent<Status>()->GetLive())
			Cam->FlontTarget<Enemy>();
}