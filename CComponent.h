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
	virtual void Update() = 0;
	virtual void Draw() {};
	void Setobj(GameObject* obj) { object = obj; }
};
