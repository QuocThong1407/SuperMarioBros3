#include "Coin.h"
#include "Point.h"
#include "PlayScene.h"

void CCoin::Render()
{
	if (isDeleted) return;

	int aniId = -1;

	if (state == COIN_STATE_JUMP)
		aniId = ID_ANI_COIN;

	if (aniId == -1)
		aniId = ID_ANI_STATIC_COIN;

	CAnimations* animations = CAnimations::GetInstance();
	animations->Get(aniId)->Render(x, y);

	//RenderBoundingBox();
}

void CCoin::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - COIN_BBOX_WIDTH / 2;
	t = y - COIN_BBOX_HEIGHT / 2;
	r = l + COIN_BBOX_WIDTH;
	b = t + COIN_BBOX_HEIGHT;
}

void CCoin::OnNoCollision(DWORD dt) {
	if (isDeleted) return;

	x += vx * dt;
	y += vy * dt;
}

void CCoin::Update(DWORD dt, std::vector<LPGAMEOBJECT>* coObjects)
{
	if (!isCollected && y < collect_y)
	{
		isCollected = true;
	}

	if (isCollected && y >= collect_y)
	{
		LPGAME game = CGame::GetInstance();
		LPPLAYSCENE scene = (LPPLAYSCENE)game->GetCurrentScene();
		isDeleted = true;
		CPoint* point = new CPoint(x, y);
		point->SetState(POINT_STATE_100);
		scene->AddObject(point);

		return;
	}

	vx += ax * dt;
	vy += ay * dt;

	CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CCoin::SetState(int state)
{
	this->state = state;

	switch (state)
	{
	case COIN_STATE_IDLE:
		vx = vy = 0;
		ay = 0;
		break;
	case COIN_STATE_JUMP:
		vy = -COIN_JUMP_SPEED_Y;
		ay = COIN_GRAVITY;

		collect_y = y - ((vy * vy) / (2 * COIN_GRAVITY)) * 1 / 3;
		break;
	}
}