///System
#include "main.h"
#include "manager.h"
#include "input.h"
#include "renderer.h"

///Scene
#include "Game.h"
#include "Practice.h"
#include "Fade.h"
#include "TextureDrawScene.h"
#include "DebugDrawScene.h"

///GameObject
#include "audio.h"
#include "GameObject.h"
#include "Player.h"
#include "DebugCamMove.h"
#include "Camera.h"
#include "ViewCamera.h"
#include "Field.h"
#include "Wall.h"
#include "Blur_Tex.h"
#include "Skybox.h"
#include "Plane.h"

//Conponent
#include "Status.h"
#include "HitBox.h"

void Game::Init()
{
	Next = false;
	view.Width = (FLOAT)SCREEN_WIDTH;
	view.Height = (FLOAT)SCREEN_HEIGHT;
	view.MinDepth = 0.0f;
	view.MaxDepth = 1.0f;
	view.TopLeftX = 0;
	view.TopLeftY = 0;

	fade = NULL;

	Manager::AddScene<TextureDrawScene>();

	m_pDefaultBlend = ResourceManager::GetBlend(BLEND_S::OBJ_OPAQUE);

	//カメラ管理用オブジェクトの追加 インスタンシングにするべき
	VCam = AddGameObject<ViewCamera>((int)OBJ_LAYER::System);

	//ステージ基礎の追加
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

	Input::ShowPoint(false);
	Input::CangeFixedPointer();
	Pause = false;

	WireFrame = false;

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

	Plane* _plane = AddGameObject<Plane>((int)OBJ_LAYER::NoCaring);
	_plane->SetShader(SHADER_S::NOISE_CLOUD);
	_plane->SetRot(Float3(0.f, 3.14f, 0.f));
	_plane->SetPos(Float3(0.f, 100.f, 0.f));
}

void Game::Uninit()
{
	Renderer::SetTime();

	if (bgm != NULL)
		bgm->StopAll();

	//Input::ShowPoint(true);
	Input::EndFixedPointer();
	Pause = false;

	Renderer::GetDeviceContext()->RSSetState(ResourceManager::GetFrame(FRAME_S::CULL_BACK));

	Scene::Uninit();

	if (B_Data)
		DeleteBattle();
}

void Game::Update()
{
	Scene::Update();
	if (B_Data)
		B_Data->Update();

	if (Input::GetKeyTrigger(DIK_TAB))
	{
		Input::CangeFixedPointer();
		Pause = Pause ? false : true;
		Input::ShowPoint(Pause);
		SetAllStop(Pause);
	}

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
		//デバックカメラに切り替え
		if (Input::GetKeyTrigger(DIK_F1)) {
			if (GetGameObject<DebugCamMove>() == NULL)
			{
				if (Manager::GetScene()->GetGameObject<Player>())
					AddFlontGameObject<DebugCamMove>((int)OBJ_LAYER::GameObject)->Set(Manager::GetScene()->GetGameObject<Player>());
			}
			else
				GetGameObject<DebugCamMove>()->SetDestroy();
		}

		//疑似ヒットボックスの表示
		if (Input::GetKeyTrigger(DIK_F2)) {
			for (GameObject* obj : GetGameObjCmp<HitBox>())
			{
				HitBox* hit = obj->LoadComponent<HitBox>();

				hit->Set(hit->Gettype(), hit->GetDraw() ? false : true);
			}
		}

		//ワイヤフレームの表示
		if (Input::GetKeyTrigger(DIK_F3)) {


			if (!WireFrame)
				Manager::GetAddScene<TextureDrawScene>()->SetShader(SHADER_S::LIGHT_OFF);
			else
				Manager::GetAddScene<TextureDrawScene>()->SetShader(SHADER_S::EDGE);

			WireFrame = WireFrame ? false : true;
		}

		//フレーム停止
		if (Input::GetKeyTrigger(DIK_F4)) {
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
			if (cleartime >= TOOL::FrameMulti(2.f))
			{
				tex->SetDraw(true);
				tex->SetMax(TOOL::FrameMulti(0.5f));
				Next = false;
			}
		}

		if (tex)
			if (tex->GetIn())
				if (Manager::GetCommon() == NULL)
				{
					{
						fade = Manager::SetCommon<Fade>();
						fade->Set(1.f);
					}
				}

		if (fade != NULL)
			if (fade->GetIn())
			{
				NextScene();
			}
	}

	//デバックモード起動
	if (Input::GetKeyPress(DIK_RSHIFT) && Input::GetKeyTrigger(DIK_F12))
	{
		if (debug_com)
			debug_com = false;
		else
			debug_com = true;
	}
}

