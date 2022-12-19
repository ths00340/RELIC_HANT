#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "audio.h"
#include "Polygon2D.h"
#include "Field.h"
#include "Camera.h"
#include "input.h"
#include "model.h"
#include "Player.h"
#include "Enemy.h"
#include "bullet.h"
#include "GameObject.h"
#include "Skybox.h"
#include "ViewCamera.h"
#include "Game_S.h"
#include "DebugCamMove.h"
#include "CarMove.h"
#include "Fade.h"
#include "Leg_01.h"
#include "Gravity.h"
#include "SphereShadow.h"
#include "HitBox.h"
#include "Practice.h"
#include "Wall.h"
#include "Blur_Tex.h"
#include "DebugDrawScene.h"

class Result_S;

void Game_S::Init()
{
	Stop = false;
	Once = false;
	cleartime = 0;
	view.Width = (FLOAT)SCREEN_WIDTH;
	view.Height = (FLOAT)SCREEN_HEIGHT;
	view.MinDepth = 0.0f;
	view.MaxDepth = 1.0f;
	view.TopLeftX = 0;
	view.TopLeftY = 0;

	fade = NULL;

	//Manager::AddScene<DebugDrawScene>();

	////カメラ→3D→2Dの流れ
	VCam=AddGameObject<ViewCamera>((int)OBJ_LAYER::System);
	AddGameObject<Skybox>((int)OBJ_LAYER::NoCaring);
	AddGameObject<Field>((int)OBJ_LAYER::NoCaring);

	//プレイヤーの設定読み込み
	pl = AddGameObject<Player>((int)OBJ_LAYER::GameObject);
	pl->SetHP(pl->LoadComponent<Status>()->GetHPM());//HP表示用UI
	pl->SetMP();//MP表示用UI

	//ミッションデータ読み込み
	SetBattle<Practice>();

	tex = AddGameObject<Blur_Tex>((int)OBJ_LAYER::UI);
#ifndef MUTE
	BGMSelect();

	ClearBGM = AddGameObject<Audio>((int)OBJ_LAYER::System);
	ClearBGM->Load("asset\\SE\\Clear.wav");

	OverBGM = AddGameObject<Audio>((int)OBJ_LAYER::System);
	OverBGM->Load("asset\\SE\\Over.wav");
#endif // !MUTE
	debug_com = false;

	Input::SetPause(false);
	Input::ShowPoint(false);

	GameObject* out = AddGameObject<Wall>((int)OBJ_LAYER::NoCaring);
	out->SetPos({ 0.0f,50.0f,200.0f });

	out = AddGameObject<Wall>((int)OBJ_LAYER::NoCaring);
	out->SetRot({ 0.0f,TOOL::AToR(90),0.0f });
	out->SetPos({ 200.0f,50.0f,0.0f });

	out = AddGameObject<Wall>((int)OBJ_LAYER::NoCaring);
	out->SetRot({ 0.0f,TOOL::AToR(180),0.0f });
	out->SetPos({ 0.0f,50.0f,-200.0f });

	out = AddGameObject<Wall>((int)OBJ_LAYER::NoCaring);
	out->SetRot({ 0.0f,-TOOL::AToR(90),0.0f });
	out->SetPos({ -200.0f,50.0f,0.0f });
}

void Game_S::Uninit()
{
	if (bgm != NULL)
		bgm->StopAll();

	if (!Input::GetPause())
	{
		Input::SetPause(true);
		Input::ShowPoint(true);
	}

	Renderer::GetDeviceContext()->RSSetState(ResourceManager::GetFrame(FRAME_S::CULL_BACK));

	if (B_Data)
		DeleteBattle();

	Game::Uninit();
}

