//==============================================================================
// Filename: Timer2D.h
// Description :GameObject�N���X���p���������ԕ\���p�I�u�W�F�N�g�̊Ǘ��N���X�̒�`
//==============================================================================
#pragma once
#include "GameObject.h"
#include "NumberManager.h"
#include "TimeFrame.h"

class Timer2D :
	public GameObject
{
private:
	NumberManager* min;
	NumberManager* sec;
	TimeFrame* Frame;
public:
	void Init();
	void Uninit();
	void Update();
	void Draw() {};
	void SetTime(int InM, int InS);
};
