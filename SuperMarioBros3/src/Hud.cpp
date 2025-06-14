#include "Hud.h"
#include "Animations.h"
#include "Game.h"
#include "Mario.h"
#include "PlayScene.h"
#include "GameData.h"

void CHud::Update(DWORD dt)
{
	CGameData::GetInstance()->CountDownRemainTime();

	if (isFullPowerDark && GetTickCount64() - fullPowerFlashStart > 300)
	{
		isFullPowerDark = false;
		fullPowerFlashStart = GetTickCount64();
	}
	else if (!isFullPowerDark && GetTickCount64() - fullPowerFlashStart > 300)
	{
		isFullPowerDark = true;
		fullPowerFlashStart = GetTickCount64();
	}
}

void CHud::Render()
{
	RenderBackground();
	RenderWorldText();
	RenderFrame();
	RenderCards();
	RenderIconX();
	RenderLife();
	RenderPoints();
	RenderCoins();
	RenderTime();
	RenderPowerBar();
	RenderIconClock();
	RenderIconDolar();
}

void CHud::RenderBackground()
{
	CAnimations::GetInstance()->Get(ID_ANI_HUD_BACKGROUND)->Render(x, y);
}

void CHud::RenderWorldText()
{
	float tx = x - 112;
	float ty = y - 8;

	RenderChar('W', tx, ty);
	RenderChar('O', tx + HUD_CHAR_SPACING, ty);
	RenderChar('R', tx + HUD_CHAR_SPACING * 2, ty);
	RenderChar('L', tx + HUD_CHAR_SPACING * 3, ty);
	RenderChar('D', tx + HUD_CHAR_SPACING * 4, ty);
	RenderNumber(1, tx + HUD_CHAR_SPACING * 5 + 2, ty + 1); 
}

void CHud::RenderIconX()
{
	CAnimations::GetInstance()->Get(ID_ANI_ICON_X)->Render(x - 94, y + 1);
}

void CHud::RenderIconClock()
{
	CAnimations::GetInstance()->Get(ID_ANI_ICON_CLOCK)->Render(x - 4, y + 2);
}

void CHud::RenderIconDolar()
{
	CAnimations::GetInstance()->Get(ID_ANI_ICON_DOLAR)->Render(x + 5, y - 9);
}

void CHud::RenderCoins()
{
	int coin = CGameData::GetInstance()->GetCoin();

	if (coin < 10)
		RenderNumber(coin, x + 22, y - 8, 1);
	else
		RenderNumber(coin, x + 14, y - 8, 2);
}

void CHud::RenderTime()
{
	int time = CGameData::GetInstance()->GetRemainTime();
	RenderNumber(time, x + 6, y + 2, 3);
}

void CHud::RenderLife()
{
	int life = CGameData::GetInstance()->GetLife();
	RenderNumber(life, x - 76, y + 2);
}

void CHud::RenderPoints()
{
	int points = CGameData::GetInstance()->GetPoint();
	RenderNumber(points, x - 60, y + 2, 6);
}

void CHud::RenderFrame()
{
	CAnimations* animations = CAnimations::GetInstance();
	LPPLAYSCENE scene = dynamic_cast<LPPLAYSCENE>(CGame::GetInstance()->GetCurrentScene());
	CMario* mario = dynamic_cast<CMario*>(scene->GetPlayer());

	animations->Get(ID_ANI_M_BOX)->Render(x - 108, y);

	if (mario->IsFullPower()) 
		animations->Get(ID_ANI_WHITE_P_BOX)->Render(x - 7, y - 8);
	else
		animations->Get(ID_ANI_BLACK_P_BOX)->Render(x - 7, y - 8);
}

void CHud::RenderCards()
{
	int cardCount = CGameData::GetInstance()->GetCardCount();
	if (cardCount == 0) return;

	float startX = x + 50; 
	float yPos = y - 3;

	float tx = x - 40;
	float ty = y - 180;

	std::string line1 = "COURSE CLEAR";
	std::string line2 = "YOU GOT A CARD";

	for (int i = 0; i < line1.length(); i++)
		RenderChar(line1[i], tx + i * HUD_CHAR_SPACING, ty);

	for (int i = 0; i < line2.length(); i++)
		RenderChar(line2[i], tx + i * HUD_CHAR_SPACING - 10, ty + 12);

	int cardType = CGameData::GetInstance()->GetCard(0);

	int aniId = ID_ANI_STAR_CARD;

	if (cardType == 1) aniId = ID_ANI_MUSHROOM_CARD;
	else if (cardType == 2) aniId = ID_ANI_STAR_CARD;
	else aniId = ID_ANI_FLOWER_CARD;

	CAnimations::GetInstance()->Get(aniId)->Render(x + 80, y - 170);

	for (int i = 0; i < cardCount; i++)
	{
		int cardType = CGameData::GetInstance()->GetCard(i);

		int aniId = ID_ANI_STAR_CARD;

		if (cardType == 1) aniId = ID_ANI_MUSHROOM_CARD;
		else if (cardType == 2) aniId = ID_ANI_STAR_CARD;
		else aniId = ID_ANI_FLOWER_CARD;

		CAnimations::GetInstance()->Get(aniId)->Render(startX + i * 25, yPos);
	}
}

void CHud::RenderPowerBar()
{
	LPPLAYSCENE scene = dynamic_cast<LPPLAYSCENE>(CGame::GetInstance()->GetCurrentScene());
	if (!scene) return;

	CMario* mario = dynamic_cast<CMario*>(scene->GetPlayer());
	if (!mario) return;

	float startX = x - 60;
	float startY = y - 8;

	for (int i = 0; i < HUD_ARROW_COUNT; i++)
	{
		if (i < mario->GetPower())
			CAnimations::GetInstance()->Get(ID_ANI_ARROW_WHITE)->Render(startX + i * 8, startY);
		else
			CAnimations::GetInstance()->Get(ID_ANI_ARROW_BLACK)->Render(startX + i * 8, startY);
	}
}

void CHud::RenderNumber(int number, float x, float y, int maxDigits)
{
	string str = to_string(number);
	if (maxDigits > 0 && str.length() < maxDigits)
		str = string(maxDigits - str.length(), '0') + str;

	for (size_t i = 0; i < str.size(); i++)
	{
		int digit = str[i] - '0';
		int aniId = ID_ANI_NUMBER_0 + digit;
		CAnimations::GetInstance()->Get(aniId)->Render(x + i * HUD_NUMBER_SPACING, y);
	}
}

void CHud::RenderChar(char c, float x, float y)
{
	int aniId = -1;

	switch (c)
	{
	case 'W': aniId = ID_ANI_CHAR_W; break;
	case 'O': aniId = ID_ANI_CHAR_O; break;
	case 'R': aniId = ID_ANI_CHAR_R; break;
	case 'L': aniId = ID_ANI_CHAR_L; break;
	case 'D': aniId = ID_ANI_CHAR_D; break;
	case 'A': aniId = ID_ANI_CHAR_A; break;
	case 'C': aniId = ID_ANI_CHAR_C; break;
	case 'E': aniId = ID_ANI_CHAR_E; break;
	case 'G': aniId = ID_ANI_CHAR_G; break;
	case 'S': aniId = ID_ANI_CHAR_S; break;
	case 'T': aniId = ID_ANI_CHAR_T; break;
	case 'U': aniId = ID_ANI_CHAR_U; break;
	case 'Y': aniId = ID_ANI_CHAR_Y; break;
	}

	if (aniId != -1)
		CAnimations::GetInstance()->Get(aniId)->Render(x, y);
}
