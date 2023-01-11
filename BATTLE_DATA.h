//==============================================================================
// Filename: BATTLE_DATA.h
// Description: ミッション情報を保存するBATTLE_DATAクラスの定義
//==============================================================================
#pragma once
#include "main.h"

class Timer2D;
class TimeStr;
class GameObject;
class MissionTex;
class Scene;

//時間管理用の構造体の定義

class BATTLE_DATA
{
protected:
	Scene* sce;
	int EnemyNum = 0;//生成する敵の数
	Float3 ClearPos = Float3(0.0f, 0.0f, 0.0f);//クリア座標
	Float3 ClearSize = Float3(1.0f, 1.0f, 1.0f);//クリア座標の範囲
	TimeStr* Endurance;//耐久時間(-1なら無限)
	GameObject* Target = nullptr;//撃破目標
	bool Clear = false;//クリアフラグ
	bool GameOver = false;//ゲームオーバーフラグ
	Timer2D* timer = nullptr;//タイマー描画用オブジェクト
	MissionTex* mtex = nullptr;//ミッションの詳細看板
	bool Start;//スタートフラグ
	float ptime = 0.0f;		//時間減算用
public:
	//初期化
	virtual void Init() = 0;

	//終了処理
	virtual void Uninit();

	//更新処理
	virtual void Update();

	//最初の動作
	virtual void Begin() {};

	//クリア条件の比較
	virtual void ClearObserver() {};

	//時間の進行
	virtual void IsTimeLimit();

	//シーン独自の追加行動
	virtual void ExtraMove() {};

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
