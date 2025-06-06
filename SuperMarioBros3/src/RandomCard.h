#pragma once
#include "GameObject.h"

#define CARD_BBOX_WIDTH 16
#define CARD_BBOX_HEIGHT 16

#define ID_ANI_CARD_MUSHROOM 26001
#define ID_ANI_CARD_STAR     26002
#define ID_ANI_CARD_FLOWER   26003

#define RANDOM_CARD_CHANGE_TIME 150

#define CARD_STATE_RUNNING 0
#define CARD_STATE_STOPPED 1

class CRandomCard : public CGameObject
{
protected:
    int currentCard = 0; 
    int state = CARD_STATE_RUNNING;
    ULONGLONG lastChangeTime = 0;

public:
    CRandomCard(float x, float y) : CGameObject(x, y) {}

    void GetBoundingBox(float& l, float& t, float& r, float& b) override;
    void Render() override;
    void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL) override;

    int IsCollidable() override { return 1; }
    int IsBlocking() override { return 0; }

    void Stop(); 
    int GetResult() const { return currentCard; }
};