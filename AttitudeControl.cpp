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

	Float3 Look = TOOL::LookatX(PPos, FixedPos);

	if (!TOOL::CanHit(FixedPos, PPos, 0.05f))
		object->LoadAddRot()->x = TOOL::LookatX(PPos, FixedPos).x;

	FixedPos = PPos;
}