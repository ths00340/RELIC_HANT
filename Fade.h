//==============================================================================
// Filename: Fade.h
// Description :Common�N���X���p�������t�F�[�h�N���X�̒�`
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
