//==============================================================================
// Filename: NumberManager.h
// Description :GameObjectクラスを継承した数字表示管理用クラスの定義
//==============================================================================
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
	int m_Time = 0;//時間の取得
	int m_MaxTime = -1;//最大時間
	float m_Power = 0.f;//揺れの強さ
	std::vector<Number*> m_Num;
	int m_Score = 0;

	Float2 m_Pos = { 0.f,0.f };
	Float2 m_Size = { 0.f,0.f };
	Float2 m_Addpos = { 0.f,0.f };
	bool m_ZeroDraw = false;//ゼロの表示をするか
	int m_DrawNum = 5;
public:
	NumberManager()
	{
		m_Time = 0;//時間の取得
		m_MaxTime = -1;//最大時間
		m_Power = 0.f;//揺れの強さ

		m_Score = 0;

		m_Pos = { 0.f,0.f };
		m_Size = { 0.f,0.f };
		m_Addpos = { 0.f,0.f };
		m_ZeroDraw = false;//ゼロの表示をするか
		m_DrawNum = 5;
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
		m_Time = 0;
		m_MaxTime = max;
		m_Power = P;
	}
	void SetNum(int num);
};