void Game::Draw()
{
	float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };

	if (WireFrame)
		Renderer::GetDeviceContext()->RSSetState(ResourceManager::GetFrame(FRAME_S::WIRE_FRAME));
	else
		Renderer::GetDeviceContext()->RSSetState(ResourceManager::GetFrame(FRAME_S::CULL_BACK));

	for (int i = 0; i < 2; i++)//システム系
		for (GameObject* object : g_GameObject[i])
		{
			ObjectDraw(i);
		}

	for (int i = 2; i < LAYER_NUM - 1; i++)//視推台カリング
	{
		for (GameObject* object : g_GameObject[i])
		{
			if (VCam != nullptr)
				if (!VCam->CheckView(object->Getpos()))
					continue;

			if (object->GetBlendState() != nullptr)
				Renderer::GetDeviceContext()->OMSetBlendState(object->GetBlendState(), blendFactor, 0xffffffff);
			else
			{
				Renderer::GetDeviceContext()->OMSetBlendState(m_pDefaultBlend, blendFactor, 0xffffffff);
			}

			object->Draw();

			for (CComponent* com : object->GetComponent())
				com->Draw();
		}
	}

	UIDraw();
}

void Game::NoUIDraw()
{
	float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };

	if (WireFrame)
		Renderer::GetDeviceContext()->RSSetState(ResourceManager::GetFrame(FRAME_S::WIRE_FRAME));
	else
		Renderer::GetDeviceContext()->RSSetState(ResourceManager::GetFrame(FRAME_S::CULL_BACK));

	for (int i = 0; i < 2; i++)//システム系
		for (GameObject* object : g_GameObject[i])
		{
			if (object->GetBlendState() != nullptr)
				Renderer::GetDeviceContext()->OMSetBlendState(object->GetBlendState(), blendFactor, 0xffffffff);
			else
			{
				Renderer::GetDeviceContext()->OMSetBlendState(m_pDefaultBlend, blendFactor, 0xffffffff);
			}

			object->Draw();

			for (CComponent* com : object->GetComponent())
				com->Draw();
		}

	for (int i = 2; i < LAYER_NUM - 2; i++)//視推台カリング
	{
		for (GameObject* object : g_GameObject[i])
		{
			if (VCam != nullptr)
				if (!VCam->CheckView(object->Getpos()))
					continue;

			if (object->GetBlendState() != nullptr)
				Renderer::GetDeviceContext()->OMSetBlendState(object->GetBlendState(), blendFactor, 0xffffffff);
			else
			{
				Renderer::GetDeviceContext()->OMSetBlendState(m_pDefaultBlend, blendFactor, 0xffffffff);
			}

			object->Draw();

			for (CComponent* com : object->GetComponent())
				com->Draw();
		}
	}
}

void Game::SetPlayer(Player* player)
{
	if (!pl)
	{
		pl = player;
		g_GameObject[(int)OBJ_LAYER::GameObject].push_back(player);
		GetGameObject<ViewCamera>()->SetView(player->LoadComponent<Camera>());
	}
}

void Game::DebugDraw()
{
	float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };

	for (int i = 0; i < LAYER_NUM; i++)
	{
		for (GameObject* object : g_GameObject[i])//範囲forループ
		{
			if (object->GetBlendState() != NULL)
				Renderer::GetDeviceContext()->OMSetBlendState(object->GetBlendState(), blendFactor, 0xffffffff);

			object->Draw();

			for (CComponent* com : object->GetComponent())
				com->Draw();
		}
	}
}

void Game::NextScene()
{
	Manager::SetScene<MainMenu>();//無ければリザルトに遷移
	fade = NULL;
}

void Game::BGMSelect()
{
#ifndef MUTE
	bgm = AddGameObject<Audio>((int)OBJ_LAYER::System);
	bgm->Load("asset\\BGM\\gameover.wav");
	bgm->Play(true, 0.1f);
#endif//MUTE
}

//ゲームクリアした際の処理
void Game::Clear()
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
void Game::GameOver()
{
	Once = true;//重複阻止
	Next = true;//遷移フラグ
	tex->SetDraw(true);
	tex->SetMax(TOOL::FrameMulti(3.f));
#ifndef MUTE
	OverBGM->Play(false, 0.1f);
#endif // !MUTE
}