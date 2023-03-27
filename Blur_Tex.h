//==============================================================================
// Filename: Blur_Tex.h
// Description :Polygon2Dクラスを継承した半透明テクスチャの表示クラスの定義
//==============================================================================

//完成に間に合わせるためにUI周りを突貫で作ったためこちらも汎用的に使えるよう変更予定
#pragma once
#include "Polygon2D.h"
class Blur_Tex :
	public Polygon2D
{
protected:
	int time = 0;
	int maxtime = 0;
	bool draw = false;
	bool drawIn = false;
public:
	virtual void Init();
	virtual void Update();
	void SetDraw(bool in) { draw = in; }
	bool GetIn();
	void SetMax(int in) {
		time = 0;
		maxtime = in;
	}
};
