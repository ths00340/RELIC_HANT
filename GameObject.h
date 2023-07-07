//==============================================================================
// Filename: GameObject.h
// Description :�I�u�W�F�N�g���Ǘ����邽�߂̌p�����N���X�̒�`
//==============================================================================
#pragma once
#include "main.h"
#include <algorithm>
#include <iostream>
#include <vector>
#include <list>
#include <iterator>
#include <typeinfo>
#include "CComponent.h"
#include "Bazooka.h"

class GameObject
{
protected:
	Float3 m_pos;//���W
	Float3 m_scl;//�T�C�Y
	Float3 m_rot;//��]
	Float3 m_addrot = { 0.f, 0.f, 0.f };//�����ڂ̒ǉ��p�x
	Float3 m_addpos = { 0.f, 0.f, 0.f };//�����ڂ̒ǉ��ʒu
	Float3 m_vec = { 0.f, 0.f, 0.f };//�����x

	//���ۂ̃T�C�Y�̂��߂̕ϐ�
	Float3 minsize = { 0.0f, 0.0f, 0.0f };//�ŏ����W
	Float3 maxsize = { 0.0f, 0.0f, 0.0f };//�ő���W

	bool  m_Destoroy = false;//�j��t���O

	std::list<CComponent*> Component;//�^���R���|�[�l���g

	bool m_Stop = false;//�A�b�v�f�[�g�̒�~///�b���ɂ���ׂ���
	bool m_Enable = true;


	const char* name = "NoName";//�^�O

private:
	ID3D11BlendState* blendState = nullptr;
	GameObject* me;//�����w��//���邩�͕������

	Float3 MatrixToEuler(const D3DXMATRIX& matrix)
	{
		Float3 ret;
		// Extract the Euler angles
		ret.y = atan2(matrix._13, matrix._11);
		ret.x = asin(-matrix._12);
		ret.z = atan2(matrix._32, matrix._22);

		return ret;
	}
public:
	GameObject() {
		me = this;
		m_Destoroy = false;
		m_Stop = false;
	};

	virtual void Init() = 0;//�������z�֐�
	virtual void Uninit() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;
	virtual void InstanceDraw() {};

	virtual void FixedUpdate() {
		m_pos += m_vec;
	};
	virtual ID3D11BlendState* GetBlendState() { return blendState; }

	//�ݒ�
	virtual void SetPos(Float3 inpos) { m_pos = inpos; };
	virtual void SetRot(Float3 inrot) { m_rot = inrot; };
	virtual void SetScl(Float3 inscl) { m_scl = inscl; };
	virtual void SetVec(Float3 invec) { m_vec = invec; };
	virtual void SetAddRot(Float3 inaddrot) { m_addrot = inaddrot; };
	virtual void SetAddPos(Float3 inaddpos) { m_addpos = inaddpos; };

	void SetForward(Float3 inForward)
	{
		D3DXMATRIX rot;
		D3DXMatrixRotationYawPitchRoll(&rot, m_rot.y, m_rot.x, m_rot.z);

		rot._31 = inForward.x;
		rot._32 = inForward.y;
		rot._33 = inForward.z;

		m_rot = MatrixToEuler(rot);
	}
	void SetUp(Float3 inUp)
	{
		D3DXMATRIX rot;
		D3DXMatrixRotationYawPitchRoll(&rot, m_rot.y, m_rot.x, m_rot.z);

		rot._21 = inUp.x;
		rot._22 = inUp.y;
		rot._23 = inUp.z;

		m_addrot = MatrixToEuler(rot);
	}
	void SetSide(Float3 inSide)
	{
		D3DXMATRIX rot;
		D3DXMatrixRotationYawPitchRoll(&rot, m_rot.y, m_rot.x, m_rot.z);

		rot._11 = inSide.x;
		rot._12 = inSide.y;
		rot._13 = inSide.z;

		m_rot = MatrixToEuler(rot);
	}

	void SetDestroy() {
		m_Destoroy = true;
	}
	void SetEnable(bool inEnable) { m_Enable = inEnable; };
	void SetStop(bool stop = true)
	{
		m_Stop = stop;
	}

