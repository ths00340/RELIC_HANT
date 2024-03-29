///system
#include "main.h"
#include "renderer.h"
#include "audio.h"
#include "input.h"
#include "manager.h"
#include "model.h"
#include "Tools.h"

///GameObject
#include "GameObject.h"
#include "Player.h"
#include "ViewCamera.h"
#include "Camera_.h"
#include "NumberManager.h"
#include "Enemy.h"
#include "BreakCam.h"

///Component
#include "Status.h"
#include "Camera.h"
#include "CarMove.h"
#include "Gravity.h"
#include "SphereShadow.h"
#include "Bazooka.h"
#include "ExplodeDome.h"
#include "NormalMove.h"
#include "AttitudeControl.h"
#include "ShotGun_Physics.h"
#include "Gatling.h"
#include "ChargeLaser.h"
#include "CarWheel.h"
#include "Leg_01.h"
#include "HitBox.h"
#include "cInputOperation.h"

void Player::Init()
{
	name = "Player";

	m_model = ResourceManager::AddModel("asset\\models\\hummer_ho.obj");
	ResourceManager::GetShaderState(&m_VertexShader, &m_PixelShader, &m_VertexLayout, SHADER_S::NORMAL_TOON);
	blendState = ResourceManager::GetBlend(BLEND_S::OBJ_OPAQUE);

	int hp = 100;
	minsize = m_model->Get_min();
	maxsize = m_model->Get_max();

	m_pInput = AddComponent<cInputOperation>();
	m_pSta = AddComponent<Status>();
	m_pCam = AddComponent<Camera>();
	m_pCam->SetRange(100.0f);

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
	case WEPON_TYPE::NONE:
		hp = 50;
		break;
	}

	switch (Manager::GetDSystem())
	{
	case MOVE_TYPE::CAR:
		SetDriveS<CarMove>();
		SetDrive<CarWheel>();
		AddComponent<AttitudeControl>();
		break;
	case MOVE_TYPE::FREE:
		SetDriveS<NormalMove>();
		SetDrive<Leg_01>();
		hp -= 30;
		break;
	}

	m_pSta->SetMAX(hp);

	AddComponent<Gravity>();
	AddComponent<HitBox>()->Set(HITBOX_TYPE::CUBE);

	m_pScene = Manager::GetScene();
	m_pos = D3DXVECTOR3(-0.0f, 0.25f, 0.0f);
	m_scl = TOOL::Uniform(0.125f);
	m_rot = TOOL::Uniform();
	m_pScene = Manager::GetScene();
	ViewCamera* pViewCam = m_pScene->GetGameObject<ViewCamera>();

	m_pShotSE = m_pScene->AddGameObject<Audio>(0);
	m_pShotSE->Load("asset\\SE\\ShotSound_2.wav");

	if (pViewCam != NULL)
		pViewCam->SetView(m_pCam);
}

void Player::Uninit() {}

struct Floatt3 {
	float x;
	float y;
	float z;
};

