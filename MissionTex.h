//==============================================================================
// Filename: MissionTex.h
// Description: Polygon2D�I�u�W�F�N�g���p�������w�ߏ��\���N���X�̒�`
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
