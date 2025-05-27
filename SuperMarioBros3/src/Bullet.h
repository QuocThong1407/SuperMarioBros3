#pragma once
#include "GameObject.h"
#include <math.h>

#define BULLET_BBOX_WIDTH	8
#define BULLET_BBOX_HEIGHT	8

#define BULLET_SPEED	0.08f
#define BULLET_GRAVITY	0.000008f

#define BULLET_STATE_SHOOTING	0

#define ID_ANI_BULLET	16000

class CBullet : public CGameObject
{
protected:
	float ax, ay;
	float targetX, targetY;

public:
	CBullet(float x, float y);

	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = nullptr) override;
	void Render() override;
	void OnNoCollision(DWORD dt) override;
	void GetBoundingBox(float& l, float& t, float& r, float& b) override;
	void SetState(int state) override;

	int IsBlocking() override { return 0; }

	void SetDirection(float angleRad);
};
