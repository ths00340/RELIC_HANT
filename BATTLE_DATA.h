//==============================================================================
// Filename: BATTLE_DATA.h
// Description: �~�b�V��������ۑ�����BATTLE_DATA�N���X�̒�`
//==============================================================================
#pragma once
#include "main.h"

class Timer2D;
class TimeStr;
class GameObject;
class MissionTex;
class Scene;

//���ԊǗ��p�̍\���̂̒�`

class BATTLE_DATA
{
protected:
	Scene* sce;
	int EnemyNum = 0;//��������G�̐�
	Float3 ClearPos = Float3(0.0f, 0.0f, 0.0f);//�N���A���W
	Float3 ClearSize = Float3(1.0f, 1.0f, 1.0f);//�N���A���W�͈̔�
	TimeStr* Endurance;//�ϋv����(-1�Ȃ疳��)
	GameObject* Target = nullptr;//���j�ڕW
	bool Clear = false;//�N���A�t���O
	bool GameOver = false;//�Q�[���I�[�o�[�t���O
	Timer2D* timer = nullptr;//�^�C�}�[�`��p�I�u�W�F�N�g
	MissionTex* mtex = nullptr;//�~�b�V�����̏ڍ׊Ŕ�
	bool Start;//�X�^�[�g�t���O
	float ptime = 0.0f;		//���Ԍ��Z�p
public:
	//������
	virtual void Init() = 0;

	//�I������
	virtual void Uninit();

	//�X�V����
	virtual void Update();

	//�ŏ��̓���
	virtual void Begin() {};

	//�N���A�����̔�r
	virtual void ClearObserver() {};

	//���Ԃ̐i�s
	virtual void IsTimeLimit();

	//�V�[���Ǝ��̒ǉ��s��
	virtual void ExtraMove() {};

	//�N���A�t���O�̎擾
	bool GetClear() { return Clear; }

	//�Q�[���I�[�o�[�t���O�̎擾
	bool GetOver() { return GameOver; }

	//�����ڕW�̎擾
	GameObject* GetTarget();

	//���ԍ\���̂̎擾
	TimeStr* GetTime();

	//���Ԃ̒ǉ�
	void AddTime(int sec);

	//���ԍ\���̂̐ݒ�
	void SetTime(int sec);
};
