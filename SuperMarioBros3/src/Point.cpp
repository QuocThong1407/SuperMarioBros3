#include "Point.h"

void CPoint::Render() {
	int aniId = -1;

	switch (state) {
	case POINT_STATE_100:
		aniId = ID_ANI_POINT_100;
		break;
	case POINT_STATE_200:
		aniId = ID_ANI_POINT_200;
		break;
	case POINT_STATE_400:
		aniId = ID_ANI_POINT_400;
		break;
	case POINT_STATE_800:
		aniId = ID_ANI_POINT_800;
		break;
	case POINT_STATE_1000:
		aniId = ID_ANI_POINT_1000;
		break;
	case POINT_STATE_2000:
		aniId = ID_ANI_POINT_2000;
		break;
	case POINT_STATE_4000:
		aniId = ID_ANI_POINT_4000;
		break;
	case POINT_STATE_8000:
		aniId = ID_ANI_POINT_8000;
		break;
	}

	if (aniId == -1) aniId = ID_ANI_POINT_100;
	CAnimations* animations = CAnimations::GetInstance();
	animations->Get(aniId)->Render(x, y);
}

void CPoint::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - POINT_BBOX_WIDTH / 2;
	t = y - POINT_BBOX_HEIGHT / 2;
	r = l + POINT_BBOX_WIDTH;
	b = t + POINT_BBOX_HEIGHT;
}

void CPoint::OnNoCollision(DWORD dt) {
	if (isDeleted) return;

	x += vx * dt;
	y += vy * dt;
}

void CPoint::Update(DWORD dt, std::vector<LPGAMEOBJECT>* coObjects) {
	vx += ax * dt;
	vy += ay * dt;

	if (GetTickCount64() - timeOut >= TIME_OUT) {
		this->Delete();
	}

	CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CPoint::SetState(int state) {
	CGameObject::SetState(state);
}