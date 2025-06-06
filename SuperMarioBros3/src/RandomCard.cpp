#include "RandomCard.h"
#include "Animations.h"
#include "Game.h"

void CRandomCard::GetBoundingBox(float& l, float& t, float& r, float& b) {
    l = x - CARD_BBOX_WIDTH / 2;
    t = y - CARD_BBOX_HEIGHT / 2;
    r = l + CARD_BBOX_WIDTH;
    b = t + CARD_BBOX_HEIGHT;
}

void CRandomCard::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
    if (state == CARD_STATE_RUNNING)
    {
        if (GetTickCount64() - lastChangeTime >= RANDOM_CARD_CHANGE_TIME)
        {
            currentCard = (currentCard + 1) % 3;
            lastChangeTime = GetTickCount64();
        }
    }
}

void CRandomCard::Render()
{
    int aniId = ID_ANI_CARD_STAR;
    if (currentCard == 1) aniId = ID_ANI_CARD_MUSHROOM;
    else if (currentCard == 2) aniId = ID_ANI_CARD_STAR;
    else aniId = ID_ANI_CARD_FLOWER;

    CAnimations::GetInstance()->Get(aniId)->Render(x, y);
}

void CRandomCard::Stop()
{
    state = CARD_STATE_STOPPED;
}