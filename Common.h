#pragma once
#include "Scene.h"
#include "Tools.h"
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
	void Set(float time = 1.0f) { DestroyTime = TOOL::FrameMulti(time); }

	bool GetDestroy()
	{
		return Destroy;
	}

	bool GetIn() { return InFade; }
	bool GetOut() { return OutFade; }

	template <typename T>
	bool SetEffect()
	{
		AddGameObject<T>((int)OBJ_LAYER::UI);
	}
	//リフレクション

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
