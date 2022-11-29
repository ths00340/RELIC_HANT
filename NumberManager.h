#pragma once

#include <list>
#include <vector>
#include <typeinfo>
#include "GameObject.h"
#include "Number.h"

static int NUM_MAX(5);

class NumberManager :
	public GameObject
{
private:
	int time;//ŠÔ‚Ìæ“¾
	int maxTime;//Å‘åŠÔ
	float Power;//—h‚ê‚Ì‹­‚³
	std::vector<Number*> num;
	int Score;

	Float2 Pos;
	Float2 Size;
	Float2 Addpos;
	bool ZeroDraw = false;//ƒ[ƒ‚Ì•\¦‚ğ‚·‚é‚©
	int DrawNum = 5;
public:
	void Init();
	void Uninit();
	void Update();
	void Draw();
	void SetScore(int in);
	void AddScore(int add);
	int GetScore();
	void SetStatus(Float2 size = Float2(10.f, 10.f), Float2 pos = Float2(0.f, 0.f), bool cut = false);
	void Shake(int max = 60, float P = 1.0f)
	{
		time = 0;
		maxTime = max;
		Power = P;
	}
	void SetNum(int num);
};
