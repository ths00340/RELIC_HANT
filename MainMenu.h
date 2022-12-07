#pragma once
#include "Game.h"

enum class MISSION
{
	PRACTICE,//���K
	ENEMY_ANNI,//�r��
	ENDURA,//�ϋv
	BOSS_1,//�P��
	BOSS_2,//�{�X
	NONE,//�I��
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
