#pragma once
#include "GameObject.h"

#define SUPER_MUSHROOM_BBOX_WIDTH 16
#define SUPER_MUSHROOM_BBOX_HEIGHT 16

#define SUPER_MUSHROOM_GRAVITY 0.0005f
#define SUPER_MUSHROOM_SPEED_X 0.03f

#define ID_ANI_SUPER_MUSHROOM 14000

class CSuperMushroom : public CGameObject
{
protected:
	float ax, ay;

public:
	CSuperMushroom(float x, float y) : CGameObject(x, y) {
		ax = 0;
		ay = SUPER_MUSHROOM_GRAVITY;
		vx = SUPER_MUSHROOM_SPEED_X;
	}

	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = nullptr) override;
	void Render() override;
	void GetBoundingBox(float& l, float& t, float& r, float& b) override;
	void OnNoCollision(DWORD dt) override;

	int IsCollidable() override { return 1; }
	int IsBlocking() override { return 0; }
};