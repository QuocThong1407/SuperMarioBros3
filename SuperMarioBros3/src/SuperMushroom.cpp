#include "SuperMushroom.h"

void CSuperMushroom::OnNoCollision(DWORD dt) {
	if (isDeleted) return;

	x += vx * dt;
	y += vy * dt;
}

void CSuperMushroom::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (isDeleted) return;

	vx += ax * dt;
	vy += ay * dt;

	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CSuperMushroom::Render()
{
	if (isDeleted) return;

	CAnimations::GetInstance()->Get(ID_ANI_SUPER_MUSHROOM)->Render(x, y);
}

void CSuperMushroom::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	if (isDeleted) return;

	l = x - SUPER_MUSHROOM_BBOX_WIDTH / 2;
	t = y - SUPER_MUSHROOM_BBOX_HEIGHT / 2;
	r = l + SUPER_MUSHROOM_BBOX_WIDTH;
	b = t + SUPER_MUSHROOM_BBOX_HEIGHT;
}