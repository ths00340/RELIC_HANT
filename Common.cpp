#include "Common.h"
#include "Tools.h"

void Common::Set(float time)
{
	DestroyTime = TOOL::FrameMulti(time);
}
