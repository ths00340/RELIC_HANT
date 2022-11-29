#include "Tools.h"
#include "NumberManager.h"

void NumberManager::Init()
{
	Pos = Float2(10.f, 10.f);
	Size = Float2(20.f, 20.f);
	ZeroDraw = false;
	for (int i = 0; i < NUM_MAX; i++)
	{
		Number* Num = new Number();
		Num->Init();
		num.push_back(Num);
	}
}

void NumberManager::Uninit()
{
	for (GameObject* object : num)
	{
		object->Uninit();
		delete object;
	}
}

void NumberManager::Update()
{
	time++;
	float a = (float)time / (float)maxTime;
	a = 1.f - a;
	a = TOOL::Limit(a);
	Addpos.x = Power * a * (TOOL::RandF() - 0.5f);
	Addpos.y = Power * a * (TOOL::RandF() - 0.5f);
	Score = TOOL::Limit(Score, 99999);
}

void NumberManager::Draw()
{
	// 桁数分処理する
	int number = Score;
	int i = 0;
	int LimitNum = NUM_MAX - (NUM_MAX - DrawNum);
	for (int i = 0; i < LimitNum; i++)
	{
		// 今回表示する桁の数字
		float x = (float)(number % 10);

		// スコアの位置やテクスチャー座標を反映
		float px = Pos.x - (Size.x * i * 0.5f) + Addpos.x;	// プレイヤーの表示位置X
		float py = Pos.y + Addpos.y;			// プレイヤーの表示位置Y
		float pw = Size.x;				// プレイヤーの表示幅
		float ph = Size.y;				// プレイヤーの表示高さ

		float tw = 1.0f / 10;		// テクスチャの幅
		float th = 1.0f / 1;		// テクスチャの高さ
		float tx = x * tw;			// テクスチャの左上X座標
		float ty = 0.0f;			// テクスチャの左上Y座標
		num[i]->SetStatus(px, py, pw, ph, tx, ty, tw, th);
		num[i]->Draw();

		// 次の桁へ
		number /= 10;
		if (number == 0 && ZeroDraw)
			break;
	}
}

void NumberManager::SetScore(int in)
{
	Score = in;
}

void NumberManager::AddScore(int add)
{
	Score += add;
}

int NumberManager::GetScore()
{
	return Score;
}

void NumberManager::SetStatus(Float2 size, Float2 pos, bool cut)
{
	Size = size;
	Pos = pos;
	ZeroDraw = cut;
}

void NumberManager::SetNum(int num)
{
	DrawNum = num;
	DrawNum = TOOL::Limit(DrawNum, NUM_MAX, 0);
}