#include "Bullet.h"
#include "Game.h"
#include "PlayScene.h"
#include "Mario.h"

CBullet::CBullet(float x, float y) : CGameObject(x, y)
{
	SetState(BULLET_STATE_SHOOTING);
}

void CBullet::SetDirection(float angleRad)
{
	vx = BULLET_SPEED * cos(angleRad);
	vy = -BULLET_SPEED * sin(angleRad); 
}

void CBullet::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
}

void CBullet::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (isDeleted) return;

	vx += ax * dt;
	vy += ay * dt;

	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CBullet::Render()
{
	if (isDeleted) return;

	CAnimations::GetInstance()->Get(ID_ANI_BULLET)->Render(x, y);
}

void CBullet::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - BULLET_BBOX_WIDTH / 2;
	t = y - BULLET_BBOX_HEIGHT / 2;
	r = l + BULLET_BBOX_WIDTH;
	b = t + BULLET_BBOX_HEIGHT;
}

void CBullet::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
	case BULLET_STATE_SHOOTING:
		ax = 0;
		ay = BULLET_GRAVITY;
		break;
	}
}

