//==============================================================================
// Filename: TargetCom.h
// Description: �R���|�[�l���g�N���X���p�����������Ώۂ̓o�^�p�R���|�[�l���g�̒�`
//==============================================================================
#pragma once
#include "CComponent.h"
class TargetCom :
	public CComponent
{
public:
	TargetCom(GameObject* inobject) :CComponent(inobject) {};
	void Init();
	void Uninit();
	void OnEnable();
	void OnDisable();
};
