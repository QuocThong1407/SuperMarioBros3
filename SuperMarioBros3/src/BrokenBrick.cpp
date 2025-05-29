#include "BrokenBrick.h"
#include "Animations.h"

CBrokenBrick::CBrokenBrick(float x, float y, float vx, float vy)
    : CGameObject(x, y)
{
    this->vx = vx;
    this->vy = vy;
    this->ay = BROKEN_BRICK_GRAVITY;
    createTime = GetTickCount64();
}

void CBrokenBrick::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
    vy += ay * dt;
    x += vx * dt;
    y += vy * dt;

    if (GetTickCount64() - createTime > BROKEN_BRICK_LIFETIME)
        isDeleted = true;
}

void CBrokenBrick::Render() {
    int aniId = vx < 0 ? ID_ANI_BROKEN_BRICK_LEFT : ID_ANI_BROKEN_BRICK_RIGHT;
    CAnimations::GetInstance()->Get(aniId)->Render(x, y);

}