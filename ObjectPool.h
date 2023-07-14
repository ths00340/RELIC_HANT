#pragma once
#include "GameObject.h"

template <class T>
class ObjectPool :
	public GameObject
{
protected:
	bool Once = false;
	std::list<T*> Pool;
public:
	ObjectPool()
	{
		Once = false;
		Pool.clear();
	}

	~ObjectPool()
	{
		Pool.clear();
	}

	void Init() {};
	void Uninit() {};
	void Update() {};
	void Draw() {};
	T* Recycle();
	virtual void Set(int num) = 0;
};

template<class T>
inline T* ObjectPool<T>::Recycle()
{
	for (T* obj : Pool)
	{
		// T��IEnable�Ƃ����C���^�[�t�F�[�X�N���X���p�����Ă���ꍇ
		if (GameObject* enableable = dynamic_cast<GameObject*>(obj))
		{
			if (!enableable->GetEnable())
			{
				enableable->SetEnable(true);
				return obj;
			}
		}
	}
	return nullptr;
}
