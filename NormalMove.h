#pragma once
#include "main.h"
#include "CComponent.h"
#include "DriveSystem.h"
class NormalMove :
	public DriveSystem
{
private:
	bool Jump = false;
public:
	NormalMove(GameObject* inobject) :DriveSystem(inobject) {
		Jump = false;
	};
	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;
};
