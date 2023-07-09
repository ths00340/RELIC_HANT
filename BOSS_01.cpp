#include "manager.h"
#include "Scene.h"
#include "Tools.h"
#include "EnemyPool.h"
#include "Enemy.h"
#include "BOSS_01.h"
#include "Stage03.h"
#include "TimeStr.h"
#include "Timer2D.h"
#include "ShotGun_Physics.h"
#include "TargetCom.h"

void BOSS_01::Init()
{
	BATTLE_DATA::Init();

	//���݂̃V�[���̓o�^
	sce = Manager::GetScene();

	//�w�ߏ��̓o�^
	mtex = sce->AddGameObject<Stage03>((int)OBJ_LAYER::UI);

	m_pPool = sce->GetGameObject<EnemyPool>(OBJ_LAYER::System);
	m_pPool->Set(1);

	//�{�X�G�l�~�[�̐ݒ�
	{
		Enemy* en = NULL;
		en = m_pPool->Recycle();
		if (en)
		{
			en->SetScl(TOOL::Uniform(1.f));
			en->LoadComponent<Status>()->SetMAX(500);
			en->AddComponent<ShotGun_Physics>();
			en->SetShader(SHADER_S::LIGHT_LIM);
			en->AddComponent<TargetCom>();
		}
	}

	//���ԊǗ��I�u�W�F�N�g�̐���
	Endurance = DBG_NEW TimeStr();
	*Endurance = 300;//�������Ԃ�300�b�ɐݒ�
	ptime = 0.0f;

	//���ԕ\���p�I�u�W�F�N�g�𐶐�
	timer = sce->AddGameObject<Timer2D>((int)OBJ_LAYER::UI);
	timer->SetTime(Endurance->Min, Endurance->Sec);

	//�X�^�[�g�p�֐���false��
	Start = false;
}

void BOSS_01::Begin()
{
	if (mtex->GetEnd())
	{
		sce->SetAllStop(false);
		Start = true;
	}
	else
	{
		sce->SetAllStop();
		mtex->SetStop(false);
	}
}

void BOSS_01::ClearObserver()
{
	if (TargetList->Empty())
	{
		Clear = true;
	}

	if (!Manager::GetScene()->GetGameObject<Player>() || Endurance <= 0)
	{
		GameOver = true;
	}
}