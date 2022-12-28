#pragma once
#include "main.h"
#include <algorithm>
#include <iostream>
#include <vector>
#include <list>
#include <iterator>
#include <typeinfo>
#include "CComponent.h"

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

	const char* name = "NoName";//�^�O

private:
	static ID3D11BlendState* blendState;
	GameObject* me;//�����w��//���邩�͕������
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

	void SetDestroy() {
		m_Destoroy = true;
	}
	void SetStop(bool stop = true)
	{
		m_Stop = stop;
	}

	//�擾�n
	const bool	 GetDestory() { return m_Destoroy; }
	const bool   GetStop() { return m_Stop; }
	const Float3 Getpos() { return m_pos; };
	const Float3 Getrot() { return m_rot; };
	const Float3 Getscl() { return m_scl; };
	const Float3 Getvec() { return m_vec; };
	const Float3 Getaddrot() { return m_addrot; };

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
		else
		{
			return false;
		}
	}

	std::list<CComponent*> GetComponent()
	{
		return Component;
	}

	template <typename T>
	T* AddComponent()
	{
		T* cmp = new T(this);
		cmp->Init();
		Component.push_back(cmp);
		return cmp;
	}

	template <typename T>
	T* AddFlontComponent()
	{
		T* cmp = new T(this);
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
				//OutputDebugString(TEXT("�ʂ���\n"));
				comps.push_back((T*)comp);
			}
		}
		return comps;
	}

	template<typename T>
	bool RemoveComponent()
	{
		for (CComponent* comp : Component)
		{
			if (typeid(*comp) == typeid(T))//�^�𒲂ׂ�
			{
				delete comp;
				Component.remove(comp);
				return true;
			}
		}
		return false;
	}

	bool RemoveComponents()
	{
		std::list<CComponent*>::iterator it = Component.begin();
		for (; it != Component.end(); ++it) {
			delete* it;
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

/*
	//���Z�n
	virtual void AddLocalRot(Float3 inRot) {
		D3DXQUATERNION quat;
		Float3 axis = GetUp();
		D3DXQuaternionRotationAxis(&quat, &axis, inRot.y);
		m_rot *= quat;

		axis = GetSide();
		D3DXQuaternionRotationAxis(&quat, &axis, inRot.x);
		m_rot *= quat;

		axis = GetForward();
		D3DXQuaternionRotationAxis(&quat, &axis, inRot.z);
		m_rot *= quat;
	}
	virtual void AddLocalRot(float X = 0.f, float Y = 0.f, float Z = 0.f) {
		D3DXQUATERNION quat;
		Float3 axis = GetUp();
		D3DXQuaternionRotationAxis(&quat, &axis, Y);
		m_rot *= quat;

		axis = GetSide();
		D3DXQuaternionRotationAxis(&quat, &axis, X);
		m_rot *= quat;

		axis = GetForward();
		D3DXQuaternionRotationAxis(&quat, &axis, Z);
		m_rot *= quat;
	}

	virtual void AddWorldRot(Float3 inRot) {
		D3DXQUATERNION quat;
		Float3 axis = { 0.f,1.f,0.f };
		D3DXQuaternionRotationAxis(&quat, &axis, inRot.y);
		m_rot *= quat;

		axis = { 1.f,0.f,0.f };
		D3DXQuaternionRotationAxis(&quat, &axis, inRot.x);
		m_rot *= quat;

		axis = { 0.f,0.f,1.f };
		D3DXQuaternionRotationAxis(&quat, &axis, inRot.z);
		m_rot *= quat;
	}
	virtual void AddWorldRot(float X = 0.f, float Y = 0.f, float Z = 0.f) {
		D3DXQUATERNION quat;
		Float3 axis = { 0.f,1.f,0.f };
		D3DXQuaternionRotationAxis(&quat, &axis, Y);
		m_rot *= quat;

		axis = { 1.f,0.f,0.f };
		D3DXQuaternionRotationAxis(&quat, &axis, X);
		m_rot *= quat;

		axis = { 0.f,0.f,1.f };
		D3DXQuaternionRotationAxis(&quat, &axis, Z);
		m_rot *= quat;
	}

	virtual void AddLocalAddRot(Float3 inRot) {
		D3DXQUATERNION quat;
		Float3 axis = GetUp();
		D3DXQuaternionRotationAxis(&quat, &axis, inRot.y);
		m_addrot *= quat;

		axis = GetSide();
		D3DXQuaternionRotationAxis(&quat, &axis, inRot.x);
		m_addrot *= quat;

		axis = GetForward();
		D3DXQuaternionRotationAxis(&quat, &axis, inRot.z);
		m_addrot *= quat;
	}
	virtual void AddLocalAddRot(float X = 0.f, float Y = 0.f, float Z = 0.f) {
		D3DXQUATERNION quat;
		Float3 axis = GetUp();
		D3DXQuaternionRotationAxis(&quat, &axis, Y);
		m_addrot *= quat;

		axis = GetSide();
		D3DXQuaternionRotationAxis(&quat, &axis, X);
		m_addrot *= quat;

		axis = GetForward();
		D3DXQuaternionRotationAxis(&quat, &axis, Z);
		m_addrot *= quat;
	}

	virtual void AddWorldAddRot(Float3 inRot) {
		D3DXQUATERNION quat;
		Float3 axis = { 0.f,1.f,0.f };
		D3DXQuaternionRotationAxis(&quat, &axis, inRot.y);
		m_addrot *= quat;

		axis = { 1.f,0.f,0.f };
		D3DXQuaternionRotationAxis(&quat, &axis, inRot.x);
		m_addrot *= quat;

		axis = { 0.f,0.f,1.f };
		D3DXQuaternionRotationAxis(&quat, &axis, inRot.z);
		m_addrot *= quat;
	}
	virtual void AddWorldAddRot(float X = 0.f, float Y = 0.f, float Z = 0.f) {
		D3DXQUATERNION quat;
		Float3 axis = { 0.f,1.f,0.f };
		D3DXQuaternionRotationAxis(&quat, &axis, Y);
		m_addrot *= quat;

		axis = { 1.f,0.f,0.f };
		D3DXQuaternionRotationAxis(&quat, &axis, X);
		m_addrot *= quat;

		axis = { 0.f,0.f,1.f };
		D3DXQuaternionRotationAxis(&quat, &axis, Z);
		m_addrot *= quat;
	}
*/