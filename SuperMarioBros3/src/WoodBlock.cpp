#include "WoodBlock.h"

void CWoodBlock::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	animations->Get(ID_ANI_WOOD_BLOCK)->Render(x, y);
}

void CWoodBlock::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - WOOD_BLOCK_BBOX_WIDTH / 2;
	t = y - WOOD_BLOCK_BBOX_HEIGHT / 2;
	r = l + WOOD_BLOCK_BBOX_WIDTH;
	b = t + WOOD_BLOCK_BBOX_HEIGHT;
}