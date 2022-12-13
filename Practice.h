//==============================================================================
// Filename: Practice.h
// Description: BATTLE_DATAクラスを継承した練習用クラスの定義
//==============================================================================
#pragma once
#include "BATTLE_DATA.h"
class Practice :
	public BATTLE_DATA
{
	bool Once = true;
	float ClTime = 0.0f;//終了用変数
public:
	void Init();
	void Begin();
	void ClearObserver();
};
