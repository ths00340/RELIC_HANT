#include "Tools.h"
#include "NumberManager.h"

void NumberManager::Init()
{
	m_Pos = Float2(10.f, 10.f);
	m_Size = Float2(20.f, 20.f);
	m_ZeroDraw = false;
	for (int i = 0; i < NUM_MAX; i++)
	{
		Number* Num = DBG_NEW Number();
		Num->Init();
		m_Num.push_back(Num);
	}
}

void NumberManager::Uninit()
{
	for (GameObject* object : m_Num)
	{
		object->Uninit();
		delete object;
	}
}

void NumberManager::Update()
{
	m_Time++;
	float a = (float)m_Time / (float)m_MaxTime;
	a = 1.f - a;
	a = TOOL::Limit(a);
	m_Addpos.x = m_Power * a * (TOOL::RandF() - 0.5f);
	m_Addpos.y = m_Power * a * (TOOL::RandF() - 0.5f);
	m_Score = TOOL::Limit(m_Score, 99999);
}

void NumberManager::Draw()
{
	// 桁数分処理する
	int number = m_Score;
	int i = 0;
	int LimitNum = NUM_MAX - (NUM_MAX - m_DrawNum);
	for (int i = 0; i < LimitNum; i++)
	{
		// 今回表示する桁の数字
		float x = (float)(number % 10);

		// スコアの位置やテクスチャー座標を反映
		float px = m_Pos.x - (m_Size.x * i * 0.5f) + m_Addpos.x;	// プレイヤーの表示位置X
		float py = m_Pos.y + m_Addpos.y;			// プレイヤーの表示位置Y
		float pw = m_Size.x;				// プレイヤーの表示幅
		float ph = m_Size.y;				// プレイヤーの表示高さ

		float tw = 1.0f / 10;		// テクスチャの幅
		float th = 1.0f / 1;		// テクスチャの高さ
		float tx = x * tw;			// テクスチャの左上X座標
		float ty = 0.0f;			// テクスチャの左上Y座標
		m_Num[i]->SetStatus(px, py, pw, ph, tx, ty, tw, th);
		m_Num[i]->Draw();

		// 次の桁へ
		number /= 10;
		if (number == 0 && m_ZeroDraw)
			break;
	}
}

void NumberManager::SetScore(int in)
{
	m_Score = in;
}

void NumberManager::AddScore(int add)
{
	m_Score += add;
}

int NumberManager::GetScore()
{
	return m_Score;
}

void NumberManager::SetStatus(Float2 size, Float2 pos, bool cut)
{
	m_Size = size;
	m_Pos = pos;
	m_ZeroDraw = cut;
}

void NumberManager::SetNum(int num)
{
	m_DrawNum = num;
	m_DrawNum = TOOL::Limit(m_DrawNum, NUM_MAX, 0);
}