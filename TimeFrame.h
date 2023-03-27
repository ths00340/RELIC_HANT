//==============================================================================
// Filename: TimeFrame.h
// Description :Polygon2Dクラスを継承した時計のフレーム表示用クラスの定義
//==============================================================================
#pragma once
#include "Polygon2D.h"
class TimeFrame :
	public Polygon2D
{
public:
	void Init();
	void Update() {};
	void Uninit();
};
