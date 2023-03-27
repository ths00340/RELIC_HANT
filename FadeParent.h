//==============================================================================
// Filename: FadeParent.h
// Description :GameObjectクラスを継承したフェード用オブジェクトの親クラスの定義
//==============================================================================
#pragma once
#include "GameObject.h"
class FadeParent :
	public GameObject
{
protected:
	Float2 pos;
	float test = 0;
	bool In = false;
	bool Out = false;
	int time = 0;
	int maxTime = 10;
public:
	virtual void Init()
	{
		time = 0;
		In = false;
		Out = false;
		test = 0.f;
	};
	virtual void Uninit() = 0;
	virtual void Update()
	{
		time++;
		if (In)
		{
			if (time <= maxTime)
			{
				test = (float)time / (float)maxTime;
			}
			else
			{
				In = false;
			}
		}
		if (Out)
		{
			if (test > 0.0f)
			{
				test = 1.0f - ((float)time / (float)maxTime);
			}
			else
				Out = false;
		}
	};
	virtual void Draw() = 0;
	void SetA(bool in = true, bool out = false, int max = 60) {
		In = in;
		Out = out;
		maxTime = max;
		time = 0;
	}
};