void Player::Update()
{
	m_Idol += TOOL::AToR(30);
	if (m_Idol > TOOL::AToR(90))
		m_Idol = 0;

	m_pSta->AddWheel(Input::MouseWheel());

	if (m_pInput)
	{
		if (m_pInput->GetAds())
			if (m_pCam)
			{
				m_pCam->FlontTarget<Enemy>();
			}

		if (!m_Destroy)
			m_pSta->SetShot(m_pInput->GetShot());
	}

	//デバック用処理
	{
		if (Input::GetKeyTrigger(DIK_F5))
		{
			DeleteWepon();
			Manager::SetWeponType(WEPON_TYPE::NONE);
		}

		if (Input::GetKeyTrigger(DIK_1))
		{
			SetWepon<Bazooka>();
			Manager::SetWeponType(WEPON_TYPE::BAZOOKA);
		}

		if (Input::GetKeyTrigger(DIK_2))
		{
			SetWepon<ChargeLaser>();
			Manager::SetWeponType(WEPON_TYPE::LASER);
		}

		if (Input::GetKeyTrigger(DIK_3))
		{
			SetWepon<Gatling>();
			Manager::SetWeponType(WEPON_TYPE::GATLING);
		}

		if (Input::GetKeyTrigger(DIK_4))
		{
			SetWepon<ShotGun_Physics>();
			Manager::SetWeponType(WEPON_TYPE::SHOTGUN_1);
		}

		if (Input::GetKeyTrigger(DIK_5))
		{
			Manager::SetDsystem(MOVE_TYPE::CAR);
			SetDriveS<CarMove>();
			SetDrive<CarWheel>();
		}

		if (Input::GetKeyTrigger(DIK_6))
		{
			Manager::SetDsystem(MOVE_TYPE::FREE);
			SetDrive<Leg_01>();
			SetDriveS<NormalMove>();
		}
	}

	{
		//ステータスを持っているオブジェクトの取得
		std::vector<GameObject*> enlist = Manager::GetScene()->GetGameObjCmp<Status>();

		//enlistの範囲内に居るオブジェクトの取得
		enlist = TOOL::WithinObj(m_pos, (maxsize.z * m_scl.z) * 2.f, enlist);

		for (GameObject* obj : enlist)
		{
			if (!obj->GetEnable())
				continue;

			if (obj == this)
				continue;

			float model_Zp = m_model->Get_max().z * m_scl.z;	//プレイヤーモデルのZ軸の現在の大きさ
			float model_Ze = obj->Getmax().z * obj->Getscl().z;	//エネミーモデルのZ軸の現在の大きさ
			float model_Z_Total = model_Zp + model_Ze;			//エネミーとプレイヤーの半径の合計

			if (TOOL::CanHit(obj->Getpos(), m_pos, model_Z_Total))//model_Z_Totalより直線距離が小さいとTrue
			{
				Float3 vec = obj->Getpos() - m_pos;
				float ran = TOOL::PointRange(obj->Getpos(), m_pos);
				float diff = ran - model_Z_Total;

				vec = TOOL::VectorNormalize(vec);
				m_pos += vec * diff;//当たった範囲分弾かれる
				Damage(1);//衝突ダメージ
			}
		}
	}

	//ステータスコンポーネントを持っているなら
	if (m_pSta != nullptr)
	{
		//MP表示用UIのポインタを持っているなら
		if (m_pMp != nullptr)
		{
			m_pMp->SetScore(m_pSta->GetST());
		}
		//HP表示用UIのポインタを持っているなら
		if (m_pHp != nullptr)
		{
			m_pHp->SetScore(m_pSta->GetHP());
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

	ID3D11ShaderResourceView* DepthTexture = Renderer::GetShadowDepthTexture();
	Renderer::GetDeviceContext()->PSSetShaderResources(1, 1, &DepthTexture);

	//マトリクス設定
	D3DXMATRIX world, scl, rot, trans;
	D3DXMatrixScaling(&scl, m_scl.x, m_scl.y, m_scl.z);
	D3DXMatrixRotationYawPitchRoll(&rot, m_rot.y + m_addrot.y, m_rot.x + m_addrot.x, m_rot.z + m_addrot.z);
	D3DXMatrixTranslation(&trans, m_pos.x + m_addpos.x, m_pos.y + m_addpos.y + (sinf(m_Idol) * 0.05f * m_scl.y), m_pos.z + m_addpos.z);
	world = scl * rot * trans;
	Renderer::SetWorldMatrix(&world);
	m_model->Draw();
}

void Player::Damage(int dmg)
{
	if (m_pCam)
		m_pCam->SetShakeRot(0.25f, TOOL::AToR(3.f));

	if (m_pSta)
		m_pSta->AddHP(-dmg);

	if (m_pHp)
	{
		m_pHp->Shake(10, 100.f);
		m_pHp->SetScore(m_pSta->GetHP());
	}
}

void Player::Finish()
{
	float maxshake = 60.f;
	Scene* scene = Manager::GetScene();
	scene->AddGameObject<ExplodeDome>((int)OBJ_LAYER::GameObject)->Set(m_pos, 7.5, 0.2f, 0);
	m_pCam->SetShakePos();
	BreakCam* bCam = scene->AddGameObject<BreakCam>((int)OBJ_LAYER::GameObject);
	bCam->Set(m_pCam->GetPos(), m_pCam->GetUp(), m_pCam->GetTag());
	SetDestroy();
}

void Player::SetHP(int Inhp)
{
	if (m_pHp == nullptr)
	{
		m_pHp = Manager::GetScene()->AddGameObject<NumberManager>((int)OBJ_LAYER::UI);
		m_pHp->SetStatus(Float2(80.f, 80.f), Float2(230.f, 40.f), true);
		m_pHp->SetScore(Inhp);
	}
}

void Player::DeleteHP()
{
	if (m_pHp)
	{
		m_pHp->SetDestroy();
		m_pHp = nullptr;
	}
}

void Player::SetMP()
{
	if (m_pMp == nullptr)
	{
		m_pMp = Manager::GetScene()->AddGameObject<NumberManager>((int)OBJ_LAYER::UI);
		m_pMp->SetStatus(Float2(80.f, 80.f), Float2(230.f, 130.f), true);
	}
}

void Player::FixedUpdate()
{
	GameObject::FixedUpdate();

	GameObject* obj = m_pCam->GetTarget();

	if (obj != NULL)
		if (obj->GetDestory())
			m_pCam->FlontTarget<Enemy>();
}