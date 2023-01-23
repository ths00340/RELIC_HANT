#include "main.h"
#include "input.h"
#include "Tools.h"
#include "GameObject.h"
#include "CarMove.h"
#include "Status.h"

void CarMove::Init()
{
}

void CarMove::Uninit()
{
}

void CarMove::Update()
{
	Float3 move = object->Getvec();
	float y = move.y;
	move.y = 0.0f;
	//移動
	Status* sta = object->LoadComponent<Status>();

	float max = sta->GetSpdmax();
	float acc = sta->GetAcc() * object->Getscl().z;

	if (Input::GetKeyPress('W'))
	{
		move += object->GetForward() * acc;
		move = TOOL::VectorNormalize(move) * TOOL::Limit(TOOL::VectorLength(move), max, -max * 0.5f);
	}
	if (Input::GetKeyPress('S'))
	{
		move += object->GetForward() * -acc * 0.5f;
		move = TOOL::VectorNormalize(move) * TOOL::Limit(TOOL::VectorLength(move), max, -max * 0.5f);
	}
	if (Input::GetKeyPress('A'))
	{
		Rot += -TOOL::AToR(12 - (5.0f * object->Getscl().z));
	}
	if (Input::GetKeyPress('D'))
	{
		Rot += TOOL::AToR(12 - (5.0f * object->Getscl().z));
	}

	//if (Input::GetKeyTrigger(VK_SPACE))
	//	object->LoadPos()->y = 100.f;

	Rot = TOOL::Limit(Rot, TOOL::AToR(120.f), -TOOL::AToR(120.f));

	//回転していない場合はRotを０に
	if (!Input::GetKeyPress('D') && !Input::GetKeyPress('A'))
		if (fabsf(Rot) <= TOOL::AToR(3))
			Rot = 0;

	move.y += y;

	//float limit=

	//向いている方向にベクトル
	object->SetVec(move);

	//回転の処理
	if (abs(TOOL::VectorLength(move)) > 0.03f)
		object->SetRot(Float3(object->Getrot().x, object->Getrot().y + TOOL::SecDiv(Rot), object->Getrot().z));
	else
		Rot = 0.0f;

	//減速処理
	if (fabsf(TOOL::VectorLength(move)) < 0.002f)
	{
		object->SetVec(Float3(0.0f, 0.0f, 0.0f));
	}
	else
	{
		object->LoadAddRot()->z = (Rot / TOOL::AToR(120.0f)) * (TOOL::AToR(10) * (TOOL::VectorLength(move) / max));
		object->LoadVec()->x *= 0.93f;
		object->LoadVec()->z *= 0.93f;
	}

	Rot *= 0.9f;
}

void CarMove::Draw()
{
}