#pragma once
#include "main.h"
#include "CComponent.h"
#include "DriveSystem.h"
class CarMove :
	public DriveSystem
{
private:
	float Rot = 0.0f;
	float zlimit = 0.0f;
public:
	CarMove(GameObject* inobject) :DriveSystem(inobject) {
		Rot = 0.0f;
		zlimit = 0.0f;
	};
	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;
};
