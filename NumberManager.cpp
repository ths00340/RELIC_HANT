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
	// ��������������
	int number = Score;
	int i = 0;
	int LimitNum = NUM_MAX - (NUM_MAX - DrawNum);
	for (int i = 0; i < LimitNum; i++)
	{
		// ����\�����錅�̐���
		float x = (float)(number % 10);

		// �X�R�A�̈ʒu��e�N�X�`���[���W�𔽉f
		float px = Pos.x - (Size.x * i * 0.5f) + Addpos.x;	// �v���C���[�̕\���ʒuX
		float py = Pos.y + Addpos.y;			// �v���C���[�̕\���ʒuY
		float pw = Size.x;				// �v���C���[�̕\����
		float ph = Size.y;				// �v���C���[�̕\������

		float tw = 1.0f / 10;		// �e�N�X�`���̕�
		float th = 1.0f / 1;		// �e�N�X�`���̍���
		float tx = x * tw;			// �e�N�X�`���̍���X���W
		float ty = 0.0f;			// �e�N�X�`���̍���Y���W
		num[i]->SetStatus(px, py, pw, ph, tx, ty, tw, th);
		num[i]->Draw();

		// ���̌���
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