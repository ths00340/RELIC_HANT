//==============================================================================
// Filename: CComponent.h
// Description: オブジェクトに付与されるコンポーネントクラスの定義
//==============================================================================

#pragma once
class GameObject;

class CComponent
{
protected:
	GameObject* object;//付与されたオブジェクト
	bool m_Destroy = false;

public:
	CComponent(GameObject* gameobj)
	{
		object = gameobj;
	}
	~CComponent()
	{
		object = nullptr;
	}
	virtual void Init() = 0;
	virtual void Uninit() = 0;
	virtual void Update() {};
	virtual void Draw() {};
	virtual void OnEnable() {};
	virtual void OnDisable() {};

	void SetDestroy() { m_Destroy = true; }
	const bool GetDestroy() { return m_Destroy; }
	const bool Destroy()
	{
		if (m_Destroy)
		{
			Uninit();
			delete this;
			return true;
		}
		return false;
	}

	void Setobj(GameObject* obj) { object = obj; }
	GameObject* GetParent() { return object; }
};
