//==============================================================================
// Filename: LoadingLogo.h
// Description :Polygon2Dクラスを継承したローディング用アイコン表示クラスの定義
//==============================================================================

//アセット自体が軽量なため使用せず
#pragma once
#include "Polygon2D.h"
class LoadingLogo :
	public Polygon2D
{
public:
	void Init();
	void Update();
};
