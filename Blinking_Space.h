//==============================================================================
// Filename: Blinking_Space.h
// Description: Polygon2D�N���X���p�������_�ŃN���X�̒�`
//==============================================================================

//�p�����̔ėp���ɔ�����������ėp�I�Ɏg����悤�ɕύX�\��
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
