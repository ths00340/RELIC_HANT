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
	int time = 0;//ŠÔ‚Ìæ“¾
	int maxTime = -1;//Å‘åŠÔ
	float Power = 0.f;//—h‚ê‚Ì‹­‚³
	std::vector<Number*> num;
	int Score = 0;

	Float2 Pos = { 0.f,0.f };
	Float2 Size = { 0.f,0.f };
	Float2 Addpos = { 0.f,0.f };
	bool ZeroDraw = false;//ƒ[ƒ‚Ì•\¦‚ğ‚·‚é‚©
	int DrawNum = 5;
public:
	NumberManager()
	{
		time = 0;//ŠÔ‚Ìæ“¾
		maxTime = -1;//Å‘åŠÔ
		Power = 0.f;//—h‚ê‚Ì‹­‚³

		Score = 0;

		Pos = { 0.f,0.f };
		Size = { 0.f,0.f };
		Addpos = { 0.f,0.f };
		ZeroDraw = false;//ƒ[ƒ‚Ì•\¦‚ğ‚·‚é‚©
		DrawNum = 5;
	}

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
