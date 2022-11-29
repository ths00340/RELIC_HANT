#pragma once
#include "Polygon2D.h"
class Title2D :
	public Polygon2D
{
private:
	float updown = 0.f;
	Float2 defaultpos;
public:
	void Init();
	void Update();
};
