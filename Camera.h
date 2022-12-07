//==============================================================================
// Filename: Camera.h
// Description: �R���|�[�l���g�N���X�p�������J�����N���X�̒�`
//==============================================================================
#pragma once
#include <typeinfo>
#include "manager.h"
#include "Tools.h"
#include "CComponent.h"

class GameObject;

class Camera :public CComponent
{
private:
	CamMode Mode;//FPP��TPP��
	Float3 m_pos;//���W
	Float3 m_tag;//�����_
	Float3 m_up;//���_
	D3DXMATRIX m_ViewMatrix;//���_�s��
	Float3 Dir;//���W���璍���_�ւ̃x�N�g��
	Float3 Angle;//�p�x
	Float3 Addpos;//�ǉ����W
	float ViewAng;//����p
	GameObject* tag = NULL;//�^�[�Q�b�g�I�u�W�F�N�g
	float Range;//�擾�͈�
	
	/// �h��p�ϐ�
	bool shake = false;
	int  time = 0;
	int	 maxrand = 0;
	float shakeStr = 1.0f;

public:

	Camera(GameObject* inobject) :CComponent(inobject) {
		m_pos = Float3(0.0f, 3.0f, -5.0f);
		m_tag = TOOL::Uniform();
		m_up = Float3(0.0f, 1.0f, 0.0f);
		Dir = TOOL::Uniform();
		Angle = TOOL::Uniform();
		Addpos = TOOL::Uniform();
		ViewAng = 1.0f;
		Range = 30.0f;
		Mode = CamMode::TPP;
		shake = false;
		time = 0;
		maxrand = 0;
	}
	void Init();
	void Uninit();
	void Update();
	void Draw();

	//��ԉ�ʒ����ɋ߂��G���^�[�Q�b�g����
	template <typename T>
	T* FlontTarget() {
		if (Mode == CamMode::FPP)
			return NULL;

		T* rettag = NULL;
		Scene* scene = Manager::GetScene();

		//�^�[�Q�b�e�B���O
		if (!tag)
		{
			if (TOOL::FlontNearestPos<T>(object->Getpos(), Dir, Range, scene->GetGameObjects<T>()))
			{
				rettag = TOOL::FlontNearestPos<T>(object->Getpos(), Dir, Range, scene->GetGameObjects<T>());
				tag = rettag;
			}
		}
		else
		{
			tag = NULL;
		}
		return rettag;
	}

	void SetRange(float inRange) { Range = inRange; }
	void SetTag(D3DXVECTOR3 intag);
	void SetUp(D3DXVECTOR3 inup);
	void SetPos(Float3 inpos) { m_pos = inpos; };
	void SetMode(CamMode InMode)
	{
		Mode = InMode;
	};
	void SetTarget(GameObject* intag)
	{
		if (tag == NULL)
			tag = intag;
	}
	void SetShake(int max = 60, float Str = 1.f);

	GameObject* GetTarget() { return tag; }
	const Float3 GetDir();
	const Float3 GetAngle() { return Angle; };
	const Float3 GetTag() { return m_tag; }
	const Float3 GetUp() { return m_up; }
	const Float3 GetPos() { return m_pos; }
	const CamMode GetMode() { return Mode; }
	const float GetViewAng() { return ViewAng; }
	const D3DXMATRIX GetViewMatrix() { return m_ViewMatrix; }

	//�|�C���^�n��
	Float3* GetTagPoint() { return &m_tag; }
	Float3* GetUpPoint() { return &m_up; }
	Float3* GetPosPoint() { return &m_pos; }
	D3DXMATRIX* GetViewMatrixPoint() { return &m_ViewMatrix; }
};
