//==============================================================================
// Filename: cInputOperation.h
// Description: コンポーネントクラスを継承した操作入力の定義
//==============================================================================
#pragma once
#include "CComponent.h"
class cInputOperation :
	public CComponent
{
public:
	cInputOperation(GameObject* inobject) :CComponent(inobject) {}
	void Init();
	void Uninit();
	void Update();
	//左スティック+WASDの入力値
	Float2 GetMov() { return m_Mov; };

	//右スティック+マウス
	Float2 GetPov() { return m_Pov; };

	//射撃キーが押されているか否か
	bool GetShot();

	//覗き(ロックオン)キーが押されているか否か
	bool GetAds();

	//オプションキーが押されているか否か
	bool GetOption();

	//決定キーが押されているか否か
	bool GetOK();

	//拒否キーが押されているか否か
	bool GetCancel();

	//ジャンプキーが押されているか否か
	bool GetJump();

private:
	int m_PadNum = 0;//割り振られたパッド
	Float2 m_Pov;//視点
	Float2 m_Mov;//移動
};
