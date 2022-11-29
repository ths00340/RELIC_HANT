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
	Float3 m_pos;//座標
	Float3 m_scl;//サイズ
	Float3 m_rot;//回転
	Float3 m_addrot = Float3(0.0f, 0.0f, 0.0f);//見た目上の追加角度
	Float3 m_addpos = Float3(0.0f, 0.0f, 0.0f);
	Float3 m_vec = Float3(0.0f, 0.0f, 0.0f);//加速度
	Float3 minsize = Float3(0.0f, 0.0f, 0.0f);
	Float3 maxsize = Float3(0.0f, 0.0f, 0.0f);
	bool  m_Destoroy = false;
	std::list<CComponent*> Component;

	bool m_Stop = false;//アップデートの停止

	const char* name = "NoName";
private:
	static ID3D11BlendState* blendState;
	GameObject* me;
public:
	GameObject() {};

	GameObject(GameObject const& obj)
		:m_pos(obj.m_pos), m_scl(obj.m_scl),
		m_rot(obj.m_rot), m_addrot(obj.m_addrot),
		m_addpos(obj.m_addpos), m_vec(obj.m_vec),
		minsize(obj.minsize), maxsize(obj.maxsize), name(obj.name) {
		std::copy(obj.Component.begin(), obj.Component.end(), std::back_inserter(Component));

		for (CComponent* cmp : Component)
			cmp->Setobj(this);

		m_Destoroy = false;
		m_Stop = false;
		me = this;
	};

	virtual void Init() = 0;//純粋仮想関数
	virtual void Uninit() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;
	virtual void FixedUpdate() {
		m_pos += m_vec;
	};
	virtual ID3D11BlendState* GetBlendState() { return blendState; }

	//設定
	void SetMe(GameObject* inme) { me = inme; }
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

	//取得系
	bool GetStop() { return m_Stop; }
	Float3 Getpos() { return m_pos; };
	Float3 Getrot() { return m_rot; };
	Float3 Getscl() { return m_scl; };
	Float3 Getvec() { return m_vec; };
	Float3 Getaddrot() { return m_addrot; };

	Float3 GetForward()
	{
		D3DXMATRIX rot;
		D3DXMatrixRotationYawPitchRoll(&rot, m_rot.y, m_rot.x, m_rot.z);

		Float3 forward;
		forward.x = rot._31;
		forward.y = rot._32;
		forward.z = rot._33;

		return forward;
	}
	Float3 GetUp()
	{
		D3DXMATRIX rot;
		D3DXMatrixRotationYawPitchRoll(&rot, m_rot.y, m_rot.x, m_rot.z);

		Float3 up;
		up.x = rot._21;
		up.y = rot._22;
		up.z = rot._23;

		return up;
	}
	Float3 GetSide()
	{
		D3DXMATRIX rot;
		D3DXMatrixRotationYawPitchRoll(&rot, m_rot.y, m_rot.x, m_rot.z);

		Float3 side;
		side.x = rot._11;
		side.y = rot._12;
		side.z = rot._13;

		return side;
	}

	Float3 Getmin() { return minsize; };
	Float3 Getmax() { return maxsize; };

	//ポインタの受け取り
	Float3* LoadPos() { return &m_pos; };
	Float3* LoadScl() { return &m_scl; };
	Float3* LoadVec() { return &m_vec; };
	Float3* LoadRot() { return &m_rot; };
	Float3* LoadAddRot() { return &m_addrot; };
	Float3* LoadAddPos() { return &m_addpos; };

	//削除
	bool Destroy()
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

		for (CComponent* comp : Component)
		{
			if (typeid(*comp) == typeid(T))//型を調べる
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
			if (typeid(*comp) == typeid(T))//型を調べる
			{
				//OutputDebugString(TEXT("通った\n"));
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
			if (typeid(*comp) == typeid(T))//型を調べる
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

	virtual void Damage(int dmg) {};
	virtual void Finish() {};
	virtual void PlayHit() {};
};
