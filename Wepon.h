#pragma once
#include "main.h"
#include "CComponent.h"

class Status;
class Audio;

class Wepon :
	public CComponent
{
protected:
	Float3 m_pos;
	Float3 m_rot;
	Float3 m_scl;
	float angle = 0.0f;
	int dmg = 1;
	int time = 0;
	Float3 ShotAngle;
	Float3 shotpos;

	Status* objS = NULL;
	Audio* shot = NULL;
	float range;
	int shotnum;
	int shotMax;
	float ReCast;
public:
	Wepon(GameObject* inobject) :CComponent(inobject) {
		m_pos = { 0.0f,0.0f,0.0f };
		m_rot = { 0.0f,0.0f,0.0f };
		m_scl = { 1.0f,1.0f,1.0f };
		angle = 0.f;
		dmg = 1;
		time = 0;
		ShotAngle = { 0.0f,0.0f,0.0f };
		shotpos = { 0.0f,0.0f,0.0f };
		range = 60.f;
		shotnum = 0;
		shotMax = 20;
		ReCast = 3.f;

		objS = NULL;
		shot = NULL;
	};
};
