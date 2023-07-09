//==============================================================================
// Filename: BATTLE_DATA.h
// Description: ミッション情報を保存するBATTLE_DATAクラスの定義
//==============================================================================
#pragma once
#include "main.h"
#include <list>

class Timer2D;
class TimeStr;
class GameObject;
class MissionTex;
class Scene;
class EnemyPool;

//リスト管理用クラス
class Object_List {
private:
	std::list<GameObject*>List;
	int MaxCount = 0;
public:
	Object_List() {
		MaxCount = 0;
	}
	~Object_List() {}

	//登録
	const bool AttachTarget(GameObject* inObj) {
		List.push_back(inObj);
		MaxCount += 1;
		return true;
	}

	//解除
	const bool DetachTarget(GameObject* inObj) {
		for (GameObject* obj : List)
		{
			if (obj == inObj)
			{
				List.remove(inObj);
				return true;
			}
		}
		return false;
	}

	//現在登録されているオブジェクトの数を返す
	const int GetListCount() {
		return List.size();
	}

	//今まで登録されたオブジェクトの数を返す
	const int GetMaxCount()
	{
		return MaxCount;
	}

	//指定クラスのリストを返す
	template <typename T>
	const std::list<T*> GetSpecObjectList()
	{
		std::list<T*> RetList;
		for (GameObject* object : List)
		{
			if (typeid(*object) == typeid(T))
			{
				RetList.push_back(object);
			}
		}
		return RetList;
	}

	//生存確認用...使わない可能性大
	const bool isLiveObject(GameObject* inObj)
	{
		for (GameObject* object : List)
		{
			if (object == inObj)
			{
				return true;
			}
		}
		return false;
	}

	const bool Empty()
	{
		return List.empty();
	}

	//空かどうか
	explicit operator bool() const
	{
		return List.empty();
	}
};

class BATTLE_DATA
{
protected:
	Scene* sce;

	Float3 ClearPos = Float3(0.0f, 0.0f, 0.0f);	//クリア座標
	Float3 ClearSize = Float3(1.0f, 1.0f, 1.0f);//クリア座標の範囲

	int EnemyNum = 0;					//生成する敵の数
	Object_List* TargetList = nullptr;	//撃破目標

	TimeStr* Endurance = nullptr;	//耐久時間(-1なら無限)
	Timer2D* timer = nullptr;		//タイマー描画用オブジェクト
	MissionTex* mtex = nullptr;		//ミッションの詳細看板
	EnemyPool* m_pPool = nullptr;		//エネミー用のオブジェクトプール

	float ptime = 0.0f;				//時間減算用
	float ClTime = 0.0f;			//キープ時間計測用

	bool Start;						//スタートフラグ
	bool Clear = false;				//クリアフラグ
	bool GameOver = false;			//ゲームオーバーフラグ
	bool Once = false;				//特殊行動用フラグ

public:
	//初期化
	virtual void Init() {
		TargetList = DBG_NEW Object_List();
	};

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
	const bool GetClear() { return Clear; }

	//ゲームオーバーフラグの取得
	const bool GetOver() { return GameOver; }

	//討伐目標の取得
	Object_List* GetTargetList()
	{
		return TargetList;
	}

	//時間構造体の取得
	TimeStr* GetTime();

	//時間の追加
	void AddTime(int sec);

	//時間構造体の設定
	void SetTime(int sec);
};
