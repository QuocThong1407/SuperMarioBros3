#pragma once
#include "GameObject.h"

#define KOOPA_GRAVITY 0.002f
#define FLYING_KOOPA_GRAVITY 0.00035f

#define KOOPA_WALKING_SPEED 0.04f
#define KOOPA_FLYING_SPEED_X 0.042f
#define KOOPA_FLYING_SPEED_Y -0.18f
#define KOOPA_KICK_SPEED 0.2f

#define KOOPA_BBOX_WIDTH 16
#define KOOPA_BBOX_HEIGHT 26
#define KOOPA_BBOX_HEIGHT_DEFEND 16

#define KOOPA_DEFEND_TIMEOUT 5000
#define KOOPA_COMEBACK_TIMEOUT 2000
#define KOOPA_FLYING_TIME 1300

#define KOOPA_STATE_WALKING 100
#define KOOPA_STATE_DEFEND 200
#define KOOPA_STATE_COMEBACK 300
#define KOOPA_STATE_KICKED 400
#define KOOPA_STATE_HELD 500
#define KOOPA_STATE_FLYING 600

#define ID_ANI_RED_KOOPA_WALKING_RIGHT 18001
#define ID_ANI_RED_KOOPA_WALKING_LEFT 18002
#define ID_ANI_RED_KOOPA_DEFEND 18003
#define ID_ANI_RED_KOOPA_COMEBACK 18004
#define ID_ANI_RED_KOOPA_KICKED 18005

#define ID_ANI_GREEN_KOOPA_WALKING_RIGHT 18006
#define ID_ANI_GREEN_KOOPA_WALKING_LEFT 18007
#define ID_ANI_GREEN_KOOPA_DEFEND 18008
#define ID_ANI_GREEN_KOOPA_COMEBACK 18009
#define ID_ANI_GREEN_KOOPA_KICKED 18010

#define ID_ANI_FLYING_GREEN_KOOPA_WALKING_RIGHT 18011
#define ID_ANI_FLYING_GREEN_KOOPA_WALKING_LEFT 18012


class CKoopa : public CGameObject
{
protected:
	float ax, ay;
	int koopaType = 1;
	ULONGLONG flyStart;

	bool isOnPlatform = false;

	bool isDefend = false;
	bool isComeback = false;
	bool isKicked = false;
	bool isHeld = false;

	ULONGLONG defend_start = -1;
	ULONGLONG comeback_start = -1;

public:
	CKoopa(float x, float y, int koopaType) : CGameObject(x, y) {
		ax = 0;
		ay = KOOPA_GRAVITY;
		nx = -1;
		this->koopaType = koopaType;

		if (koopaType == 3)
			SetState(KOOPA_STATE_FLYING);
		else
			SetState(KOOPA_STATE_WALKING);
	}

	void GetBoundingBox(float& l, float& t, float& r, float& b) override;
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) override;
	void Render() override;
	void OnNoCollision(DWORD dt) override;
	void OnCollisionWith(LPCOLLISIONEVENT e) override;
	void OnCollisionWithQuestionBrick(LPCOLLISIONEVENT e);
	void OnCollisionWithEnemy(LPCOLLISIONEVENT e);
	void OnCollisionWithYellowBrick(LPCOLLISIONEVENT e);
	void CheckForEdge(vector<LPGAMEOBJECT>* coObjects);

	void BeKicked(float dir);

	void SetState(int state) override;

	bool IsDefend() { return isDefend; }
	bool IsKicked() { return isKicked; }
	bool IsComeback() { return isComeback; }
	bool IsHeld() { return isHeld; }
	int IsCollidable() override { return 1; };
	int IsBlocking() override { return 1; }
	
	void AdjustPositionAfterDefend();
	int GetType() { return koopaType; }
	void SetType(int type) { this->koopaType = type; }
};
