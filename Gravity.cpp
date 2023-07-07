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
	if (field)
		field = nullptr;
}

void Gravity::Update()
{
	isGround = false;
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
		groundHeight = field->GetHeight(object->Getpos(), &m_Normal) + minhight;
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
		//object->SetUp(m_Normal);
		//TOOL::Display("Œ»Ý‚ÌUP [X:%f][Y:%f][Z:%f]\n", object->GetUp().x, object->GetUp().y, object->GetUp().z);
		isGround = true;
	}
}

void Gravity::Draw()
{
}