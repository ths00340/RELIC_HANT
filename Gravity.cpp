#include "main.h"
#include "input.h"
#include "Tools.h"
#include "GameObject.h"
#include "Gravity.h"
#include "Field.h"
#include "manager.h"

void Gravity::Init()
{
	if (Manager::GetScene()->GetGameObject<Field>() != NULL)
		field = Manager::GetScene()->GetGameObject<Field>();
}

void Gravity::Uninit()
{
}

void Gravity::Update()
{
	if (field == nullptr)
	{
		if (Manager::GetScene()->GetGameObject<Field>() != NULL)
			field = Manager::GetScene()->GetGameObject<Field>();

		groundHeight = fabsf(object->Getmin().y) * object->Getscl().y;
	}
	else
		groundHeight = field->GetHeight(object->Getpos()) + fabsf(object->Getmin().y) * object->Getscl().y;

	if (object->Getpos().y > groundHeight)
	{
		Float3 pos = object->Getpos();
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