void Game_S::Update()
{
	Game::Update();

	//範囲外の設定
	{
		float maxx, maxz;
		maxx = 200.0f;
		maxz = 200.0f;

		for (GameObject* obj : GetGameObjCmp<Status>())
		{
			Float3 pos = obj->Getpos();
			if (pos.x > maxx)
			{
				obj->LoadPos()->x = maxx;
			}
			if (pos.x < -maxx)
			{
				obj->LoadPos()->x = -maxx;
			}
			if (pos.z > maxz)
			{
				obj->LoadPos()->z = maxz;
			}
			if (pos.z < -maxz)
			{
				obj->LoadPos()->z = -maxz;
			}
		}
	}

	//デバック用
	if (debug_com)
	{
		if (Input::GetKeyTrigger(VK_F1)) {
			if (GetGameObject<DebugCamMove>() == NULL)
			{
				if (Manager::GetScene()->GetGameObject<Player>())
					AddFlontGameObject<DebugCamMove>((int)OBJ_LAYER::GameObject)->Set(Manager::GetScene()->GetGameObject<Player>());
			}
			else
				GetGameObject<DebugCamMove>()->SetDestroy();
		}

		if (Input::GetKeyTrigger(VK_F2)) {
			for (GameObject* obj : GetGameObjCmp<HitBox>())
			{
				HitBox* hit = obj->LoadComponent<HitBox>();
				if (hit->GetDraw())
					hit->Set(hit->Gettype());
				else
					hit->Set(hit->Gettype(), true);
			}
		}

		if (Input::GetKeyTrigger(VK_F3)) {
			if (!WireFrame)
				WireFrame = true;
			else
				WireFrame = false;
		}
		if (Input::GetKeyTrigger(VK_F4)) {
			if (!Stop)
				Stop = true;
			else
			{
				if (Stop)
					SetAllStop(false);

				Stop = false;
			}
		}

		if (Stop)
		{
			if (!GetGameObject<DebugCamMove>())
				if (Input::IsMouseRightTriggered())
					SetAllStop(false);
				else
				{
					SetAllStop();
				}
		}
	}

	//シーン遷移用
	{
		if (!Once)
		{
			if (B_Data)//ミッションデータが存在しているか
			{
				if (B_Data->GetClear())//クリアしているか
				{
					Clear();
				}
				if (B_Data->GetOver())//ゲームオーバーになっているか
				{
					GameOver();
				}
			}
		}

		if (Next)//次のシーンに遷移する際の処理
		{
#ifndef MUTE
			bgm->Stop();
#endif//MUTE
			cleartime++;
			if (cleartime >= TOOL::FrameMulti(5.f))
			{
				tex->SetDraw(true);
				tex->SetMax(TOOL::FrameMulti(3.f));
				Next = false;
			}
		}

		if (tex->GetIn())
			if (Manager::GetCommon() == NULL)
			{
				//if (Input::GetKeyTrigger(VK_SPACE))
				{
					fade = Manager::SetCommon<Fade>();
					fade->Set(0.5f);
				}
			}

		if (fade != NULL)
			if (fade->GetIn())
			{
				NextScene();
			}
	}

	//デバックモード起動
	if (Input::GetKeyPress(VK_RSHIFT) && Input::GetKeyTrigger(VK_F12))
	{
		if (debug_com)
			debug_com = false;
		else
			debug_com = true;
	}

	//マウスカーソルの表示
	if (Input::GetKeyTrigger(VK_TAB))
	{
		if (Input::GetPause())
		{
			Input::SetPause(false);
			Input::ShowPoint(false);
		}
		else
		{
			Input::SetPause(true);
			Input::ShowPoint(true);
		}
	}
}

void Game_S::Draw()
{
	if (WireFrame)
		Renderer::GetDeviceContext()->RSSetState(ResourceManager::GetFrame(FRAME_S::WIRE_FRAME));
	else
		Renderer::GetDeviceContext()->RSSetState(ResourceManager::GetFrame(FRAME_S::CULL_BACK));

	Game::Draw();
}

void Game_S::NextScene()
{
	if (B_Data)//ミッションデータが存在しているか
	{
		if (B_Data->GetClear())
			Manager::SetScene<MainMenu>();//ゲームクリアした際の遷移先
		else
			Manager::SetScene<MainMenu>();//ゲームオーバーした際の遷移先
	}
	else
		Manager::SetScene<Result_S>();//無ければリザルトに遷移
	fade = NULL;
}

void Game_S::BGMSelect()
{
#ifndef MUTE
	bgm = AddGameObject<Audio>((int)OBJ_LAYER::System);
	bgm->Load("asset\\BGM\\gameover.wav");
	bgm->Play(true, 0.1f);
#endif//MUTE
}

//ゲームクリアした際の処理
void Game_S::Clear()
{
	Next = true;
	Once = true;
	cleartime = 0;
#ifndef MUTE
	ClearBGM->Play(false, 0.1f);
#endif // !MUTE
	if (GetLiveObj(pl))
		pl->LoadComponent<Status>()->SetBreak();
}

//ゲームオーバーした際の処理
void Game_S::GameOver()
{
	Once = true;//重複阻止
	Next = true;//遷移フラグ
	tex->SetDraw(true);
	tex->SetMax(TOOL::FrameMulti(3.f));
#ifndef MUTE
	OverBGM->Play(false, 0.1f);
#endif // !MUTE
}