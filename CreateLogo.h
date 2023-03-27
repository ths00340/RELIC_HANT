//==============================================================================
// Filename: CreateLogo.h
// Description :Polygon2Dクラスを継承したロゴ表示用クラスの定義
//==============================================================================

//継承元の汎用化に伴いこちらも削除予定
#pragma once
#include "Polygon2D.h"
class CreateLogo :
	public Polygon2D
{
public:
	void Init();
	void Update();
};
