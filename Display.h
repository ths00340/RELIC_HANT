//==============================================================================
// Filename: Display.h
// Description :Polygon2Dクラスを継承したデファードレンダリング用表示クラスの定義
//==============================================================================
#pragma once
#include "Polygon2D.h"
class Display :
	public Polygon2D
{
public:
	void Init();
	void Update() {};
};
