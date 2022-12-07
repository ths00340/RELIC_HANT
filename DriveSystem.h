//==============================================================================
// Filename: DriveSystem.h
// Description: コンポーネントクラスを継承した移動用クラスの定義
//==============================================================================
#pragma once
#include "CComponent.h"
class DriveSystem :
	public CComponent
{
public:
	DriveSystem(GameObject* inobject) :CComponent(inobject) {
	};
};
