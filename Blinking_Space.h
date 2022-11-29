#pragma once
#include "Polygon2D.h"
class Blinking_Space :
	public Polygon2D
{
private:
	float a = 0.0f;
public:
	void Init();
	void Update();
};
