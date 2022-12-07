#pragma once
#include "Game.h"

enum class MISSION
{
	PRACTICE,//練習
	ENEMY_ANNI,//殲滅
	ENDURA,//耐久
	BOSS_1,//単体
	BOSS_2,//ボス
	NONE,//終了
};

class MainMenu :
	public Game
{
private:
	class Camera_* menuCam;
	class MenuTitle* title;
	class Blur_Tex* blur;
	class NumberManager* score;
	MISSION MissionNum = MISSION::PRACTICE;
public:
	void Init();
	void Uninit();
	void Update();
};
