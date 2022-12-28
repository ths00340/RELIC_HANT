#pragma once

#include <list>
#include <vector>
#include <typeinfo>
#include <thread>
#include "GameObject.h"
#include "bullet.h"
#include "Enemy.h"
#include "ParticleManager.h"
#include "ParticleTest.h"
#include "Skybox.h"
#include "Explosion.h"
#include "Player.h"
#include "Polygon2D.h"
#include "Tree.h"
#include "Platform.h"
#include "renderer.h"
#include "Leg_01.h"
#include "SphereShadow.h"
#include "Bazooka.h"
#include "ExplosiveBullet.h"
#include "HitBox.h"
#include "CarWheel.h"
#include "ExplodeDome.h"
#include "ChargeLaser.h"
#include "LaserBullet.h"
#include "ChargeEffect.h"
#include "Gatling.h"
#include "ShutterFade.h"
#include "NormalFade.h"
#include "Fissure.h"
#include "ShotGun_Physics.h"

#define LAYER_NUM (6)

enum class OBJ_LAYER {
	System,
	NoCaring,
	GameObject,
	Enemy,
	Billboard,
	UI
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
	virtual ~Scene() {};
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
		for (int i = 0; i < LAYER_NUM; i++)
		{
			for (GameObject* object : g_GameObject[i])
			{
				object->RemoveComponents();

				object->Uninit();
				delete object;
			}
			g_GameObject[i].clear();
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

	template <typename T>//テンプレート関数
	T* AddGameObject(int Layer)
	{
		T* gameObject = new T();
		GameObject* obj = gameObject;
		gameObject->Init();
		g_GameObject[Layer].push_back(gameObject);
		return gameObject;
	}

	template <typename T>//テンプレート関数
	T* AddFlontGameObject(int Layer)
	{
		T* gameObject = new T();
		GameObject* obj = gameObject;
		gameObject->Init();
		g_GameObject[Layer].push_front(gameObject);
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
		return NULL;
	}

	template <typename T>
	std::vector<T*> GetGameObjects(int a = -1)
	{
		std::vector<T*>objects;

		if (a == -1)
			for (int i = 0; i < LAYER_NUM; i++)
				for (GameObject* object : g_GameObject[i])
				{
					if (typeid(*object) == typeid(T))//型を調べる
					{
						objects.push_back((T*)object);
					}
				}
		else
			for (GameObject* object : g_GameObject[a])
			{
				if (typeid(*object) == typeid(T))//型を調べる
				{
					objects.push_back((T*)object);
				}
			}

		return objects;
	}

	std::vector<GameObject*> GetList(int Layer)
	{
		std::vector<GameObject*>objects;

		for (GameObject* object : g_GameObject[Layer])
			objects.push_back(object);

		return objects;
	}

	const bool GetLiveObj(GameObject* obj, int a = -1)
	{
		if (a == -1)
		{
			for (int i = 0; i < LAYER_NUM; i++)
				for (GameObject* object : g_GameObject[i])
				{
					if (object == obj)//型を調べる
					{
						return true;
					}
				}
			return false;
		}
		else
		{
			for (GameObject* object : g_GameObject[a])
			{
				if (object == obj)//型を調べる
				{
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

		objects.clear();

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
	static void Loads()
	{
		Bullet::Load();
		ParticleManager::Load();
		Player::Load();
		Enemy::Load();
		Skybox::Load();
		Explosion::Load();
		Tree::Load();
		Platform::Load();
		Leg_01::Load();
		SphereShadow::Load();
		Bazooka::Load();
		ExplosiveBullet::Load();
		HitBox::Load();
		CarWheel::Load();
		ExplodeDome::Load();
		ChargeLaser::Load();
		LaserBullet::Load();
		ChargeEffect::Load();
		Gatling::Load();
		ShutterFade::Load();
		NormalFade::Load();
		Fissure::Load();
		ShotGun_Physics::Load();

		isLoad = true;
	}
	static void UnLoads()
	{
		Explosion::UnLoad();
		Tree::UnLoad();
		SphereShadow::UnLoad();
		ChargeEffect::UnLoad();
		ShutterFade::UnLoad();
		NormalFade::UnLoad();
		Fissure::UnLoad();
	}
	static const bool Getisload() { return isLoad; }
private:
	static bool isLoad;
	void ObjectDraw(int inLayer)
	{
		if (inLayer >= LAYER_NUM)
			return;

		float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };

		for (GameObject* object : g_GameObject[inLayer])//範囲forループ
		{
			if (object->GetBlendState() != NULL)
				Renderer::GetDeviceContext()->OMSetBlendState(object->GetBlendState(), blendFactor, 0xffffffff);

			object->Draw();

			for (CComponent* com : object->GetComponent())
				com->Draw();
		}
	}
};
