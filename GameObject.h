//==============================================================================
// Filename: GameObject.h
// Description :オブジェクトを管理するための継承元クラスの定義
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
	Float3 m_pos;//座標
	Float3 m_scl;//サイズ
	Float3 m_rot;//回転
	Float3 m_addrot = { 0.f, 0.f, 0.f };//見た目の追加角度
	Float3 m_addpos = { 0.f, 0.f, 0.f };//見た目の追加位置
	Float3 m_vec = { 0.f, 0.f, 0.f };//加速度

	//実際のサイズのための変数
	Float3 minsize = { 0.0f, 0.0f, 0.0f };//最小座標
	Float3 maxsize = { 0.0f, 0.0f, 0.0f };//最大座標

	bool  m_Destoroy = false;//破壊フラグ

	std::list<CComponent*> Component;//疑似コンポーネント

	bool m_Stop = false;//アップデートの停止///秒数にするべきか

	const char* name = "NoName";//タグ

private:
	ID3D11BlendState* blendState = nullptr;
	GameObject* me;//自分指定//いるかは分からん
public:
	GameObject() {
		me = this;
		m_Destoroy = false;
		m_Stop = false;
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
	const bool	 GetDestory() { return m_Destoroy; }
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

	//ポインタの受け取り
	Float3* LoadPos() { return &m_pos; };
	Float3* LoadScl() { return &m_scl; };
	Float3* LoadVec() { return &m_vec; };
	Float3* LoadRot() { return &m_rot; };
	Float3* LoadAddRot() { return &m_addrot; };
	Float3* LoadAddPos() { return &m_addpos; };

	//削除
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
				Component.remove(comp);
				delete comp;
				return true;
			}
		}
		return false;
	}

	bool RemoveComponents()
	{
		//std::list<CComponent*>::iterator it = Component.begin();
		//for (; it != Component.end(); ++it) {
		//
		//	delete* it;
		//}
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

	virtual void Damage(int dmg) {};//ダメージを負った時の反応
	virtual void Finish() {};//破壊された時の反応
	virtual void PlayHit() {};
};