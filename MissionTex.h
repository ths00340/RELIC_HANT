//==============================================================================
// Filename: MissionTex.h
// Description: Polygon2Dオブジェクトを継承した指令書表示クラスの定義
//==============================================================================

#pragma once
#include "Polygon2D.h"

class MissionTex :
	public Polygon2D
{
protected:
	bool End = false;
	float time = 0.f;
public:
	MissionTex()
	{
		End = false;
		time = 0.f;
	}

	virtual void Init();
	virtual void Update();
	bool GetEnd() { return End; }
};
