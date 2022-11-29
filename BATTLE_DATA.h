//==============================================================================
// Filename: BATTLE_DATA.h
// Description: ミッション情報を保存するBATTLE_DATAクラスの定義
//==============================================================================
#pragma once
#include "main.h"
#include "Scene.h"
#include "GameObject.h"
#include "Timer2D.h"
#include "MissionTex.h"

//時間管理用の構造体の定義
struct TimeStr {
	int Hour = 0;
	int Min = 0;
	int Sec = 0;
	TimeStr() {
		Hour = 0;
		Min = 0;
		Sec = 0;
	};

	TimeStr(int h, int m, int s) {
		Hour = h;
		Min = m;
		Sec = s;
	}

	//秒数指定
	TimeStr& operator=(int sec)
	{
		Hour = sec / 3600;
		Min = (sec % 3600) / 60;
		Sec = sec % 60;
		return *this;
	}

	//秒数を減らす
	TimeStr& operator-=(int sec)
	{
		int total = (Hour * 3600) + (Min * 60) + Sec;
		total -= sec;
		TimeStr a;
		a = total;
		this->Hour = a.Hour;
		this->Min = a.Min;
		this->Sec = a.Sec;
		return *this;
	}

	//秒数を減らす
	TimeStr& operator-(int sec)
	{
		TimeStr ret;

		int total = (Hour * 3600) + (Min * 60) + Sec;
		total -= sec;
		ret = total;
		this->Hour = ret.Hour;
		this->Min = ret.Min;
		this->Sec = ret.Sec;

		return *this;
	}

	//秒数の追加
	TimeStr& operator+(int sec)
	{
		TimeStr ret;

		int total = (Hour * 3600) + (Min * 60) + Sec;
		total += sec;
		ret = total;
		this->Hour = ret.Hour;
		this->Min = ret.Min;
		this->Sec = ret.Sec;

		return *this;
	}

	//全ての時間を秒数で返却
	int GetTotal() {
		return (Hour * 3600) + (Min * 60) + Sec;
	}

	//同じ時間か比較（同構造体）
	bool operator==(TimeStr a)
	{
		return GetTotal() == a.GetTotal();
	}

	//同じ時間か比較（秒数）
	bool operator==(int a)
	{
		return GetTotal() == a;
	}

	//aと比較（秒数）
	bool operator>=(int a)
	{
		return GetTotal() >= a;
	}

	//aと比較（秒数）
	bool operator>(int a)
	{
		return GetTotal() > a;
	}

	//aと比較（秒数）
	bool operator<=(int a)
	{
		return GetTotal() <= a;
	}

	//aと比較（秒数）
	bool operator<(int a)
	{
		return GetTotal() < a;
	}
};

class BATTLE_DATA
{
protected:
	int EnemyNum = 0;//生成する敵の数
	Float3 ClearPos = Float3(0.0f, 0.0f, 0.0f);//クリア座標
	Float3 ClearSize = Float3(1.0f, 1.0f, 1.0f);//クリア座標の範囲
	TimeStr Endurance;//耐久時間(-1なら無限)
	GameObject* Target = nullptr;//撃破目標
	bool Clear = false;//クリアフラグ
	bool GameOver = false;//ゲームオーバーフラグ
	Timer2D* timer;//タイマー描画用オブジェクト
	MissionTex* mtex;//ミッションの詳細看板
	bool Start;//スタートフラグ
	float ptime = 1.0f;		//時間減算用
public:
	//初期化
	virtual void Init() = 0;

	//終了処理
	virtual void Uninit() = 0;

	//更新処理
	virtual void Update() = 0;

	//クリアフラグの取得
	bool GetClear() { return Clear; }

	//ゲームオーバーフラグの取得
	bool GetOver() { return GameOver; }

	//討伐目標の取得
	GameObject* GetTarget() {
		if (Target)
			return Target;
		else
			return nullptr;
	}

	//時間構造体の取得
	TimeStr GetTime() {
		return Endurance;
	}

	//時間の追加
	void AddTime(int sec)
	{
		Endurance + sec;
	}

	//時間構造体の設定
	void SetTime(int sec)
	{
		Endurance = sec;
	}
};
