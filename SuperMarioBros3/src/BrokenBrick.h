#pragma once
#include "GameObject.h"

#define BROKEN_BRICK_GRAVITY 0.0005f
#define BROKEN_BRICK_SPEED_X 0.06f
#define BROKEN_BRICK_SPEED_Y 0.18f
#define BROKEN_BRICK_LIFETIME 1000

#define ID_ANI_BROKEN_BRICK_LEFT 23001
#define ID_ANI_BROKEN_BRICK_RIGHT 23002

class CBrokenBrick : public CGameObject {
    ULONGLONG createTime;
    float ay;

public:
    CBrokenBrick(float x, float y, float vx, float vy);

    void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) override;
    void Render() override;
    void GetBoundingBox(float& l, float& t, float& r, float& b) override {}
};