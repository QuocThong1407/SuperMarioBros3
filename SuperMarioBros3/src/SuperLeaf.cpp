#include "SuperLeaf.h"
#include "Animations.h"
#include "PlayScene.h"
#include "Mario.h"

CSuperLeaf::CSuperLeaf(float x, float y) : CGameObject(x, y)
{
    startY = y;
    ay = SUPERLEAF_GRAVITY;
    SetState(SUPERLEAF_STATE_RISING);
}

void CSuperLeaf::OnNoCollision(DWORD dt) {
    x += vx * dt;
    y += vy * dt;
}

void CSuperLeaf::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
    vy += ay * dt;

    if (state == SUPERLEAF_STATE_RISING)
    {
        if (vy > 0 && startY - y <= SUPERLEAF_RISE_HEIGHT)
        {
            SetState(SUPERLEAF_STATE_FLOATING);
        }
    }
    else if (state == SUPERLEAF_STATE_FLOATING)
    {
        if (GetTickCount64() - float_start >= SUPERLEAF_FLOAT_PERIOD)
        {
            vy = -vy;
            float_start = GetTickCount64();
        }
    }

    CGameObject::Update(dt, coObjects);
    CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CSuperLeaf::Render()
{
    CAnimations::GetInstance()->Get(ID_ANI_SUPERLEAF)->Render(x, y);
}

void CSuperLeaf::GetBoundingBox(float& l, float& t, float& r, float& b)
{
    l = x - SUPERLEAF_BBOX_WIDTH / 2;
    t = y - SUPERLEAF_BBOX_HEIGHT / 2;
    r = l + SUPERLEAF_BBOX_WIDTH;
    b = t + SUPERLEAF_BBOX_HEIGHT;
}

void CSuperLeaf::SetState(int state)
{
    CGameObject::SetState(state);

    switch (state)
    {
    case SUPERLEAF_STATE_RISING:
        vy = -SUPERLEAF_RISE_SPEED;
        vx = 0;
        break;

    case SUPERLEAF_STATE_FLOATING:
        vy = SUPERLEAF_FLOAT_SPEED;
        ay = 0;
        float_start = GetTickCount64();
        break;
    }
}