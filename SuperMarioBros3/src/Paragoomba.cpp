#include "Paragoomba.h"
#include "Animations.h"

void CParagoomba::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
    if (state == PARAGOOMBA_STATE_DIE)
    {
        left = x - PARAGOOMBA_BBOX_WIDTH / 2;
        top = y - PARAGOOMBA_BBOX_HEIGHT_DIE / 2;
        right = left + PARAGOOMBA_BBOX_WIDTH;
        bottom = top + PARAGOOMBA_BBOX_HEIGHT_DIE;
    }
    else
    {
        left = x - PARAGOOMBA_BBOX_WIDTH / 2;
        top = y - PARAGOOMBA_BBOX_HEIGHT / 2;
        right = left + PARAGOOMBA_BBOX_WIDTH;
        bottom = top + PARAGOOMBA_BBOX_HEIGHT;
    }
}

void CParagoomba::OnNoCollision(DWORD dt)
{
    x += vx * dt;
    y += vy * dt;
}

void CParagoomba::OnCollisionWith(LPCOLLISIONEVENT e)
{
    if (!e->obj->IsBlocking()) return;
    if (dynamic_cast<CParagoomba*>(e->obj)) return;

    if (e->ny != 0)
    {
        vy = 0;
    }
    else if (e->nx != 0)
    {
        vx = -vx;
    }
}

void CParagoomba::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
    vy += ay * dt;
    vx += ax * dt;

    if ((state == PARAGOOMBA_STATE_DIE) && (GetTickCount64() - die_start > PARAGOOMBA_DIE_TIMEOUT))
    {
        isDeleted = true;
        return;
    }

    if (state == PARAGOOMBA_STATE_FLYING)
    {
        ULONGLONG now = GetTickCount64();

        if (now - last_fly_time > PARAGOOMBA_FLYING_TIME)
        {
            vy = PARAGOOMBA_FLYING_SPEED;
            last_fly_time = now;
        }
    }

    CheckForEdge(coObjects);

    CGameObject::Update(dt, coObjects);
    CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CParagoomba::Render()
{
    int aniId = ID_ANI_PARAGOOMBA_WALKING;

    if (state == PARAGOOMBA_STATE_DIE)
        aniId = ID_ANI_PARAGOOMBA_DIE;
    else if (state == PARAGOOMBA_STATE_FLYING)
        aniId = ID_ANI_PARAGOOMBA_FLYING;

    CAnimations::GetInstance()->Get(aniId)->Render(x, y);
}

void CParagoomba::SetState(int state)
{
    CGameObject::SetState(state);

    switch (state)
    {
    case PARAGOOMBA_STATE_DIE:
        die_start = GetTickCount64();
        y += (PARAGOOMBA_BBOX_HEIGHT - PARAGOOMBA_BBOX_HEIGHT_DIE) / 2;
        vx = 0;
        vy = 0;
        ay = 0;
        break;

    case PARAGOOMBA_STATE_WALKING:
        vx = -PARAGOOMBA_WALKING_SPEED;
        break;

    case PARAGOOMBA_STATE_FLYING:
        vx = -PARAGOOMBA_WALKING_SPEED;
        ay = PARAGOOMBA_GRAVITY;
        break;
    }
}

void CParagoomba::CheckForEdge(vector<LPGAMEOBJECT>* coObjects)
{
    float px, py;
    GetPosition(px, py);

    float checkX = vx > 0 ? px + PARAGOOMBA_BBOX_WIDTH + 1 : px - PARAGOOMBA_BBOX_WIDTH - 1;
    float checkY = startY + PARAGOOMBA_BBOX_HEIGHT + 1;

    bool foundGround = false;

    for (LPGAMEOBJECT obj : *coObjects)
    {
        if (!obj->IsBlocking()) continue;

        float l, t, r, b;
        obj->GetBoundingBox(l, t, r, b);

        if (checkX >= l && checkX <= r && checkY >= t && checkY <= b)
        {
            foundGround = true;
            break;
        }
    }

    if (!foundGround)
    {
        vx = -vx;
    }
}
