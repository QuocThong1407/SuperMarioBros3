#pragma once
#include "GameObject.h"

#define PARAGOOMBA_BBOX_WIDTH   16
#define PARAGOOMBA_BBOX_HEIGHT  14
#define PARAGOOMBA_BBOX_HEIGHT_DIE  7

#define PARAGOOMBA_WALKING_SPEED    0.05f
#define PARAGOOMBA_GRAVITY  0.0005f
#define PARAGOOMBA_FLYING_SPEED -0.2f
#define PARAGOOMBA_FLYING_TIME  1300


#define PARAGOOMBA_STATE_WALKING    100
#define PARAGOOMBA_STATE_FLYING 300
#define PARAGOOMBA_STATE_DIE    200

#define ID_ANI_PARAGOOMBA_WALKING   17001
#define ID_ANI_PARAGOOMBA_FLYING    17002
#define ID_ANI_PARAGOOMBA_DIE   17003

#define PARAGOOMBA_DIE_TIMEOUT  500

class CParagoomba : public CGameObject
{
protected:
    float ax;
    float ay;
    float startX;
    float startY;
    ULONGLONG die_start;
    ULONGLONG last_fly_time = 0;

    virtual void OnNoCollision(DWORD dt);
    virtual void OnCollisionWith(LPCOLLISIONEVENT e);

public:
    CParagoomba(float x, float y) : CGameObject(x, y) {
        ax = 0;
        ay = PARAGOOMBA_GRAVITY;
        die_start = -1;
        last_fly_time = GetTickCount64();
        GetPosition(startX, startY);
        SetState(PARAGOOMBA_STATE_FLYING);
    }

    virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom) override;
    virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) override;
    virtual void Render() override;

    virtual int IsCollidable() override { return 1; };
    virtual int IsBlocking() override { return 0; }

    virtual void SetState(int state) override;

    void CheckForEdge(vector<LPGAMEOBJECT>* coObjects);
};
