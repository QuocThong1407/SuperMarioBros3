#pragma once
#include "GameObject.h"

#define SUPERLEAF_BBOX_WIDTH 14
#define SUPERLEAF_BBOX_HEIGHT 16

#define SUPERLEAF_GRAVITY 0.0003f
#define SUPERLEAF_FLOAT_SPEED 0.015f
#define SUPERLEAF_FLOAT_PERIOD 800

#define SUPERLEAF_RISE_SPEED 0.18f
#define SUPERLEAF_RISE_HEIGHT 22.0f

#define ID_ANI_SUPERLEAF 19000

#define SUPERLEAF_STATE_RISING 0
#define SUPERLEAF_STATE_FLOATING 1

class CSuperLeaf : public CGameObject
{
protected:
    float ay;
    float startY;
    ULONGLONG float_start;

public:
    CSuperLeaf(float x, float y);

    void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) override;
    void Render() override;
    void GetBoundingBox(float& l, float& t, float& r, float& b) override;
    void OnNoCollision(DWORD dt) override;

    void SetState(int state) override;

    int IsCollidable() override { return 1; }
    int IsBlocking() override { return 0; }
};