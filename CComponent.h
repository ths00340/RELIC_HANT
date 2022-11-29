#pragma once
class GameObject;

class CComponent
{
protected:
	GameObject* object;
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
	virtual void Draw() = 0;
	void Setobj(GameObject* obj) { object = obj; }
};
