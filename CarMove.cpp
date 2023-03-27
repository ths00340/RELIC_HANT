#include "main.h"
#include "input.h"
#include "Tools.h"
#include "GameObject.h"
#include "CarMove.h"
#include "Status.h"
#include "cInputOperation.h"

void CarMove::Init()
{
	m_pInput = NULL;
	if (m_pInput == NULL)
	{
		m_pInput = object->LoadComponent<cInputOperation>();
	}
}

void CarMove::Uninit()
{
}

void CarMove::Update()
{
	if (m_pInput == NULL)
	{
		m_pInput = object->LoadComponent<cInputOperation>();
		return;
	}

	Float3 move = object->Getvec();
	float y = move.y;
	move.y = 0.0f;
	//ˆÚ“®
	Status* sta = object->LoadComponent<Status>();

	float max = sta->GetSpdmax();
	float acc = sta->GetAcc() * object->Getscl().z;

	move += object->GetForward() * acc * m_pInput->GetMov().y;
	Rot += TOOL::AToR(12 - (5.0f * object->Getscl().z)) * m_pInput->GetMov().x;

	move = TOOL::VectorNormalize(move) * TOOL::Limit(TOOL::VectorLength(move), max, -max * 0.5f);

	Rot = TOOL::Limit(Rot, TOOL::AToR(120.f), -TOOL::AToR(120.f));

	//‰ñ“]‚µ‚Ä‚¢‚È‚¢ê‡‚ÍRot‚ğ‚O‚É
	if (!Input::GetKeyPress(DIK_D) && !Input::GetKeyPress(DIK_A))
		if (fabsf(Rot) <= TOOL::AToR(3))
			Rot = 0;

	move.y += y;

	//Œü‚¢‚Ä‚¢‚é•ûŒü‚ÉƒxƒNƒgƒ‹
	object->SetVec(move);

	//‰ñ“]‚Ìˆ—
	if (abs(TOOL::VectorLength(move)) > 0.03f)
		object->SetRot(Float3(object->Getrot().x, object->Getrot().y + TOOL::SecDiv(Rot), object->Getrot().z));
	else
		Rot = 0.0f;

	//Œ¸‘¬ˆ—
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