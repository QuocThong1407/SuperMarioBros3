#include "CloudBlock.h"

void CCloudBlock::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	animations->Get(ID_ANI_CLOUD_BLOCK)->Render(x, y);
}

void CCloudBlock::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - CLOUD_BLOCK_BBOX_WIDTH / 2;
	t = y - CLOUD_BLOCK_BBOX_HEIGHT / 2;
	r = l + CLOUD_BLOCK_BBOX_WIDTH;
	b = t + CLOUD_BLOCK_BBOX_HEIGHT;
}