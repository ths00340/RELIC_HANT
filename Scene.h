//==============================================================================
// Filename: Scene.h
// Description :マネージャーの管理用継承元クラスの定義
//==============================================================================
#pragma once

#include <list>
#include <vector>
#include <typeinfo>
#include <thread>
#include "Tools.h"
#include "renderer.h"
#include "GameObject.h"
#include "Player.h"

#define LAYER_NUM (6)

enum class OBJ_LAYER {
	System,		//システム用
	NoCaring,	//視錐台カリングで消さないオブジェクト
	GameObject,	//ゲームオブジェクト
	Enemy,		//敵…管理しやすくするため
	Billboard,	//ビルボード
	UI			//UI周り
};

class Scene
{
protected:
	std::list<GameObject*> g_GameObject[LAYER_NUM];
	D3D11_VIEWPORT view{ (FLOAT)SCREEN_WIDTH ,(FLOAT)SCREEN_HEIGHT,0.0f,1.0f,0,0 };

public:
	Scene() {
		view.Width = (FLOAT)SCREEN_WIDTH;
		view.Height = (FLOAT)SCREEN_HEIGHT;
		view.MinDepth = 0.0f;
		view.MaxDepth = 1.0f;
		view.TopLeftX = 0;
		view.TopLeftY = 0;
	};
	virtual ~Scene() {
		ClearGameObjects();
	};
	virtual void Init()
	{
		isLoad = false;
		view.Width = (FLOAT)SCREEN_WIDTH;
		view.Height = (FLOAT)SCREEN_HEIGHT;
		view.MinDepth = 0.0f;
		view.MaxDepth = 1.0f;
		view.TopLeftX = 0;
		view.TopLeftY = 0;

		TOOL::Display((char*)"使用できるスレッド数=%d\n", std::thread::hardware_concurrency());
	};
	virtual void Uninit()
	{
		int I = 0;
		for (int i = LAYER_NUM - 1; i >= 0; i--)
		{
			for (GameObject* object : g_GameObject[i])
			{
				object->RemoveComponents();

				object->Uninit();
				delete object;
				I++;
			}
			g_GameObject[i].clear();
			I = 0;
		}
	}
	virtual void Update();

	virtual void Draw()
	{
		for (int i = 0; i < LAYER_NUM; i++)
		{
			ObjectDraw(i);
		}
	}

	virtual void UIDraw() {
		ObjectDraw((int)OBJ_LAYER::UI);
	};

	template <typename T>//テンプレート関数
	T* AddGameObject(int Layer)
	{
		T* gameObject = DBG_NEW T();
		GameObject* obj = gameObject;
		gameObject->Init();
		g_GameObject[Layer].push_back(gameObject);
		TOOL::Display("追加：%s\n", obj->GetName());
		return gameObject;
	}

	template <typename T>//テンプレート関数
	T* AddFlontGameObject(int Layer)
	{
		T* gameObject = DBG_NEW T();
		GameObject* obj = gameObject;
		gameObject->Init();
		g_GameObject[Layer].push_front(gameObject);
		TOOL::Display("追加：%s\n", obj->GetName());
		return gameObject;
	}

	template <typename T>
	T* GetGameObject()
	{
		for (int i = 0; i < LAYER_NUM; i++)
			for (GameObject* object : g_GameObject[i])
			{
				if (typeid(*object) == typeid(T))//型を調べる
				{
					return (T*)object;
				}
			}
		return nullptr;
	}

	template <typename T>
	std::vector<T*> GetGameObjects(int a = -1)
	{
		std::vector<T*>objects;

		if (a == -1)
			for (int i = 0; i < LAYER_NUM; i++)
				for (GameObject* object : g_GameObject[i])
				{
					if (!object->GetEnable())
						continue;

					if (typeid(*object) == typeid(T))//型を調べる
					{
						if (object->GetEnable())
						objects.push_back((T*)object);
					}
				}
		else
			for (GameObject* object : g_GameObject[a])
			{
				if (!object->GetEnable())
					continue;

				if (typeid(*object) == typeid(T))//型を調べる
				{
					if (object->GetEnable())
					objects.push_back((T*)object);
				}
			}

		return objects;
	}

	std::vector<GameObject*> GetList(int Layer)
	{
		std::vector<GameObject*>objects;

		for (GameObject* object : g_GameObject[Layer])
		{
				objects.push_back(object);
		}
		return objects;
	}

	const bool GetLiveObj(GameObject* obj, int a = -1)
	{
		if (a == -1)
		{
			for (int i = 0; i < LAYER_NUM; i++)
				for (GameObject* object : g_GameObject[i])
				{
					bool isEnable = object->GetEnable();

					if (!isEnable)
						continue;

					if (object == obj)//型を調べる
					{
						if (!isEnable)
							return false;

						return true;
					}
				}
			return false;
		}
		else
		{
			for (GameObject* object : g_GameObject[a])
			{
				bool isEnable = object->GetEnable();

				if (!isEnable)
					continue;

				if (object == obj)//型を調べる
				{
					if (!isEnable)
						return false;

					return true;
				}
			}
			return false;
		}
	}

	template <typename T>
	std::vector<GameObject*> GetGameObjCmp()
	{
		std::vector<GameObject*>objects;
		for (int i = 0; i < LAYER_NUM; i++)
			for (GameObject* object : g_GameObject[i])
			{
				if (object->LoadComponent<T>())//型を調べる
				{
					objects.push_back(object);
				}
			}
		return objects;
	}

	void SetAllStop(bool stop = true)
	{
		for (int i = 1; i < LAYER_NUM; i++)
			for (GameObject* object : g_GameObject[i])
			{
				object->SetStop(stop);
			}
	}

	D3D11_VIEWPORT* GetView() { return &view; }
	static void Loads();
	static void UnLoads();
	static const bool Getisload() { return isLoad; }

	//指定レイヤーのオブジェクト描画
	void ObjectDraw(int inLayer)
	{
		if (inLayer >= LAYER_NUM)
			return;

		float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };

		for (GameObject* object : g_GameObject[inLayer])//範囲forループ
		{
			if (!object->GetEnable())
				continue;

			if (object->GetBlendState() != NULL)
				Renderer::GetDeviceContext()->OMSetBlendState(object->GetBlendState(), blendFactor, 0xffffffff);

			object->Draw();

			for (CComponent* com : object->GetComponent())
				com->Draw();
		}
	}

	void ObjectDraws(int inStartLayer, int inEndLayer)
	{
		int EndLayer = inEndLayer > inStartLayer ? inEndLayer : inStartLayer;
		int StartLayer = inEndLayer > inStartLayer ? inStartLayer : inEndLayer;

		if (StartLayer < 0)
			return;

		for (int i = StartLayer; i <= EndLayer; i++)
			ObjectDraw(i);
	}

	void ObjectDraws(OBJ_LAYER inStartLayer, OBJ_LAYER inEndLayer)
	{
		int EndLayer = (int)inEndLayer > (int)inStartLayer ? (int)inEndLayer : (int)inStartLayer;
		int StartLayer = (int)inEndLayer > (int)inStartLayer ? (int)inStartLayer : (int)inEndLayer;

		if (StartLayer < 0)
			return;

		for (int i = StartLayer; i <= EndLayer; i++)
			ObjectDraw(i);
	}
private:
	static bool isLoad;

	void ClearGameObjects() {
		for (int i = LAYER_NUM - 1; i >= 0; i--) {
			for (GameObject* object : g_GameObject[i]) {
				object->RemoveComponents();
				object->Uninit();
				delete object;
			}
			g_GameObject[i].clear();
		}
	}
};