	//�擾�n
	const bool	 GetDestory() { return m_Destoroy; }
	const bool	 GetEnable() { return m_Enable; }
	const bool   GetStop() { return m_Stop; }
	const Float3 Getpos() { return m_pos; };
	const Float3 Getrot() { return m_rot; };
	const Float3 Getscl() { return m_scl; };
	const Float3 Getvec() { return m_vec; };
	const Float3 Getaddrot() { return m_addrot; };
	const Float3 GetAddPos() { return m_addpos; };

	const Float3 GetForward()
	{
		D3DXMATRIX rot;
		D3DXMatrixRotationYawPitchRoll(&rot, m_rot.y, m_rot.x, m_rot.z);

		Float3 forward;
		forward.x = rot._31;
		forward.y = rot._32;
		forward.z = rot._33;

		return forward;
	}
	const Float3 GetUp()
	{
		D3DXMATRIX rot;
		D3DXMatrixRotationYawPitchRoll(&rot, m_rot.y, m_rot.x, m_rot.z);

		Float3 Up;
		Up.x = rot._21;
		Up.y = rot._22;
		Up.z = rot._23;

		return Up;
	}
	const Float3 GetSide()
	{
		D3DXMATRIX rot;
		D3DXMatrixRotationYawPitchRoll(&rot, m_rot.y, m_rot.x, m_rot.z);

		Float3 side;
		side.x = rot._11;
		side.y = rot._12;
		side.z = rot._13;

		return side;
	}

	const Float3 Getmin() { return minsize; };
	const Float3 Getmax() { return maxsize; };

	//�|�C���^�̎󂯎��
	Float3* LoadPos() { return &m_pos; };
	Float3* LoadScl() { return &m_scl; };
	Float3* LoadVec() { return &m_vec; };
	Float3* LoadRot() { return &m_rot; };
	Float3* LoadAddRot() { return &m_addrot; };
	Float3* LoadAddPos() { return &m_addpos; };

	//�폜
	const bool Destroy()
	{
		if (m_Destoroy)
		{
			RemoveComponents();
			Uninit();
			delete this;
			return true;
		}
		return false;
	}

	std::list<CComponent*> GetComponent()
	{
		return Component;
	}

	template <typename T>
	T* AddComponent()
	{
		T* cmp = DBG_NEW T(this);
		cmp->Init();
		Component.push_back(cmp);
		return cmp;
	}

	template <typename T>
	T* AddFlontComponent()
	{
		T* cmp = DBG_NEW T(this);
		cmp->Init();
		Component.push_front(cmp);
		return cmp;
	}

	template <typename T>
	T* LoadComponent()
	{
		GameObject* a = this;
		int aaa = 0;
		for (CComponent* comp : Component)
		{
			aaa++;
			if (typeid(*comp) == typeid(T))//�^�𒲂ׂ�
			{
				return (T*)comp;
			}
		}
		return NULL;
	}

	template <typename T>
	std::vector<T*> GetComponents()
	{
		std::vector<T*>comps;

		for (GameObject* comp : Component)
		{
			if (typeid(*comp) == typeid(T))//�^�𒲂ׂ�
			{
				comps.push_back((T*)comp);
			}
		}
		return comps;
	}

	template<typename T>
	const bool RemoveComponent()
	{
		for (CComponent* comp : Component)
		{
			if (typeid(*comp) == typeid(T))//�^�𒲂ׂ�
			{
				comp->Uninit();
				Component.remove(comp);
				delete comp;
				return true;
			}
		}
		return false;
	}

	const bool RemoveComponents()
	{
		for (CComponent* it : Component)
		{
			it->Uninit();
			delete it;
		}

		Component.clear();
		return true;
	}

	const char* GetName()
	{
		return name;
	}

	virtual void Damage(int dmg) {};//�_���[�W�𕉂������̔���
	virtual void Finish() {};//�j�󂳂ꂽ���̔���
	virtual void PlayHit() {};
};