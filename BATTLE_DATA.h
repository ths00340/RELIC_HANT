//==============================================================================
// Filename: BATTLE_DATA.h
// Description: �~�b�V��������ۑ�����BATTLE_DATA�N���X�̒�`
//==============================================================================
#pragma once
#include "main.h"
#include <list>

class Timer2D;
class TimeStr;
class GameObject;
class MissionTex;
class Scene;
class EnemyPool;

//���X�g�Ǘ��p�N���X
class Object_List {
private:
	std::list<GameObject*>List;
	int MaxCount = 0;
public:
	Object_List() {
		MaxCount = 0;
	}
	~Object_List() {}

	//�o�^
	const bool AttachTarget(GameObject* inObj) {
		List.push_back(inObj);
		MaxCount += 1;
		return true;
	}

	//����
	const bool DetachTarget(GameObject* inObj) {
		for (GameObject* obj : List)
		{
			if (obj == inObj)
			{
				List.remove(inObj);
				return true;
			}
		}
		return false;
	}

	//���ݓo�^����Ă���I�u�W�F�N�g�̐���Ԃ�
	const int GetListCount() {
		return List.size();
	}

	//���܂œo�^���ꂽ�I�u�W�F�N�g�̐���Ԃ�
	const int GetMaxCount()
	{
		return MaxCount;
	}

	//�w��N���X�̃��X�g��Ԃ�
	template <typename T>
	const std::list<T*> GetSpecObjectList()
	{
		std::list<T*> RetList;
		for (GameObject* object : List)
		{
			if (typeid(*object) == typeid(T))
			{
				RetList.push_back(object);
			}
		}
		return RetList;
	}

	//�����m�F�p...�g��Ȃ��\����
	const bool isLiveObject(GameObject* inObj)
	{
		for (GameObject* object : List)
		{
			if (object == inObj)
			{
				return true;
			}
		}
		return false;
	}

	const bool Empty()
	{
		return List.empty();
	}

	//�󂩂ǂ���
	explicit operator bool() const
	{
		return List.empty();
	}
};

class BATTLE_DATA
{
protected:
	Scene* sce;

	Float3 ClearPos = Float3(0.0f, 0.0f, 0.0f);	//�N���A���W
	Float3 ClearSize = Float3(1.0f, 1.0f, 1.0f);//�N���A���W�͈̔�

	int EnemyNum = 0;					//��������G�̐�
	Object_List* TargetList = nullptr;	//���j�ڕW

	TimeStr* Endurance = nullptr;	//�ϋv����(-1�Ȃ疳��)
	Timer2D* timer = nullptr;		//�^�C�}�[�`��p�I�u�W�F�N�g
	MissionTex* mtex = nullptr;		//�~�b�V�����̏ڍ׊Ŕ�
	EnemyPool* m_pPool = nullptr;		//�G�l�~�[�p�̃I�u�W�F�N�g�v�[��

	float ptime = 0.0f;				//���Ԍ��Z�p
	float ClTime = 0.0f;			//�L�[�v���Ԍv���p

	bool Start;						//�X�^�[�g�t���O
	bool Clear = false;				//�N���A�t���O
	bool GameOver = false;			//�Q�[���I�[�o�[�t���O
	bool Once = false;				//����s���p�t���O

public:
	//������
	virtual void Init() {
		TargetList = DBG_NEW Object_List();
	};

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
	const bool GetClear() { return Clear; }

	//�Q�[���I�[�o�[�t���O�̎擾
	const bool GetOver() { return GameOver; }

	//�����ڕW�̎擾
	Object_List* GetTargetList()
	{
		return TargetList;
	}

	//���ԍ\���̂̎擾
	TimeStr* GetTime();

	//���Ԃ̒ǉ�
	void AddTime(int sec);

	//���ԍ\���̂̐ݒ�
	void SetTime(int sec);
};
