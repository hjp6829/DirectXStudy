#include "Mouse.h"
#include "Log.h"
#include "string"
void Mouse::SetPos(int x, int y)
{
	pos.x = x;
	pos.y = y;
	int deltaPosXTemp = pos.x - prePos.x;
	int deltaPosYTemp = pos.y - prePos.y;
	if(deltaPosXTemp>1 || deltaPosXTemp<-1)
		deltaPos.x = -deltaPosXTemp;
	else
		deltaPos.x=0;
	if (deltaPosYTemp > 1 || deltaPosYTemp < -1)
		deltaPos.y = deltaPosYTemp;
	else
		deltaPos.y=0;
	prePos.x = x;
	prePos.y = y;
}

void Mouse::SetWheelDelta(float delta)
{
	mouseWheelDelta = delta;
}

