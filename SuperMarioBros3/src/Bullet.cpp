#include "Bullet.h"
#include "Game.h"
#include "PlayScene.h"
#include "Mario.h"

CBullet::CBullet(float x, float y, float targetX, float targetY)
	: CGameObject(x, y)
{
	this->targetX = targetX;
	this->targetY = targetY;

	ax = 0;
	ay = BULLET_GRAVITY;

	SetState(BULLET_STATE_SHOOTING);
}

void CBullet::CalculateVelocity(float targetX, float targetY)
{
	float dx = targetX - x;
	float dy = targetY - y;

	if (dx == 0) dx = 0.0001f;

	float angle = atan(fabs(dy) / fabs(dx));

	vx = BULLET_SPEED * cos(angle);
	vy = BULLET_SPEED * sin(angle);

	if (dx < 0) vx = -vx;
	if (dy < 0) vy = -vy;
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
		CalculateVelocity(targetX, targetY);
		break;
	}
}

