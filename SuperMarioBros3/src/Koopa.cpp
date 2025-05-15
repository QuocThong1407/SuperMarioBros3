#include "Koopa.h"
#include "PlayScene.h"
#include "Mario.h"
#include "Platform.h"
#include "Brick.h"
#include "QuestionBrick.h"

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

    if (isDefend && !isComeback)
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

    if (!isDefend) 
        CheckForEdge(coObjects);

    CGameObject::Update(dt, coObjects);
    CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CKoopa::Render()
{
    int aniId = ID_ANI_KOOPA_WALKING_LEFT;

    if (isDefend) {
        aniId = isComeback ? ID_ANI_KOOPA_COMEBACK : (isKicked ? ID_ANI_KOOPA_KICKED : ID_ANI_KOOPA_DEFEND);
    }
    else if (vx > 0)
        aniId = ID_ANI_KOOPA_WALKING_RIGHT;
    else
        aniId = ID_ANI_KOOPA_WALKING_LEFT;

    CAnimations::GetInstance()->Get(aniId)->Render(x, y);
}

void CKoopa::SetState(int state)
{
    CGameObject::SetState(state);

    switch (state)
    {
    case KOOPA_STATE_WALKING:
        vx = -KOOPA_WALKING_SPEED;
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
    }
}

void CKoopa::OnCollisionWith(LPCOLLISIONEVENT e)
{
    if (!e->obj->IsBlocking()) return;
    if (dynamic_cast<CKoopa*>(e->obj)) return;

    if (e->ny != 0) vy = 0;
    else if (e->nx != 0)
    {
        vx = -vx;
        nx = -nx;
    }

    if (dynamic_cast<CQuestionBrick*>(e->obj))
        OnCollisionWithQuestionBrick(e);
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

    float checkX = vx > 0 ? px + KOOPA_BBOX_WIDTH / 2 + 1 : px - KOOPA_BBOX_WIDTH / 2 - 1;
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



