#include "Koopa.h"
#include "PlayScene.h"
#include "Mario.h"
#include "Platform.h"
#include "Brick.h"
#include "QuestionBrick.h"
#include "Goomba.h"
#include "Paragoomba.h"
#include "YellowBrick.h"

void CKoopa::GetBoundingBox(float& l, float& t, float& r, float& b)
{
    if (isDefend)
    {
        l = x - KOOPA_BBOX_WIDTH / 2;
        t = y - KOOPA_BBOX_HEIGHT_DEFEND / 2;
        r = l + KOOPA_BBOX_WIDTH;
        b = t + KOOPA_BBOX_HEIGHT_DEFEND;
    }
    else
    {
        l = x - KOOPA_BBOX_WIDTH / 2;
        t = y - KOOPA_BBOX_HEIGHT / 2;
        r = l + KOOPA_BBOX_WIDTH;
        b = t + KOOPA_BBOX_HEIGHT;
    }
}

void CKoopa::OnNoCollision(DWORD dt) {
    x += vx * dt;
    y += vy * dt;
}

void CKoopa::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
    vy += ay * dt;
    vx += ax * dt;

    if (koopaType == 3) {
        if (GetTickCount64() - flyStart >= KOOPA_FLYING_TIME && isOnPlatform) {
            vy = KOOPA_FLYING_SPEED_Y;
            isOnPlatform = false;
            flyStart = GetTickCount64();
        }
    }

    if (isDefend && !isComeback && !isKicked)
    {
        if (GetTickCount64() - defend_start > KOOPA_DEFEND_TIMEOUT) {
            SetState(KOOPA_STATE_COMEBACK);
        }

    }
    else if (isComeback)
    {
        if (GetTickCount64() - comeback_start > KOOPA_COMEBACK_TIMEOUT) {
            AdjustPositionAfterDefend();
            SetState(KOOPA_STATE_WALKING);
        }
    }

    if (!isDefend && koopaType == 1) 
        CheckForEdge(coObjects);

    CGameObject::Update(dt, coObjects);
    CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CKoopa::Render()
{
    int aniId = -1;

    if (state == KOOPA_STATE_WALKING) {
        if (vx > 0) {
            if (koopaType == 1) 
                aniId = ID_ANI_RED_KOOPA_WALKING_RIGHT;
            else if (koopaType == 2) 
                aniId = ID_ANI_GREEN_KOOPA_WALKING_RIGHT;
        }
        else {
            if (koopaType == 1)
                aniId = ID_ANI_RED_KOOPA_WALKING_LEFT;
            else if (koopaType == 2)
                aniId = ID_ANI_GREEN_KOOPA_WALKING_LEFT;
        }
    }
    else if (state == KOOPA_STATE_FLYING) {
        if (vx > 0) {
            aniId = ID_ANI_FLYING_GREEN_KOOPA_WALKING_RIGHT;
        }
        else {
            aniId = ID_ANI_FLYING_GREEN_KOOPA_WALKING_LEFT;
        }
    }
    else if (state == KOOPA_STATE_DEFEND) {
        if (koopaType == 1)
            aniId = ID_ANI_RED_KOOPA_DEFEND;
        else if (koopaType == 2)
            aniId = ID_ANI_GREEN_KOOPA_DEFEND;
    }
    else if (state == KOOPA_STATE_COMEBACK) {
        if (koopaType == 1)
            aniId = ID_ANI_RED_KOOPA_COMEBACK;
        else if (koopaType == 2)
            aniId = ID_ANI_GREEN_KOOPA_COMEBACK;
    }
    else if (state == KOOPA_STATE_KICKED) {
        if (koopaType == 1)
            aniId = ID_ANI_RED_KOOPA_KICKED;
        else if (koopaType == 2)
            aniId = ID_ANI_GREEN_KOOPA_KICKED;
    }
    else if (state == KOOPA_STATE_HELD) {
        if (koopaType == 1)
            aniId = ID_ANI_RED_KOOPA_DEFEND;
        else if (koopaType == 2)
            aniId = ID_ANI_GREEN_KOOPA_DEFEND;
    }

    CAnimations::GetInstance()->Get(aniId)->Render(x, y);
}

