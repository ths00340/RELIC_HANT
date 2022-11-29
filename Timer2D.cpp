#include"manager.h"
#include "Scene.h"
#include "Timer2D.h"

void Timer2D::Init()
{
	Frame = Manager::GetScene()->AddGameObject<TimeFrame>((int)OBJ_LAYER::UI);
	min = Manager::GetScene()->AddGameObject<NumberManager>((int)OBJ_LAYER::UI);
	sec = Manager::GetScene()->AddGameObject<NumberManager>((int)OBJ_LAYER::UI);

	min->SetStatus(Float2(100.f, 100.f), Float2(SCREEN_WIDTH - 200.f, 50.f));
	sec->SetStatus(Float2(100.f, 100.f), Float2(SCREEN_WIDTH - 50, 50.f));
	min->SetNum(2);
	sec->SetNum(2);
}

void Timer2D::Uninit()
{
	min->SetDestroy();
	sec->SetDestroy();
}

void Timer2D::Update() {}

void Timer2D::SetTime(int InM, int InS)
{
	min->SetScore(InM);
	sec->SetScore(InS);
}