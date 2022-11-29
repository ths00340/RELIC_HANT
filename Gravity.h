#pragma once
#include "CComponent.h"

class Field;

class Gravity :
	public CComponent
{
private:
	float groundHeight = 0.0f;//íÖínÇ∑ÇÈínñ ÇÃçÇÇ≥
	float gravity = GRAVITY;
	Field* field = nullptr;

public:
	Gravity(GameObject* inobject) :CComponent(inobject) {
		groundHeight = 0.0f;
		gravity = GRAVITY;
	};

	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;
	void Set(float ingra) { gravity = ingra; }
};
