#include "manager.h"
#include "Fade.h"
#include "ShutterFade.h"

void Fade::Load()
{
}

void Fade::UnLoad()
{
}

void Fade::Init()
{
	OutputDebugString(TEXT("フェード開始\n"));
	time = 0;
	InFade = false;
	OutFade = false;
}

void Fade::Uninit()
{
	Scene::Uninit();
}

void Fade::Update()
{
	if (fade == NULL)
		fade = AddGameObject<ShutterFade>((int)OBJ_LAYER::UI);

	Scene::Update();

	if (time == 0)//初期動作
	{
		OutputDebugString(TEXT("フェードイン開始\n"));
		fade->SetA(true, false, DestroyTime / 2);
	}
	time++;

	if (time < DestroyTime / 2.0f)//フェードイン中の動作
	{
	}
	else if (time == DestroyTime / 2.0f)//フェードイン終了用動作
	{
		OutputDebugString(TEXT("フェードイン終了\n"));
		InFade = true;
		fade->SetA(false, true, DestroyTime / 2);
		OutputDebugString(TEXT("フェードアウト開始\n"));
	}
	else if (time <= DestroyTime)//フェードアウト中の動作
	{
	}
	else//フェードアウト終了用動作
	{
		OutputDebugString(TEXT("フェードアウト終了\n"));
		OutFade = true;
		Destroy = true;
	}
}

void Fade::Draw()
{
	Scene::Draw();
}