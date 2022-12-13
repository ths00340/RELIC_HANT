//==============================================================================
// Filename: Status.h
// Description: �R���|�[�l���g�N���X���p�������X�e�[�^�X�N���X�̒�`
//==============================================================================
#pragma once
#include "Tools.h"
#include "CComponent.h"
class Status :
	public CComponent
{
private:
	float heal;		//int�^��HP���񕜂��邽�߂�float
	int HP;			//�̗�
	float St;		//�X�^�~�i
	int HP_MAX;		//�̗͂̍ő�l
	float St_MAX;	//�X�^�~�i�̍ő�l

	bool AutoHeal;//���ȉ񕜂��邩�ۂ�:�̗�
	float HealTime;//�ő�l�ɉ񕜂���܂ł̎���

	bool AutoStHeal;//���ȉ񕜂��邩�ۂ�:�X�^�~�i
	float StHealTime;//�ő�l�ɉ񕜂���܂ł̎���

	bool AutoBreak;//���Ȕj��ݒ�

	float speedmax;//�ő呬�x
	float acc;//�����x

	bool Shot;//�ˌ��t���O

	int wheel;//�}�E�X�z�C�[���̈ړ���
public:
	Status(GameObject* inobject) :CComponent(inobject) {
		heal = 0.f;
		HP = 100;
		St = 100.0f;

		HP_MAX = 100;
		St_MAX = 100.0f;

		AutoHeal = false;;
		HealTime = 0.0f;

		AutoStHeal = true;
		StHealTime = 5.0f;

		AutoBreak = true;

		speedmax = 0.5f;
		acc = speedmax * 0.4f;

		Shot = false;
		wheel = 0;
	}
	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	//��`�n
		//�ő�l�̐ݒ�FRecover�cHP�S�J���邩�ۂ�
	void SetMAX(int inHpM = 100, float inStM = 100.0f, bool Recover = true) {
		HP_MAX = inHpM;
		St_MAX = inStM;

		if (!Recover)
			return;

		HP = inHpM;
		St = inStM;
	}
	//HP�̎��ȉ񕜂��邩�ۂ�
	void SetAutoHeal_HP(bool Heal = true, float HPTime = 5.0f)
	{
		AutoHeal = Heal;
		HealTime = HPTime;
	}
	//�X�^�~�i�̎��ȉ񕜐ݒ�
	void SetAutoHeal_ST(bool Heal = true, float StTime = 5.0f)
	{
		AutoStHeal = Heal;
		StHealTime = StTime;
	}
	//���Ȕj��̐ݒ�
	void SetBreak(bool inBreak = false) {
		AutoBreak = inBreak;
	}

	void SetSpdmax(float inmax = 15.0f)
	{
		speedmax = inmax;
	}
	void SetAcc(float inacc = 0.55f) {
		acc = inacc;
	}

	void SetShot(bool inshot) { Shot = inshot; }

	void SetHP(int hp) {
		HP = hp;
	}
	void SetST(int st)
	{
		St = st;
	}

	//�擾�n
		//HP�̎擾
	const int GetHP() { return HP; }
	//HP�ő�l�̎擾
	const int GetHPM() { return HP_MAX; }
	//�X�^�~�i�̎擾
	const float GetST() { return St; }
	//�X�^�~�i�ő�l�̎擾
	const float GetSTM() { return St_MAX; }
	//������True
	const bool GetLive() { return HP > 0; }
	//wheel�̈ړ��ʂ��擾
	const int GetWheel() {
		return wheel;
	}

	//�ő呬�x�̎擾
	const float GetSpdmax() { return speedmax; }

	//�����x�̎擾
	const float GetAcc() { return acc; }

	//�ˌ��t���O�̎擾
	const bool GetShot() { return Shot; }

	//���Z�n
	void PullHP(int inHp) { HP -= inHp; }
	void PullST(float inSt) { St -= inSt; }

	//���Z�n
	void AddHP(int inHp) { HP += inHp; }
	void AddST(float inSt) { St += inSt; }
	void AddWheel(int in) { wheel += in; }
};
