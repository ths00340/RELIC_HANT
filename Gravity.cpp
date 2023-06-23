#include "main.h"
#include "input.h"
#include "Tools.h"
#include "Gravity.h"
#include "GameObject.h"
#include "Field.h"
#include "manager.h"

void Gravity::Init()
{
	if (Manager::GetScene()->GetGameObject<Field>() != NULL)
	{
		field = Manager::GetScene()->GetGameObject<Field>();
		return;
	}
}

void Gravity::Uninit()
{
}

void Gravity::Update()
{

	float minhight = 0.f;
	if (field == nullptr)
	{
		if (Manager::GetScene()->GetGameObject<Field>() != NULL)
			field = Manager::GetScene()->GetGameObject<Field>();

		groundHeight = fabsf(object->Getmin().y) * object->Getscl().y;
	}
	else
	{
		minhight = fabsf(object->Getmin().y) * object->Getscl().y;
		groundHeight = field->GetHeight(object->Getpos()) + minhight;
	}

	
	

	if (object->Getpos().y > groundHeight)
	{
		object->LoadVec()->y -= gravity;	//d—Íˆ—
	}
	else
	{
		Float3 a = object->Getpos();
		a.y = groundHeight;
		object->SetPos(a);
		object->LoadVec()->y = 0.0f;
	}
}

void Gravity::Draw()
{
}