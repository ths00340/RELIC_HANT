#pragma once
#include "Scene.h"
#include "FadeParent.h"
class Common :public Scene
{
protected:
	bool Destroy = false;//Trueにするとこのシーンを破棄
	bool InFade = false;//終わったタイミングでTrue
	bool OutFade = false;//終わったタイミングでTrue
	int time = 0;
	int DestroyTime = 60;
	FadeParent* fade;
public:
	virtual void Init() = 0;
	virtual void Uninit() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;
	void Set(float time = 1.0f);

	const bool GetDestroy()
	{
		return Destroy;
	}

	const bool GetIn() { return InFade; }
	const bool GetOut() { return OutFade; }

	template <typename T>
	const bool SetEffect()
	{
		AddGameObject<T>((int)OBJ_LAYER::UI);
	}
	//リフレクション…難しい

	template <typename T>//テンプレート関数
	FadeParent* SetFadeObject()
	{
		if (fade != NULL)
			return fade;

		T* gameObject = new T();
		FadeParent* obj = gameObject;
		gameObject->Init();
		g_GameObject[(int)OBJ_LAYER::UI].push_back(gameObject);
		fade = obj;
		return gameObject;
	}
};
