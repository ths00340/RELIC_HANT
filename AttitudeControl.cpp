#include "AttitudeControl.h"
#include "GameObject.h"
#include "Tools.h"

void AttitudeControl::Init()
{
	FixedPos = object->Getpos();
	time = 0;
}

void AttitudeControl::Update()
{
	time++;
	Float3 PPos = object->Getpos();

	float Look = TOOL::LookatX(PPos, FixedPos).x;

	if (!TOOL::CanHit(FixedPos, PPos, 0.05f))
	{
		float _nowRot = object->Getaddrot().x;
		float zeroFloat = 0.f;
		object->LoadAddRot()->x = TOOL::SmoothDamp(_nowRot, Look, zeroFloat, 0.025f, 1000.f, 0.016f);
	}

	FixedPos = PPos;
}