//==============================================================================
// Filename: Status.h
// Description: コンポーネントクラスを継承したステータスクラスの定義
//==============================================================================
#pragma once
#include "Tools.h"
#include "CComponent.h"
class Status :
	public CComponent
{
private:
	float heal;		//int型のHPを回復するためのfloat
	int HP;			//体力
	float St;		//スタミナ
	int HP_MAX;		//体力の最大値
	float St_MAX;	//スタミナの最大値

	bool AutoHeal;//自己回復するか否か:体力
	float HealTime;//最大値に回復するまでの時間

	bool AutoStHeal;//自己回復するか否か:スタミナ
	float StHealTime;//最大値に回復するまでの時間

	bool AutoBreak;//自己破壊設定

	float speedmax;//最大速度
	float acc;//加速度

	bool Shot;//射撃フラグ

	int wheel;//マウスホイールの移動量
public:
	Status(GameObject* inobject) :CComponent(inobject) {
		heal = 0.f;
		HP = 100;
		St = 100.0f;

		HP_MAX = 100;
		St_MAX = 100.0f;

		AutoHeal = false;;
		HealTime = 0.0f;

		AutoStHeal = true;
		StHealTime = 5.0f;

		AutoBreak = true;

		speedmax = 0.5f;
		acc = speedmax * 0.4f;

		Shot = false;
		wheel = 0;
	}
	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	//定義系
		//最大値の設定：Recover…HP全開するか否か
	void SetMAX(int inHpM = 100, float inStM = 100.0f, bool Recover = true) {
		HP_MAX = inHpM;
		St_MAX = inStM;

		if (!Recover)
			return;

		HP = inHpM;
		St = inStM;
	}
	//HPの自己回復するか否か
	void SetAutoHeal_HP(bool Heal = true, float HPTime = 5.0f)
	{
		AutoHeal = Heal;
		HealTime = HPTime;
	}
	//スタミナの自己回復設定
	void SetAutoHeal_ST(bool Heal = true, float StTime = 5.0f)
	{
		AutoStHeal = Heal;
		StHealTime = StTime;
	}
	//自己破壊の設定
	void SetBreak(bool inBreak = false) {
		AutoBreak = inBreak;
	}

	void SetSpdmax(float inmax = 15.0f)
	{
		speedmax = inmax;
	}
	void SetAcc(float inacc = 0.55f) {
		acc = inacc;
	}

	void SetShot(bool inshot) { Shot = inshot; }

	void SetHP(int hp) {
		HP = hp;
	}
	void SetST(int st)
	{
		St = st;
	}

	//取得系
		//HPの取得
	const int GetHP() { return HP; }
	//HP最大値の取得
	const int GetHPM() { return HP_MAX; }
	//スタミナの取得
	const float GetST() { return St; }
	//スタミナ最大値の取得
	const float GetSTM() { return St_MAX; }
	//生存でTrue
	const bool GetLive() { return HP > 0; }
	//wheelの移動量を取得
	const int GetWheel() {
		return wheel;
	}

	//最大速度の取得
	const float GetSpdmax() { return speedmax; }

	//加速度の取得
	const float GetAcc() { return acc; }

	//射撃フラグの取得
	const bool GetShot() { return Shot; }

	//減算系
	void PullHP(int inHp) { HP -= inHp; }
	void PullST(float inSt) { St -= inSt; }

	//加算系
	void AddHP(int inHp) { HP += inHp; }
	void AddST(float inSt) { St += inSt; }
	void AddWheel(int in) { wheel += in; }
};
