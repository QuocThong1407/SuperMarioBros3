#include "YellowBrick.h"

void CYellowBrick::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	animations->Get(ID_ANI_YELLOW_BRICK)->Render(x, y);
}

void CYellowBrick::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - YELLOW_BRICK_BBOX_WIDTH / 2;
	t = y - YELLOW_BRICK_BBOX_HEIGHT / 2;
	r = l + YELLOW_BRICK_BBOX_WIDTH;
	b = t + YELLOW_BRICK_BBOX_HEIGHT;
}