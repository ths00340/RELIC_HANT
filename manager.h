//==============================================================================
// Filename: manager.h
// Description: �}�l�[�W���[�N���X�̒�`
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

//�����
enum class WEPON_TYPE
{
	GATLING,
	LASER,
	BAZOOKA,
	SHOTGUN_1,
	NONE,
};

//�ړ���
enum class MOVE_TYPE
{
	CAR,
	FREE
};

//�V�[���̓����}�l�[�W���[
class Manager
{
private:
	static class Scene* NowScene;
	static std::list<Scene*> addScene;
	static class Common* common;

	///���ʃN���X�Ɉړ��̕����ǂ�
	/// �v���C���[�Ǘ�----------------------------
	//�ړ���
	static MOVE_TYPE legtype;

	//�����
	static WEPON_TYPE wepon;
	///-------------------------------------------

public:
	//������
	static void Init();

	//�I������
	static void Uninit();

	//�X�V����
	static void Update();

	//�`�揈��
	static void Draw();

	//���ݎg�p���Ă���V�[���̎擾
	static class Scene* GetScene() { return NowScene; };

	//�V�[���̐ݒ�
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

	//�t�F�[�h�V�[���ݒ�
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

	//�t�F�[�h�V�[���̎擾
	static Common* GetCommon() { return common; };

	/// �v���C���[�Ǘ�----------------------------
	//�ړ���̎擾
	static MOVE_TYPE GetDSystem() { return legtype; }

	//�����̎擾
	static WEPON_TYPE GetWepontype() { return wepon; }

	//�ړ���̐ݒ�
	static void SetDsystem(MOVE_TYPE in) { legtype = in; }

	//�����̐ݒ�
	static void SetWeponType(WEPON_TYPE in) { wepon = in; }
	///-------------------------------------------

	//�ǉ��V�[���̒ǉ�
	template<typename T>
	static T* AddScene()
	{
		T* scene = DBG_NEW T();
		scene->Init();
		addScene.push_back(scene);
		return scene;
	};

	//�w�肵���ǉ��V�[���̎擾
	template <typename T>
	static T* GetAddScene()
	{
		for (Scene* sce : addScene)
		{
			if (typeid(*sce) == typeid(T))//�^�𒲂ׂ�
			{
				return (T*)sce;
			}
		}
		return nullptr;
	}

	//�w�肵���ǉ��V�[���̍폜
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

	//�ǉ��V�[���̈ꊇ�폜
	static void DeleteAddScenes()
	{
		std::list<Scene*>::iterator it = addScene.begin();
		for (; it != addScene.end(); ++it) {
			delete* it;
		}
		addScene.clear();
	}
};
