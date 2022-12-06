//==============================================================================
// Filename: BATTLE_DATA.h
// Description: ミッション情報を保存するBATTLE_DATAクラスの定義
//==============================================================================
#pragma once
#include "main.h"
#include "Scene.h"

class Timer2D;
class TimeStr;
class GameObject;
class MissionTex;

//時間管理用の構造体の定義

class BATTLE_DATA
{
protected:
	int EnemyNum = 0;//生成する敵の数
	Float3 ClearPos = Float3(0.0f, 0.0f, 0.0f);//クリア座標
	Float3 ClearSize = Float3(1.0f, 1.0f, 1.0f);//クリア座標の範囲
	TimeStr* Endurance;//耐久時間(-1なら無限)
	GameObject* Target = nullptr;//撃破目標
	bool Clear = false;//クリアフラグ
	bool GameOver = false;//ゲームオーバーフラグ
	Timer2D* timer;//タイマー描画用オブジェクト
	MissionTex* mtex;//ミッションの詳細看板
	bool Start;//スタートフラグ
	float ptime = 0.0f;		//時間減算用

	//virtual void StartUp();
	//virtual void ClearObserver();
public:
	//初期化
	virtual void Init() = 0;

	//終了処理
	virtual void Uninit();

	//更新処理
	virtual void Update() = 0;

	//クリアフラグの取得
	bool GetClear() { return Clear; }

	//ゲームオーバーフラグの取得
	bool GetOver() { return GameOver; }

	//討伐目標の取得
	GameObject* GetTarget();

	//時間構造体の取得
	TimeStr* GetTime();

	//時間の追加
	void AddTime(int sec);

	//時間構造体の設定
	void SetTime(int sec);
};
