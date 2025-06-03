#pragma once

#include "GameObject.h"

#define ID_ANI_HUD_BACKGROUND        25000

#define ID_ANI_MUSHROOM_CARD         25001
#define ID_ANI_FLOWER_CARD           25002
#define ID_ANI_STAR_CARD             25003

#define ID_ANI_M_BOX                 25004
#define ID_ANI_WHITE_P_BOX           25005
#define ID_ANI_BLACK_P_BOX           25006

#define ID_ANI_ARROW_WHITE           25007
#define ID_ANI_ARROW_BLACK           25008

#define ID_ANI_ICON_X                25009
#define ID_ANI_ICON_CLOCK            25010
#define ID_ANI_ICON_DOLAR            25011

#define ID_ANI_NUMBER_0              25020
#define ID_ANI_NUMBER_1              25021
#define ID_ANI_NUMBER_2              25022
#define ID_ANI_NUMBER_3              25023
#define ID_ANI_NUMBER_4              25024
#define ID_ANI_NUMBER_5              25025
#define ID_ANI_NUMBER_6              25026
#define ID_ANI_NUMBER_7              25027
#define ID_ANI_NUMBER_8              25028
#define ID_ANI_NUMBER_9              25029

#define ID_ANI_CHAR_W                25030
#define ID_ANI_CHAR_O                25031
#define ID_ANI_CHAR_R                25032
#define ID_ANI_CHAR_L                25033
#define ID_ANI_CHAR_D                25034

#define HUD_NUMBER_SPACING           8
#define HUD_CHAR_SPACING             8
#define HUD_ARROW_COUNT              6

class CHud : public CGameObject
{
protected:
    ULONGLONG fullPowerFlashStart;
    bool isFullPowerDark;

public:
    CHud(float x, float y) : CGameObject(x, y)
    {
        fullPowerFlashStart = 0;
        isFullPowerDark = true;
    }

    void Update(DWORD dt);
    void Render() override;
    void GetBoundingBox(float& l, float& t, float& r, float& b) override {}

    void RenderBackground();
    void RenderWorldText();
    void RenderIconX();
    void RenderIconClock();
    void RenderIconDolar();
    void RenderCoins();
    void RenderPoints();
    void RenderTime();
    void RenderLife();
    void RenderFrame();
    void RenderPowerBar();

    void RenderNumber(int number, float x, float y, int maxDigits = 0);
    void RenderChar(char c, float x, float y);
};
