//==============================================================================
// Filename: Gravity.h
// Description: �R���|�[�l���g�N���X���p�������d�̓N���X�̒�`
//==============================================================================
#pragma once
#include "CComponent.h"

class Field;

class Gravity :
	public CComponent
{
private:
	float groundHeight = 0.0f;//���n����n�ʂ̍���
	float gravity = GRAVITY;//���t���[�����Z����鉺�����ւ̗�
	Field* field = nullptr;//�ׂ��������蔻��p�t�B�[���h
	bool isGround = false;//�����������ǂ���
	Float3 m_Normal = Float3(0.f, 0.f, 0.f);
public:
	Gravity(GameObject* inobject) :CComponent(inobject) {
		groundHeight = 0.0f;
		gravity = GRAVITY;
	};

	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;
	void Set(float ingra) { gravity = ingra; }//�d�͒l�ݒ�
	const bool GetisGround() { return isGround; }
	const Float3 GetGroundNormal() { return m_Normal; }
};
