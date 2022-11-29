#include "main.h"
#include "input.h"
#include "Tools.h"
#include "GameObject.h"
#include "Camera.h"
#include "NormalMove.h"
#include "Status.h"

void NormalMove::Init()
{
	Jump = true;
}

void NormalMove::Uninit()
{
}

void NormalMove::Update()
{
	Float3 oldPos = object->Getpos();
	Float3 move = Float3(0.0f, 0.0f, 0.0f);

	Status* sta = object->LoadComponent<Status>();

	float max = sta->GetSpdmax();

	if (Input::GetKeyPress('W'))
	{
		move += object->GetForward() * max * 0.35f;
	}
	if (Input::GetKeyPress('S'))
	{
		move -= object->GetForward() * max * 0.35f;
	}
	if (Input::GetKeyPress('A'))
	{
		move -= object->GetSide() * max * 0.35f;
	}
	if (Input::GetKeyPress('D'))
	{
		move += object->GetSide() * max * 0.35f;
	}
	if (Input::GetKeyTrigger(VK_SPACE) && Jump && sta->GetST() >= 20.f)
	{
		object->LoadPos()->y += 0.01f;
		move.y = TOOL::SecDiv(20.0f);
		Jump = false;
		sta->PullST(20.f);
	}
	object->LoadVec()->x *= 0.6f;
	object->LoadVec()->y *= 0.98f;
	object->LoadVec()->z *= 0.6f;
	object->LoadVec()->x += move.x;
	object->LoadVec()->y += move.y;
	object->LoadVec()->z += move.z;

	Float3 rot = object->Getrot();
	if (object->LoadComponent<Camera>())
		rot.y = object->LoadComponent<Camera>()->GetAngle().y;
	object->SetRot(rot);

	if (!Jump && move.y <= 0.f)
	{
		if ((fabsf(object->Getmin().y) * object->Getscl().y) >= object->Getpos().y)
			Jump = true;
	}
}

void NormalMove::Draw() {}