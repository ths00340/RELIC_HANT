//==============================================================================
// Filename: BOSS_01_InEn.h
// Description: BATTLE_DATA�N���X���p�������{�X��P_�G�����N���N���X�̒�`
//==============================================================================
#pragma once
#include "BATTLE_DATA.h"
class BOSS_01_InEn :
	public BATTLE_DATA
{
private:
	bool Once = false;//��񂵂��s�����Ȃ����߂̃t���O
public:
	void Init();
	void Update();
};
