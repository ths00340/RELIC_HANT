#pragma once
#include "Polygon2D.h"
class MissionTex :
	public Polygon2D
{
protected:
	bool End;
	float time;
public:
	virtual void Init();
	virtual void Update();
	bool GetEnd() { return End; }
};
