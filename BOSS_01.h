//==============================================================================
// Filename: BOSS_01.h
// Description: BATTLE_DATAクラスを継承したボス戦１クラスの定義
//==============================================================================
#pragma once
#include "BATTLE_DATA.h"
class BOSS_01 :
	public BATTLE_DATA
{
public:
	void Init();
	void Uninit();
	void Update();
};
