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
	bool m_Enable = true;


	const char* name = "NoName";//タグ

private:
	ID3D11BlendState* blendState = nullptr;
	GameObject* me;//自分指定//いるかは分からん

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

	virtual void Init() = 0;//純粋仮想関数
	virtual void Uninit() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;
	virtual void InstanceDraw() {};

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

	//取得系
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
			if (typeid(*comp) == typeid(T))//型を調べる
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

	virtual void Damage(int dmg) {};//ダメージを負った時の反応
	virtual void Finish() {};//破壊された時の反応
	virtual void PlayHit() {};
};