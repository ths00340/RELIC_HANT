//==============================================================================
// Filename: BATTLE_DATA.h
// Description: �~�b�V��������ۑ�����BATTLE_DATA�N���X�̒�`
//==============================================================================
#pragma once
#include "main.h"
#include "Scene.h"
#include "GameObject.h"
#include "Timer2D.h"
#include "MissionTex.h"

//���ԊǗ��p�̍\���̂̒�`
struct TimeStr {
	int Hour = 0;
	int Min = 0;
	int Sec = 0;
	TimeStr() {
		Hour = 0;
		Min = 0;
		Sec = 0;
	};

	TimeStr(int h, int m, int s) {
		Hour = h;
		Min = m;
		Sec = s;
	}

	//�b���w��
	TimeStr& operator=(int sec)
	{
		Hour = sec / 3600;
		Min = (sec % 3600) / 60;
		Sec = sec % 60;
		return *this;
	}

	//�b�������炷
	TimeStr& operator-=(int sec)
	{
		int total = (Hour * 3600) + (Min * 60) + Sec;
		total -= sec;
		TimeStr a;
		a = total;
		this->Hour = a.Hour;
		this->Min = a.Min;
		this->Sec = a.Sec;
		return *this;
	}

	//�b�������炷
	TimeStr& operator-(int sec)
	{
		TimeStr ret;

		int total = (Hour * 3600) + (Min * 60) + Sec;
		total -= sec;
		ret = total;
		this->Hour = ret.Hour;
		this->Min = ret.Min;
		this->Sec = ret.Sec;

		return *this;
	}

	//�b���̒ǉ�
	TimeStr& operator+(int sec)
	{
		TimeStr ret;

		int total = (Hour * 3600) + (Min * 60) + Sec;
		total += sec;
		ret = total;
		this->Hour = ret.Hour;
		this->Min = ret.Min;
		this->Sec = ret.Sec;

		return *this;
	}

	//�S�Ă̎��Ԃ�b���ŕԋp
	int GetTotal() {
		return (Hour * 3600) + (Min * 60) + Sec;
	}

	//�������Ԃ���r�i���\���́j
	bool operator==(TimeStr a)
	{
		return GetTotal() == a.GetTotal();
	}

	//�������Ԃ���r�i�b���j
	bool operator==(int a)
	{
		return GetTotal() == a;
	}

	//a�Ɣ�r�i�b���j
	bool operator>=(int a)
	{
		return GetTotal() >= a;
	}

	//a�Ɣ�r�i�b���j
	bool operator>(int a)
	{
		return GetTotal() > a;
	}

	//a�Ɣ�r�i�b���j
	bool operator<=(int a)
	{
		return GetTotal() <= a;
	}

	//a�Ɣ�r�i�b���j
	bool operator<(int a)
	{
		return GetTotal() < a;
	}
};

class BATTLE_DATA
{
protected:
	int EnemyNum = 0;//��������G�̐�
	Float3 ClearPos = Float3(0.0f, 0.0f, 0.0f);//�N���A���W
	Float3 ClearSize = Float3(1.0f, 1.0f, 1.0f);//�N���A���W�͈̔�
	TimeStr Endurance;//�ϋv����(-1�Ȃ疳��)
	GameObject* Target = nullptr;//���j�ڕW
	bool Clear = false;//�N���A�t���O
	bool GameOver = false;//�Q�[���I�[�o�[�t���O
	Timer2D* timer;//�^�C�}�[�`��p�I�u�W�F�N�g
	MissionTex* mtex;//�~�b�V�����̏ڍ׊Ŕ�
	bool Start;//�X�^�[�g�t���O
	float ptime = 1.0f;		//���Ԍ��Z�p
public:
	//������
	virtual void Init() = 0;

	//�I������
	virtual void Uninit() = 0;

	//�X�V����
	virtual void Update() = 0;

	//�N���A�t���O�̎擾
	bool GetClear() { return Clear; }

	//�Q�[���I�[�o�[�t���O�̎擾
	bool GetOver() { return GameOver; }

	//�����ڕW�̎擾
	GameObject* GetTarget() {
		if (Target)
			return Target;
		else
			return nullptr;
	}

	//���ԍ\���̂̎擾
	TimeStr GetTime() {
		return Endurance;
	}

	//���Ԃ̒ǉ�
	void AddTime(int sec)
	{
		Endurance + sec;
	}

	//���ԍ\���̂̐ݒ�
	void SetTime(int sec)
	{
		Endurance = sec;
	}
};
