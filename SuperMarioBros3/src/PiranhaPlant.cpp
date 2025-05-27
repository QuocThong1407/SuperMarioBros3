#include "PiranhaPlant.h"
#include "PlayScene.h"
#include "Mario.h"
#include "Bullet.h"

void CPiranhaPlant::GetBoundingBox(float& l, float& t, float& r, float& b) {
	l = x - PIRANHA_PLANT_BBOX_WIDTH / 2;
	t = y - PIRANHA_PLANT_BBOX_HEIGHT / 2;
	r = l + PIRANHA_PLANT_BBOX_WIDTH;
	b = t + PIRANHA_PLANT_BBOX_HEIGHT;
}

void CPiranhaPlant::Render()
{
	if (isDeleted) return;

	int aniId = -1;

	LPPLAYSCENE scene = (LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene();
	CMario* mario = (CMario*)scene->GetPlayer();

	if (!mario) return;

	float mx, my;
	mario->GetPosition(mx, my);

	bool isMarioOnTop = my < y;
	bool isMarioOnLeft = mx < x;

	if (type == 3) {
		aniId = ID_ANI_GREEN_HANGING_PIRANHA_PLANT;
	}
	else if (isMarioOnLeft && isMarioOnTop) {
		if (type == 1) 
			aniId = ID_ANI_RED_PIRANHA_PLANT_LEFT_UP;
		else if (type == 2) 
			aniId = ID_ANI_GREEN_PIRANHA_PLANT_LEFT_UP;

	}
	else if (isMarioOnLeft && !isMarioOnTop) {
		if (type == 1)
			aniId = ID_ANI_RED_PIRANHA_PLANT_LEFT_DOWN;
		else if (type == 2)
			aniId = ID_ANI_GREEN_PIRANHA_PLANT_LEFT_DOWN;
	}
	else if (!isMarioOnLeft && isMarioOnTop) {
		if (type == 1) 
			aniId = ID_ANI_RED_PIRANHA_PLANT_RIGHT_UP;
		else if (type == 2)
			aniId = ID_ANI_GREEN_PIRANHA_PLANT_RIGHT_UP;
	}
	else if (!isMarioOnLeft && !isMarioOnTop) {
		if (type == 1)
			aniId = ID_ANI_RED_PIRANHA_PLANT_RIGHT_DOWN;
		else if (type == 2)
			aniId = ID_ANI_GREEN_PIRANHA_PLANT_RIGHT_DOWN;
	}
		

	CAnimations::GetInstance()->Get(aniId)->Render(x, y);
}

void CPiranhaPlant::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
	case PIRANHA_PLANT_STATE_UP:
		isUpping = true;
		isDowning = false;
		hasShot = false;
		vy = -PIRANHA_PLANT_SPEED_Y;
		timeUpStart = GetTickCount64();
		timeShootStart = GetTickCount64();
		break;
	case PIRANHA_PLANT_STATE_DOWN:
		isUpping = false;
		isDowning = true;
		hasShot = false;
		vy = PIRANHA_PLANT_SPEED_Y;
		timeDownStart = GetTickCount64();
		break;
	}
}

void CPiranhaPlant::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (isUpping)
	{
		if (y <= minY)
		{
			y = minY;
			vy = 0;

			if (type == 1 || type == 2) {
				if (!hasShot && GetTickCount64() - timeShootStart >= PIRANHA_PLANT_RELOAD_TIME)
				{
					Shoot();
					hasShot = true;
				}
			}

			if (GetTickCount64() - timeUpStart >= PIRANHA_PLANT_TIME_UP)
				SetState(PIRANHA_PLANT_STATE_DOWN);
		}
	}
	else if (isDowning)
	{
		if (y >= maxY)
		{
			y = maxY;
			vy = 0;
			if (GetTickCount64() - timeDownStart >= PIRANHA_PLANT_TIME_DOWN)
				SetState(PIRANHA_PLANT_STATE_UP);
		}
	}

	y += vy * dt;
}

void CPiranhaPlant::Shoot() {
	LPPLAYSCENE scene = (LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene();
	CMario* mario = (CMario*)scene->GetPlayer();
	if (!mario) return;

	float mx, my;
	mario->GetPosition(mx, my);

	bool isMarioOnTop = my < y;
	bool isMarioOnLeft = mx < x;

	float angleRad = 0;

	if (isMarioOnLeft && isMarioOnTop)
		angleRad = D3DXToRadian(135); 
	else if (isMarioOnLeft && !isMarioOnTop)
		angleRad = D3DXToRadian(195); 
	else if (!isMarioOnLeft && isMarioOnTop)
		angleRad = D3DXToRadian(45); 
	else if (!isMarioOnLeft && !isMarioOnTop)
		angleRad = D3DXToRadian(345); 

	CBullet* bullet = new CBullet(x, y - 4);
	bullet->SetDirection(angleRad);

	scene->AddObject(bullet);
}