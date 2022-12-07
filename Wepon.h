//==============================================================================
// Filename: Wepon.h
// Description: �R���|�[�l���g�N���X�p����������p�N���X�̒�`
//==============================================================================
#pragma once
#include "main.h"
#include "CComponent.h"

class Status;
class Audio;

class Wepon :
	public CComponent
{
protected:
	Float3 m_pos;//���W(�e�I�u�W�F�N�g�ɑ��ΓI�Ȉʒu)
	Float3 m_rot;//�p�x(�e�I�u�W�F�N�g�ɑ��ΓI�Ȋp�x)
	Float3 m_scl;//�T�C�Y(�e�I�u�W�F�N�g�ɑ��ΓI�ȃT�C�Y)
	float angle = 0.0f;
	int dmg = 1;
	int time = 0;//���Ԍv���p
	Float3 ShotAngle;//���ˊp�x
	Float3 shotpos;//���ˍ��W

	Status* objS = NULL;
	Audio* shot = NULL;
	float range;//�˒�����
	float ReCast;
public:
	Wepon(GameObject* inobject) :CComponent(inobject) {
		m_pos = { 0.0f,0.0f,0.0f };
		m_rot = { 0.0f,0.0f,0.0f };
		m_scl = { 1.0f,1.0f,1.0f };
		angle = 0.f;
		dmg = 1;
		time = 0;
		ShotAngle = { 0.0f,0.0f,0.0f };
		shotpos = { 0.0f,0.0f,0.0f };
		range = 60.f;
		ReCast = 3.f;

		objS = NULL;
		shot = NULL;
	};
};