void CKoopa::SetState(int state)
{
    CGameObject::SetState(state);

    switch (state)
    {
    case KOOPA_STATE_WALKING:
        vx = -KOOPA_WALKING_SPEED;
        ay = KOOPA_GRAVITY;
        isDefend = false;
        isComeback = false;
        isKicked = false;
        isHeld = false;
        break;

    case KOOPA_STATE_DEFEND:
        vx = 0;
        isDefend = true;
        isComeback = false;
        isKicked = false;
        isHeld = false;
        defend_start = GetTickCount64();
        break;

    case KOOPA_STATE_COMEBACK:
        isComeback = true;
        isKicked = false;
        isHeld = false;
        comeback_start = GetTickCount64();
        break;

    case KOOPA_STATE_KICKED:
        isKicked = true;
        isDefend = true;
        isComeback = false;
        isHeld = false;
        vx = KOOPA_KICK_SPEED * nx;
        break;

    case KOOPA_STATE_HELD:
        vx = 0;
        vy = 0;
        isHeld = true;
        break;

    case KOOPA_STATE_FLYING:
        vx = -KOOPA_FLYING_SPEED_X;
        ay = FLYING_KOOPA_GRAVITY;
        flyStart = GetTickCount64();
        break;
    }

}

void CKoopa::OnCollisionWith(LPCOLLISIONEVENT e)
{
    if (dynamic_cast<CGoomba*>(e->obj))
        OnCollisionWithEnemy(e);
    if (dynamic_cast<CParagoomba*>(e->obj))
        OnCollisionWithEnemy(e);

    if (!e->obj->IsBlocking()) return;
    if (dynamic_cast<CKoopa*>(e->obj)) return;

    if (e->ny != 0) {
        vy = 0;
        isOnPlatform = true;
    }
    else if (e->nx != 0)
    {
        vx = -vx;
        nx = -nx;
    }

    if (dynamic_cast<CQuestionBrick*>(e->obj))
        OnCollisionWithQuestionBrick(e); 
    if (dynamic_cast<CYellowBrick*>(e->obj))
        OnCollisionWithYellowBrick(e);
}

void CKoopa::OnCollisionWithQuestionBrick(LPCOLLISIONEVENT e)
{
    if (!e->obj->IsBlocking()) return;

    CQuestionBrick* qb = dynamic_cast<CQuestionBrick*>(e->obj);
    if (!qb) return;

    if (this->IsKicked() && e->nx != 0)
    {
        if (!qb->GetIsUnboxed())
        {
            qb->SetState(QUESTION_BRICK_STATE_UNBOXING);
        }
    }
}

void CKoopa::OnCollisionWithYellowBrick(LPCOLLISIONEVENT e) {
    if (!e->obj->IsBlocking()) return;

    CYellowBrick* brick = dynamic_cast<CYellowBrick*>(e->obj);

    if (this->IsKicked() && e->nx != 0)
        brick->Break();
}

void CKoopa::OnCollisionWithEnemy(LPCOLLISIONEVENT e) {
    if (!isKicked) return;

    if (e->nx != 0) {
        if (dynamic_cast<CGoomba*>(e->obj))
        {
            CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj);
            goomba->SetState(GOOMBA_STATE_DIE);
        }

        if (dynamic_cast<CParagoomba*>(e->obj))
        {
            CParagoomba* pgoomba = dynamic_cast<CParagoomba*>(e->obj);
            if (pgoomba->GetState() == PARAGOOMBA_STATE_FLYING)
            {
                pgoomba->SetState(PARAGOOMBA_STATE_WALKING);
            }
            else
            {
                pgoomba->SetState(PARAGOOMBA_STATE_DIE);
            }
        }
    }
}


void CKoopa::BeKicked(float direction)
{
    nx = direction > 0 ? 1 : -1;
    SetState(KOOPA_STATE_KICKED);
}

void CKoopa::AdjustPositionAfterDefend()
{
    y -= (KOOPA_BBOX_HEIGHT - KOOPA_BBOX_HEIGHT_DEFEND) / 2;
}

void CKoopa::CheckForEdge(vector<LPGAMEOBJECT>* coObjects) {
    float px, py;
    GetPosition(px, py);

    float checkX = vx > 0 ? px + KOOPA_BBOX_WIDTH / 2 - 3: px - KOOPA_BBOX_WIDTH / 2 + 3;
    float checkY = py + KOOPA_BBOX_HEIGHT + 1;

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



