//==============================================================================
// Filename: Title2D.h
// Description :Polygon2Dクラスを継承したタイトル表示用クラスの定義
//==============================================================================
#pragma once
class Polygon2D;

class Title2D :
	public Polygon2D
{
private:
	float updown = 0.f;
	Float2 defaultpos;
public:
	void Init();
	void Update();
};
