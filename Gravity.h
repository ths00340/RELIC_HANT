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
};
