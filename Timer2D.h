#pragma once
#include "GameObject.h"
#include "NumberManager.h"
#include "TimeFrame.h"

class Timer2D :
	public GameObject
{
private:
	NumberManager* min;
	NumberManager* sec;
	TimeFrame* Frame;
public:
	void Init();
	void Uninit();
	void Update();
	void Draw() {};
	void SetTime(int InM, int InS);
};
