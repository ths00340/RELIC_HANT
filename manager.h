//==============================================================================
// Filename: manager.h
// Description: マネージャークラスの定義
//==============================================================================
#pragma once
#include <unordered_map>
#include <iostream>
#include "Scene.h"
#include "Result_S.h"
#include "Game.h"
#include "MainMenu.h"
#include "model.h"
#include "Common.h"
#include "ResourceManager.h"

//武器種
enum class WEPON_TYPE
{
	GATLING,
	LASER,
	BAZOOKA,
	SHOTGUN_1,
	NONE,
};

//移動種
enum class MOVE_TYPE
{
	CAR,
	FREE
};

//シーンの統括マネージャー
class Manager
{
private:
	static class Scene* NowScene;
	static std::list<Scene*> addScene;
	static class Common* common;

	///※別クラスに移動の方が良し
	/// プレイヤー管理----------------------------
	//移動種
	static MOVE_TYPE legtype;

	//武器種
	static WEPON_TYPE wepon;
	///-------------------------------------------

public:
	//初期化
	static void Init();

	//終了処理
	static void Uninit();

	//更新処理
	static void Update();

	//描画処理
	static void Draw();

	//現在使用しているシーンの取得
	static class Scene* GetScene() { return NowScene; };

	//シーンの設定
	template<typename T>
	static void SetScene()
	{
		if (NowScene)
		{
			NowScene->Uninit();
			delete NowScene;
			DeleteAddScenes();
		}

		NowScene = DBG_NEW T();
		NowScene->Init();
	};

	//フェードシーン設定
	template<typename T>
	static Common* SetCommon()
	{
		if (common == nullptr)
		{
			common = DBG_NEW T();
			common->Init();
		}

		return common;
	}

	//フェードシーンの取得
	static Common* GetCommon() { return common; };

	/// プレイヤー管理----------------------------
	//移動種の取得
	static MOVE_TYPE GetDSystem() { return legtype; }

	//武器種の取得
	static WEPON_TYPE GetWepontype() { return wepon; }

	//移動種の設定
	static void SetDsystem(MOVE_TYPE in) { legtype = in; }

	//武器種の設定
	static void SetWeponType(WEPON_TYPE in) { wepon = in; }
	///-------------------------------------------

	//追加シーンの追加
	template<typename T>
	static T* AddScene()
	{
		T* scene = DBG_NEW T();
		scene->Init();
		addScene.push_back(scene);
		return scene;
	};

	//指定した追加シーンの取得
	template <typename T>
	static T* GetAddScene()
	{
		for (Scene* sce : addScene)
		{
			if (typeid(*sce) == typeid(T))//型を調べる
			{
				return (T*)sce;
			}
		}
		return nullptr;
	}

	//指定した追加シーンの削除
	template<typename T>
	static const bool DeleteAddScene()
	{
		for (Scene* sce : addScene)
		{
			if (typeid(*sce) == typeid(T))
			{
				sce->Uninit();
				addScene.remove(sce);
				return true;
			}
		}
		return false;
	}

	//追加シーンの一括削除
	static void DeleteAddScenes()
	{
		std::list<Scene*>::iterator it = addScene.begin();
		for (; it != addScene.end(); ++it) {
			delete* it;
		}
		addScene.clear();
	}
};
