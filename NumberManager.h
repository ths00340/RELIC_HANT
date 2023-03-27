//==============================================================================
// Filename: NumberManager.h
// Description :GameObject�N���X���p�����������\���Ǘ��p�N���X�̒�`
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
	int m_Time = 0;//���Ԃ̎擾
	int m_MaxTime = -1;//�ő厞��
	float m_Power = 0.f;//�h��̋���
	std::vector<Number*> m_Num;
	int m_Score = 0;

	Float2 m_Pos = { 0.f,0.f };
	Float2 m_Size = { 0.f,0.f };
	Float2 m_Addpos = { 0.f,0.f };
	bool m_ZeroDraw = false;//�[���̕\�������邩
	int m_DrawNum = 5;
public:
	NumberManager()
	{
		m_Time = 0;//���Ԃ̎擾
		m_MaxTime = -1;//�ő厞��
		m_Power = 0.f;//�h��̋���

		m_Score = 0;

		m_Pos = { 0.f,0.f };
		m_Size = { 0.f,0.f };
		m_Addpos = { 0.f,0.f };
		m_ZeroDraw = false;//�[���̕\�������邩
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
