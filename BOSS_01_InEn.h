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

public:
	void Init();
	void Begin();
	void ClearObserver();
	void ExtraMove();
};
