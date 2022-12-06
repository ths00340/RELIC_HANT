//==============================================================================
// Filename: BOSS_01_InEn.h
// Description: BATTLE_DATAクラスを継承したボス戦１_敵無限湧きクラスの定義
//==============================================================================
#pragma once
#include "BATTLE_DATA.h"
class BOSS_01_InEn :
	public BATTLE_DATA
{
private:
	bool Once = false;//一回しか行動しないためのフラグ
public:
	void Init();
	void Update();
};
