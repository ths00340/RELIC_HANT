#include "Title.h"
#include "Camera.h"
#include "Polygon2D.h"
#include "audio.h"
#include "Tools.h"
#include "Common.h"
#include "Fade.h"
#include "CreateLogo.h"
#include "Logo_S.h"
#include "NormalFade.h"
#include "ShutterFade.h"

#include <thread>

void Logo_S::Init()
{
	time = 0;
	view.Width = (FLOAT)SCREEN_WIDTH;
	view.Height = (FLOAT)SCREEN_HEIGHT;
	view.MinDepth = 0.0f;
	view.MaxDepth = 1.0f;
	view.TopLeftX = 0;
	view.TopLeftY = 0;
	fade = NULL;

	AddGameObject<CreateLogo>((int)OBJ_LAYER::UI);
}

void Logo_S::Uninit()
{
}

void Logo_S::Update()
{
	Scene::Update();

	if (fade == NULL)
		time++;

	if (time > TOOL::FrameMulti(1.0f))
	{
		time = 0;
		if (Manager::GetCommon() == NULL)
		{
			fade = Manager::SetCommon<Fade>();
			fade->SetFadeObject<NormalFade>();
			fade->Set(2.0f);
		}
	}

	if (fade != NULL)
		if (fade->GetIn())
		{
			Manager::SetScene<Title>();
			fade = NULL;
		}
}