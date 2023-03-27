//==============================================================================
// Filename: Blinking_Space.h
// Description: Polygon2Dクラスを継承した点滅クラスの定義
//==============================================================================

//継承元の汎用化に伴いこちらも汎用的に使えるように変更予定
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
