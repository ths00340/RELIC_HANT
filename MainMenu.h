#pragma once
#include "Game.h"
#include "Camera_.h"
#include "MenuTitle.h"
#include "NumberManager.h"
#include "Blur_Tex.h"

enum class MISSION
{
	PRACTICE,//—ûK
	ENEMY_ANNI,//Ÿr–Å
	ENDURA,//‘Ï‹v
	BOSS_1,//’P‘Ì
	BOSS_2,//ƒ{ƒX
	NONE,//I—¹
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
