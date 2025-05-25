#pragma once

#include "GameObject.h"

#define ID_ANI_CLOUD_BLOCK 20000
#define BRICK_WIDTH 16
#define BRICK_BBOX_WIDTH 16
#define BRICK_BBOX_HEIGHT 16

class CCloudBlock : public CGameObject {
public:
	CCloudBlock(float x, float y) : CGameObject(x, y) {}
	void Render();
	void Update(DWORD dt) {}
	void GetBoundingBox(float& l, float& t, float& r, float& b);
};