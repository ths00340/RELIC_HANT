//==============================================================================
// Filename: Practice.h
// Description: BATTLE_DATA�N���X���p���������K�p�N���X�̒�`
//==============================================================================
#pragma once
#include "BATTLE_DATA.h"
class Practice :
	public BATTLE_DATA
{
	bool Once = true;
	float ClTime = 0.0f;//�I���p�ϐ�
public:
	void Init();
	void Begin();
	void ClearObserver();
};
