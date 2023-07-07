//==============================================================================
// Filename: Scene.h
// Description :�}�l�[�W���[�̊Ǘ��p�p�����N���X�̒�`
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
	System,		//�V�X�e���p
	NoCaring,	//������J�����O�ŏ����Ȃ��I�u�W�F�N�g
	GameObject,	//�Q�[���I�u�W�F�N�g
	Enemy,		//�G�c�Ǘ����₷�����邽��
	Billboard,	//�r���{�[�h
	UI			//UI����
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

		TOOL::Display((char*)"�g�p�ł���X���b�h��=%d\n", std::thread::hardware_concurrency());
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

	template <typename T>//�e���v���[�g�֐�
	T* AddGameObject(int Layer)
	{
		T* gameObject = DBG_NEW T();
		GameObject* obj = gameObject;
		gameObject->Init();
		g_GameObject[Layer].push_back(gameObject);
		TOOL::Display("�ǉ��F%s\n", obj->GetName());
		return gameObject;
	}

	template <typename T>//�e���v���[�g�֐�
	T* AddFlontGameObject(int Layer)
	{
		T* gameObject = DBG_NEW T();
		GameObject* obj = gameObject;
		gameObject->Init();
		g_GameObject[Layer].push_front(gameObject);
		TOOL::Display("�ǉ��F%s\n", obj->GetName());
		return gameObject;
	}

	template <typename T>
	T* GetGameObject()
	{
		for (int i = 0; i < LAYER_NUM; i++)
			for (GameObject* object : g_GameObject[i])
			{
				if (typeid(*object) == typeid(T))//�^�𒲂ׂ�
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

					if (typeid(*object) == typeid(T))//�^�𒲂ׂ�
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

				if (typeid(*object) == typeid(T))//�^�𒲂ׂ�
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

					if (object == obj)//�^�𒲂ׂ�
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

				if (object == obj)//�^�𒲂ׂ�
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
				if (object->LoadComponent<T>())//�^�𒲂ׂ�
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

	//�w�背�C���[�̃I�u�W�F�N�g�`��
	void ObjectDraw(int inLayer)
	{
		if (inLayer >= LAYER_NUM)
			return;

		float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };

		for (GameObject* object : g_GameObject[inLayer])//�͈�for���[�v
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
