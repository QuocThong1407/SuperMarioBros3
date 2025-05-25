#pragma once

#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"

#define ID_ANI_COIN 11000
#define ID_ANI_STATIC_COIN 11001

#define	COIN_WIDTH 10
#define COIN_BBOX_WIDTH 14
#define COIN_BBOX_HEIGHT 16

#define COIN_GRAVITY 0.003f
#define COIN_JUMP_SPEED_Y 0.6f

#define COIN_STATE_IDLE 0
#define COIN_STATE_JUMP 1

class CCoin : public CGameObject {
	bool isCollected = false;
	float ax = 0;
	float ay = COIN_GRAVITY;
	float collect_y = 0;
public:
	CCoin(float x, float y) : CGameObject(x, y) {
		SetState(COIN_STATE_IDLE);
	}

	void GetBoundingBox(float& l, float& t, float& r, float& b);
	void Render();
	void Update(DWORD dt, std::vector<LPGAMEOBJECT>* coObjects);
	void OnNoCollision(DWORD dt) override;
	void SetState(int state);
	int IsBlocking() override { return 0; }

	bool IsCollected() { return isCollected; }
};