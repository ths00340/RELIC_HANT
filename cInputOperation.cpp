#include "main.h"
#include "input.h"
#include "manager.h"
#include "cInputOperation.h"

void cInputOperation::Init()
{
	Scene* scene = Manager::GetScene();
	m_PadNum = scene->GetGameObjCmp<cInputOperation>().size();
	if (m_PadNum >= GAMEPAD_MAX)
	{
		TOOL::Display((char*)"ƒpƒbƒh”‚ÌãŒÀ\n");
		object->RemoveComponent<cInputOperation>();
	}
	m_Pov = { 0.f,0.f };
	m_Mov = { 0.f,0.f };
}

void cInputOperation::Uninit()
{
	m_Pov = { 0.f,0.f };
	m_Mov = { 0.f,0.f };
}

void cInputOperation::Update()
{
	m_Pov.x = Input::MouseRelative().x + (Input::GetJoyPadRight(m_PadNum).x);
	m_Pov.y = Input::MouseRelative().y + (Input::GetJoyPadRight(m_PadNum).y);

	m_Pov.x = TOOL::Limit(m_Pov.x, 1.f, -1.f);
	m_Pov.y = TOOL::Limit(m_Pov.y, 1.f, -1.f);

	m_Mov.x = Input::GetJoyPadLeft(m_PadNum).x;
	m_Mov.y = -Input::GetJoyPadLeft(m_PadNum).y;

	if (Input::GetKeyPress(DIK_W))
	{
		m_Mov.y += 1.f;
	}

	if (Input::GetKeyPress(DIK_S))
	{
		m_Mov.y -= 1.f;
	}

	if (Input::GetKeyPress(DIK_A) || Input::IsButtonPressed(m_PadNum, BUTTON_L))
	{
		m_Mov.x -= 1.f;
	}

	if (Input::GetKeyPress(DIK_D) || Input::IsButtonPressed(m_PadNum, BUTTON_R))
	{
		m_Mov.x += 1.f;
	}

	m_Mov.x = TOOL::Limit(m_Mov.x, 1.f, -1.f);
	m_Mov.y = TOOL::Limit(m_Mov.y, 1.f, -1.f);
}

bool cInputOperation::GetShot()
{
	bool ret = false;
	if (Input::IsMouseLeftPressed() || Input::IsButtonPressed(m_PadNum, BUTTON_R2))
		ret = true;

	return ret;
}

bool cInputOperation::GetAds()
{
	bool ret = false;
	if (Input::IsMouseRightTriggered() || Input::IsButtonTriggered(m_PadNum, BUTTON_L2))
		ret = true;

	return ret;
}

bool cInputOperation::GetOption()
{
	bool ret = false;
	if (Input::GetKeyPress(DIK_TAB) || Input::IsButtonPressed(m_PadNum, BUTTON_START))
		ret = true;

	return ret;
}

bool cInputOperation::GetOK()
{
	bool ret = false;
	if (Input::GetKeyPress(DIK_SPACE) || Input::IsButtonPressed(m_PadNum, BUTTON_A))
		ret = true;

	return ret;
}

bool cInputOperation::GetCancel()
{
	bool ret = false;
	if (Input::GetKeyPress(DIK_BACKSPACE) || Input::IsButtonPressed(m_PadNum, BUTTON_B))
		ret = true;

	return ret;
}