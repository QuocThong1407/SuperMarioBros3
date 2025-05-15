#pragma once
#include "GameObject.h"

#define KOOPA_GRAVITY 0.002f

#define KOOPA_WALKING_SPEED 0.04f
#define KOOPA_KICK_SPEED 0.25f

#define KOOPA_BBOX_WIDTH 16
#define KOOPA_BBOX_HEIGHT 26
#define KOOPA_BBOX_HEIGHT_DEFEND 16

#define KOOPA_HOLD_OFFSET_X 10
#define KOOPA_HOLD_OFFSET_Y 2

#define KOOPA_DEFEND_TIMEOUT 5000
#define KOOPA_COMEBACK_TIMEOUT 2000

#define KOOPA_STATE_WALKING 100
#define KOOPA_STATE_DEFEND 200
#define KOOPA_STATE_COMEBACK 300
#define KOOPA_STATE_KICKED 400

#define ID_ANI_KOOPA_WALKING_RIGHT 18001
#define ID_ANI_KOOPA_WALKING_LEFT 18002
#define ID_ANI_KOOPA_DEFEND 18003
#define ID_ANI_KOOPA_COMEBACK 18004
#define ID_ANI_KOOPA_KICKED 18005

class CKoopa : public CGameObject
{
protected:
	float ax, ay;
	int koopaType = 1;

	bool isDefend = false;
	bool isComeback = false;
	bool isKicked = false;
	bool isHeld = false;

	ULONGLONG defend_start = -1;
	ULONGLONG comeback_start = -1;

public:
	CKoopa(float x, float y) : CGameObject(x, y) {
		ax = 0;
		ay = KOOPA_GRAVITY;
		nx = -1;
		SetState(KOOPA_STATE_WALKING);
	}

	void GetBoundingBox(float& l, float& t, float& r, float& b) override;
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) override;
	void Render() override;
	void OnNoCollision(DWORD dt) override;
	void OnCollisionWith(LPCOLLISIONEVENT e) override;
	void CheckForEdge(vector<LPGAMEOBJECT>* coObjects);

	void BeKicked(float dir);

	void SetState(int state) override;

	bool IsDefend() { return isDefend; }
	bool IsKicked() { return isKicked; }
	bool IsComeback() { return isComeback; }
	int IsCollidable() override { return 1; };
	int IsBlocking() override { return !isDefend; }
};
