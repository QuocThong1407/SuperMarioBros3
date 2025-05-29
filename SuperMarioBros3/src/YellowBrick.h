#pragma once

#include "GameObject.h"

#define ID_ANI_YELLOW_BRICK 22000

#define YELLOW_BRICK_BBOX_WIDTH 16
#define YELLOW_BRICK_BBOX_HEIGHT 16

class CYellowBrick : public CGameObject {
public:
	CYellowBrick(float x, float y) : CGameObject(x, y) {}
	void Render();
	void Update(DWORD dt) {}
	void GetBoundingBox(float& l, float& t, float& r, float& b);

	void Break();
};