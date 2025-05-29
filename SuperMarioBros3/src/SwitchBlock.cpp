#include "SwitchBlock.h"

void CSwitchBlock::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
    if (state == SWITCH_BLOCK_STATE_RISING)
    {
        y += vy * dt;

        if (y <= maxY)
        {
            y = maxY;
            vy = 0;
            SetState(SWITCH_BLOCK_STATE_DEFAULT);
        }
    }

    CGameObject::Update(dt, coObjects);
    CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CSwitchBlock::Render()
{
    int aniId = ID_ANI_SWITCH_BLOCK_DEFAULT;

    if (state == SWITCH_BLOCK_STATE_DISABLED)
        aniId = ID_ANI_SWITCH_BLOCK_DISABLED;

    CAnimations::GetInstance()->Get(aniId)->Render(x, y);
}

void CSwitchBlock::GetBoundingBox(float& l, float& t, float& r, float& b)
{
    if (state == SWITCH_BLOCK_STATE_DISABLED) {
        l = x - SWITCH_BLOCK_BBOX_WITDH / 2;
        t = y - SWITCH_BLOCK_DISABLED_BBOX_HEIGHT / 2;
        r = l + SWITCH_BLOCK_BBOX_WITDH;
        b = t + SWITCH_BLOCK_DISABLED_BBOX_HEIGHT;
    }
    else {
        l = x - SWITCH_BLOCK_BBOX_WITDH / 2;
        t = y - SWITCH_BLOCK_BBOX_HEIGHT / 2;
        r = l + SWITCH_BLOCK_BBOX_WITDH;
        b = t + SWITCH_BLOCK_BBOX_HEIGHT;
    }
    
}

void CSwitchBlock::SetState(int state)
{
    CGameObject::SetState(state);

    switch (state)
    {
    case SWITCH_BLOCK_STATE_RISING:
        vy = SWITCH_BLOCK_SPEED;
        break;

    case SWITCH_BLOCK_STATE_DEFAULT:
        vy = 0;
        break;

    case SWITCH_BLOCK_STATE_DISABLED:
        vy = 0;
        SetPosition(x, y + 5);
        break;
    }
}