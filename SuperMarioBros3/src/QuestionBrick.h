#pragma once 
#include "GameObject.h"

#define QUESTION_BRICK_BBOX_WIDTH 16
#define QUESTION_BRICK_BBOX_HEIGHT 16

#define QUESTION_BRICK_BOUNCE_SPEED 0.15f
#define QUESTION_BRICK_GRAVITY 0.0015f

// States
#define QUESTION_BRICK_STATE_UNBOXING 1
#define QUESTION_BRICK_STATE_DISABLED 2

// Item types
#define QUESTION_BRICK_ITEM_NONE 0
#define QUESTION_BRICK_ITEM_COIN 1
#define QUESTION_BRICK_ITEM_MUSHROOM 2
#define QUESTION_BRICK_ITEM_SUPER_LEAF 3

// Animation IDs
#define ID_ANI_QUESTION_BRICK 12000
#define ID_ANI_QUESTION_BRICK_DISABLED 12001

class CQuestionBrick : public CGameObject {
protected:
	bool isUnboxed = false;
	int item;
	float originalY;
	float ax, ay; 
public:
	CQuestionBrick(float x, float y, int item = QUESTION_BRICK_ITEM_COIN) : CGameObject(x, y) {
		this->item = item;
		this->originalY = y;
		this->ax = 0;
		this->ay = 0;
	}

	void GetBoundingBox(float& l, float& t, float& r, float& b) override;
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) override;
	void Render() override;
	void OnNoCollision(DWORD dt) override;
	void SetState(int state) override;

	int IsBlocking() override { return 1; }
	int IsCollidable() override { return !isUnboxed; }

	void Unbox() { SetState(QUESTION_BRICK_STATE_UNBOXING); }
	void Disable() { SetState(QUESTION_BRICK_STATE_DISABLED); }

	int GetIsUnboxed() { return isUnboxed; }
	int GetItem() { return item; }
};