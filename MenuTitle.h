//==============================================================================
// Filename: MenuTitle.h
// Description :Polygon2D�N���X���p���������j���[�^�C�g���̕\���p�N���X�̒�`
//==============================================================================
#pragma once
#include "Polygon2D.h"

class MenuTitle :
	public Polygon2D
{
	float type = 0.0f;
	int time = 0;
	int maxtime = 0;
	float defaultpos = -100.0f;
public:
	void Init();
	void Update();
	void Settype(int intype)
	{
		if (type != intype)
		{
			type = (float)intype;
			time = 0;
		}
	}
};
