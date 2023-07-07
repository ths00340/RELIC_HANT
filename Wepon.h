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
	float angle = 0.f;
	int dmg = 1;
	float time = 0.f;//���Ԍv���p
	Float3 ShotAngle;//���ˊp�x
	Float3 shotpos;//���ˍ��W

	Status* objS = NULL;
	Audio* shot = NULL;
	float range;//�˒�����
	float ReCast;
public:
	Wepon(GameObject* inobject) :CComponent(inobject) {
		m_pos = { 0.f,0.f,0.f };
		m_rot = { 0.f,0.f,0.f };
		m_scl = { 1.f,1.f,1.f };
		angle = 0.f;
		dmg = 1;
		time = 0.f;
		ShotAngle = { 0.f,0.f,0.f };
		shotpos = { 0.f,0.f,0.f };
		range = 60.f;
		ReCast = 3.f;

		objS = NULL;
		shot = NULL;
	};
};
