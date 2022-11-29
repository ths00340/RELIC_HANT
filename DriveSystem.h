#pragma once
#include "CComponent.h"
class DriveSystem :
	public CComponent
{
public:
	DriveSystem(GameObject* inobject) :CComponent(inobject) {
	};
};
