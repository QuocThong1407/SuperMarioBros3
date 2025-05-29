#pragma once

#include "GameObject.h"

#define ID_ANI_YELLOW_BRICK 22000

#define YELLOW_BRICK_BBOX_WIDTH 16
#define YELLOW_BRICK_BBOX_HEIGHT 16

class CYellowBrick : public CGameObject {
	int type;
public:
	CYellowBrick(float x, float y, int type) : CGameObject(x, y) {
		this->type = type;
	}

	void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) override {}
	void GetBoundingBox(float& l, float& t, float& r, float& b);

	void Break();
};