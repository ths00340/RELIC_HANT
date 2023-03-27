//==============================================================================
// Filename: AttitudeControl.h
// Description: �R���|�[�l���g�N���X���p�������ȈՓI�Ȏp������̒�`
//==============================================================================
#pragma once
#include "main.h"
#include "CComponent.h"
class AttitudeControl :
	public CComponent
{
private:
	Float3 FixedPos = { 0.f,0.f,0.f };
	int time = 0;
public:
	AttitudeControl(GameObject* inobject) :CComponent(inobject)
	{
		FixedPos = { 0.f,0.f,0.f };
		time = 0;
	}
	void Init();
	void Update();
	void Uninit() {};
	void Draw() {};
};
