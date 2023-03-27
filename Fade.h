//==============================================================================
// Filename: Fade.h
// Description :Commonクラスを継承したフェードクラスの定義
//==============================================================================
#pragma once
#include "Common.h"
#include <iostream>

class Fade :
	public Common
{
public:
	static void Load();
	static void UnLoad();
	void Init()		override;
	void Uninit()	override;
	void Update()	override;
	void Draw()		override;
};
