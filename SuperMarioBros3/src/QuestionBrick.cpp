
#include "QuestionBrick.h"
#include "PlayScene.h"

void CQuestionBrick::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - QUESTION_BRICK_BBOX_WIDTH / 2;
	t = y - QUESTION_BRICK_BBOX_HEIGHT / 2;
	r = l + QUESTION_BRICK_BBOX_WIDTH;
	b = t + QUESTION_BRICK_BBOX_HEIGHT;
}

void CQuestionBrick::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (!isUnboxed) return;

	vx += ax * dt;
	vy += ay * dt;

	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CQuestionBrick::OnNoCollision(DWORD dt)
{
	if (!isUnboxed) return;

	x += vx * dt;
	y += vy * dt;

	if (y > originalY)
	{
		Disable();
	}
}

void CQuestionBrick::Render()
{
	int aniId = ID_ANI_QUESTION_BRICK;
	if (state == QUESTION_BRICK_STATE_DISABLED)
		aniId = ID_ANI_QUESTION_BRICK_DISABLED;

	CAnimations* animations = CAnimations::GetInstance();
	animations->Get(aniId)->Render(x, y);
}

void CQuestionBrick::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
	case QUESTION_BRICK_STATE_UNBOXING:
	{
		isUnboxed = true;
		originalY = y;
		vy = -QUESTION_BRICK_BOUNCE_SPEED;
		ay = QUESTION_BRICK_GRAVITY;

		LPGAME game = CGame::GetInstance();

		switch (item)
		{
		case QUESTION_BRICK_ITEM_COIN:
		{
			break;
		}
		default:
			break;
		}
		break;
	}
	case QUESTION_BRICK_STATE_DISABLED:
	{
		vy = 0;
		y = originalY;
		break;
	}
	}
}


