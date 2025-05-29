#include <algorithm>
#include "debug.h"

#include "Mario.h"
#include "Game.h"
#include "PlayScene.h"

#include "Goomba.h"
#include "Coin.h"
#include "Portal.h"
#include "QuestionBrick.h"
#include "SuperMushroom.h"
#include "PiranhaPlant.h"
#include "Bullet.h"
#include "Paragoomba.h"
#include "Koopa.h"
#include "SuperLeaf.h"
#include "Point.h"
#include "YellowBrick.h"
#include "SwitchBlock.h"

#include "Collision.h"

void CMario::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	vy += ay * dt;
	vx += ax * dt;

	if (abs(vx) > abs(maxVx)) vx = maxVx;

	// reset untouchable timer if untouchable time has passed
	if ( GetTickCount64() - untouchable_start > MARIO_UNTOUCHABLE_TIME) 
	{
		untouchable_start = 0;
		untouchable = 0;
	}

	if (isTransforming)
	{
		if (GetTickCount64() - transform_start > MARIO_TRANSFORM_TIME)
			isTransforming = false;

		return; 
	}

	if (dynamic_cast<CKoopa*>(item)) {
		CKoopa* heldKoopa = dynamic_cast<CKoopa*>(item);

		float MARIO_WIDTH = (level > MARIO_LEVEL_SMALL) ? MARIO_BIG_BBOX_WIDTH : MARIO_SMALL_BBOX_WIDTH;
		float heldKoopaX = nx > 0 ? x + MARIO_WIDTH / 2 + 5 : x - MARIO_WIDTH / 2 - 5;
		float heldKoopaY = y - 2;

		heldKoopa->SetPosition(heldKoopaX, heldKoopaY);
	}

	CCollision::GetInstance()->Process(this, dt, coObjects);
	DropItem();
}

void CMario::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
	isOnPlatform = false;
}

void CMario::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (e->ny != 0 && e->obj->IsBlocking())
	{
		vy = 0;
		if (e->ny < 0) isOnPlatform = true;
	}
	else 
	if (e->nx != 0 && e->obj->IsBlocking())
	{
		vx = 0;
	}

	if (dynamic_cast<CGoomba*>(e->obj))
		OnCollisionWithGoomba(e);
	else if (dynamic_cast<CCoin*>(e->obj))
		OnCollisionWithCoin(e);
	else if (dynamic_cast<CPortal*>(e->obj))
		OnCollisionWithPortal(e);
	else if (dynamic_cast<CQuestionBrick*>(e->obj))
		OnCollisionWithQuestionBrick(e);
	else if (dynamic_cast<CPiranhaPlant*>(e->obj))
		OnCollisionWithPiranhaPlant(e);
	else if (dynamic_cast<CSuperMushroom*>(e->obj))
		OnCollisionWithSuperMushroom(e);
	else if (dynamic_cast<CBullet*>(e->obj))
		OnCollisionWithBullet(e);
	else if (dynamic_cast<CParagoomba*>(e->obj))
		OnCollisionWithParagoomba(e);
	else if (dynamic_cast<CKoopa*>(e->obj))
		OnCollisionWithKoopa(e);
	else if (dynamic_cast<CSuperLeaf*>(e->obj))
		OnCollisionWithSuperLeaf(e);
	else if (dynamic_cast<CYellowBrick*>(e->obj))
		OnCollisionWithYellowBrick(e);
	else if (dynamic_cast<CSwitchBlock*>(e->obj))
		OnCollisionWithSwitchBlock(e);
}

void CMario::OnCollisionWithGoomba(LPCOLLISIONEVENT e)
{
	CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj);

	// jump on top >> kill Goomba and deflect a bit 
	if (e->ny < 0)
	{
		if (goomba->GetState() != GOOMBA_STATE_DIE)
		{
			float goombaX, goombaY;
			goomba->GetPosition(goombaX, goombaY);
			LPGAME game = CGame::GetInstance();
			LPPLAYSCENE scene = (LPPLAYSCENE)game->GetCurrentScene();
			CPoint* point = new CPoint(goombaX, goombaY);

			goomba->SetState(GOOMBA_STATE_DIE);
			point->SetState(POINT_STATE_100);
			scene->AddObject(point);


			if (level == MARIO_LEVEL_RACOON)
				vy = -MARIO_JUMP_DEFLECT_SPEED / 2;
			else
				vy = -MARIO_JUMP_DEFLECT_SPEED;
		}
	}
	else // hit by Goomba
	{
		if (untouchable == 0)
		{
			if (goomba->GetState() != GOOMBA_STATE_DIE)
			{
				if (level > MARIO_LEVEL_BIG)
				{
					StartTransform();
					level = MARIO_LEVEL_BIG;
					StartUntouchable();
				}
				else if (level > MARIO_LEVEL_SMALL)
				{
					StartTransform();
					level = MARIO_LEVEL_SMALL;
					StartUntouchable();
				}
				else
				{
					SetState(MARIO_STATE_DIE);
				}
			}
		}
	}
}

void CMario::OnCollisionWithCoin(LPCOLLISIONEVENT e)
{
	e->obj->Delete();
	coin++;
}

void CMario::OnCollisionWithPortal(LPCOLLISIONEVENT e)
{
	CPortal* p = (CPortal*)e->obj;
	CGame::GetInstance()->InitiateSwitchScene(p->GetSceneId());
}

void CMario::OnCollisionWithQuestionBrick(LPCOLLISIONEVENT e) 
{
	CQuestionBrick* questionBrick = dynamic_cast<CQuestionBrick*>(e->obj);
	if (questionBrick->GetIsUnboxed()) return;

	if (e->ny > 0) {
		questionBrick->Unbox();
	}
}

void CMario::OnCollisionWithSuperMushroom(LPCOLLISIONEVENT e) {
	CSuperMushroom* mushroom = dynamic_cast<CSuperMushroom*>(e->obj);

	if (level == MARIO_LEVEL_SMALL)
	{
		StartTransform();
		SetLevel(MARIO_LEVEL_BIG);
	}

	float mushroomX, mushroomY;
	mushroom->GetPosition(mushroomX, mushroomY);
	LPGAME game = CGame::GetInstance();
	LPPLAYSCENE scene = (LPPLAYSCENE)game->GetCurrentScene();
	CPoint* point = new CPoint(mushroomX, mushroomY);

	mushroom->Delete();
	point->SetState(POINT_STATE_1000);
	scene->AddObject(point);
}

void CMario::OnCollisionWithPiranhaPlant(LPCOLLISIONEVENT e) {
	CPiranhaPlant* plant = dynamic_cast<CPiranhaPlant*>(e->obj);

	if (untouchable == 0) {
		if (level > MARIO_LEVEL_BIG)
		{
			StartTransform();
			level = MARIO_LEVEL_BIG;
			StartUntouchable();
		}
		else if (level > MARIO_LEVEL_SMALL)
		{
			StartTransform();
			level = MARIO_LEVEL_SMALL;
			StartUntouchable();
		}
		else
		{
			SetState(MARIO_STATE_DIE);
		}
	}
}

void CMario::OnCollisionWithBullet(LPCOLLISIONEVENT e) {
	CBullet* bullet = dynamic_cast<CBullet*>(e->obj);

	if (untouchable == 0) {
		if (level > MARIO_LEVEL_BIG)
		{
			StartTransform();
			level = MARIO_LEVEL_BIG;
			StartUntouchable();
		}
		else if (level > MARIO_LEVEL_SMALL)
		{
			StartTransform();
			level = MARIO_LEVEL_SMALL;
			StartUntouchable();
		}
		else
		{
			SetState(MARIO_STATE_DIE);
		}
	}

	bullet->Delete();
}

void CMario::OnCollisionWithParagoomba(LPCOLLISIONEVENT e) {
	CParagoomba* paragoomba = dynamic_cast<CParagoomba*>(e->obj);
	float paragoombaX, paragoombaY;
	paragoomba->GetPosition(paragoombaX, paragoombaY);
	LPGAME game = CGame::GetInstance();
	LPPLAYSCENE scene = (LPPLAYSCENE)game->GetCurrentScene();
	CPoint* point = new CPoint(paragoombaX, paragoombaY);

	if (e->ny < 0)
	{
		if (paragoomba->GetState() != PARAGOOMBA_STATE_DIE)
		{
			if (paragoomba->GetState() == PARAGOOMBA_STATE_FLYING)
			{
				paragoomba->SetState(PARAGOOMBA_STATE_WALKING);
				point->SetState(POINT_STATE_200);
				scene->AddObject(point);
			}
			else
			{
				paragoomba->SetState(PARAGOOMBA_STATE_DIE);
				point->SetState(POINT_STATE_100);
				scene->AddObject(point);
			}

			if (level == MARIO_LEVEL_RACOON)
				vy = -MARIO_JUMP_DEFLECT_SPEED / 2;
			else 
				vy = -MARIO_JUMP_DEFLECT_SPEED;
		}
	}
	else
	{
		if (untouchable == 0)
		{
			if (paragoomba->GetState() != PARAGOOMBA_STATE_DIE)
			{
				if (level > MARIO_LEVEL_BIG)
				{
					StartTransform();
					level = MARIO_LEVEL_BIG;
					StartUntouchable();
				}
				else if (level > MARIO_LEVEL_SMALL)
				{
					StartTransform();
					level = MARIO_LEVEL_SMALL;
					StartUntouchable();
				}
				else
				{
					SetState(MARIO_STATE_DIE);
				}
			}
		}
	}
}

void CMario::OnCollisionWithKoopa(LPCOLLISIONEVENT e) {
	CKoopa* koopa = dynamic_cast<CKoopa*>(e->obj);
	float koopaX, koopaY;
	koopa->GetPosition(koopaX, koopaY);
	LPGAME game = CGame::GetInstance();
	LPPLAYSCENE scene = (LPPLAYSCENE)game->GetCurrentScene();
	CPoint* point = new CPoint(koopaX, koopaY);

	if (!koopa || koopa->IsDeleted()) return;

	if (e->ny < 0)
	{
		if (koopa->GetType() == 3) {
			koopa->SetType(2);
			koopa->SetState(KOOPA_STATE_WALKING);
			point->SetState(POINT_STATE_100);
			scene->AddObject(point);
			vy = -MARIO_JUMP_DEFLECT_SPEED;
		}
		else {
			if (!koopa->IsDefend())
			{
				koopa->SetState(KOOPA_STATE_DEFEND);
				point->SetState(POINT_STATE_100);
				scene->AddObject(point);
				vy = -MARIO_JUMP_DEFLECT_SPEED;
			}
			else if (koopa->IsKicked())
			{
				koopa->SetState(KOOPA_STATE_DEFEND);
				vy = -MARIO_JUMP_DEFLECT_SPEED;
			}
			else if (koopa->IsDefend() && !koopa->IsKicked())
			{
				koopa->BeKicked(nx > 0 ? KOOPA_KICK_SPEED : -KOOPA_KICK_SPEED);
			}
		}
	}
	else if (e->nx != 0)
	{
		if (koopa->IsDefend() && !koopa->IsKicked() && !koopa->IsHeld())
		{
			if (CGame::GetInstance()->IsKeyDown(DIK_A) && item == nullptr)
			{
				item = dynamic_cast<CKoopa*>(koopa);
				koopa->SetState(KOOPA_STATE_HELD);
			}
			else
			{
				koopa->BeKicked(nx > 0 ? KOOPA_KICK_SPEED : -KOOPA_KICK_SPEED);
				point->SetState(POINT_STATE_200);
				scene->AddObject(point);
			}
		}
		else if (koopa->IsKicked() || !koopa->IsDefend())
		{
			if (untouchable == 0)
			{
				if (level > MARIO_LEVEL_BIG)
				{
					StartTransform();
					level = MARIO_LEVEL_BIG;
					StartUntouchable();
				}
				else if (level > MARIO_LEVEL_SMALL)
				{
					StartTransform();
					level = MARIO_LEVEL_SMALL;
					StartUntouchable();
				}
				else
				{
					SetState(MARIO_STATE_DIE);
				}
			}
		}
	}
}

void CMario::OnCollisionWithYellowBrick(LPCOLLISIONEVENT e) {
	CYellowBrick* brick = dynamic_cast<CYellowBrick*>(e->obj);
	if (e->ny > 0)
		brick->Break();
}

void CMario::OnCollisionWithSwitchBlock(LPCOLLISIONEVENT e) {
	CSwitchBlock* sw = dynamic_cast<CSwitchBlock*>(e->obj);
	if (!sw || sw->isActivated) return;

	if (e->ny < 0)
	{
		sw->isActivated = true;
		sw->SetState(SWITCH_BLOCK_STATE_DISABLED);

		LPPLAYSCENE scene = (LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene();
		vector<LPGAMEOBJECT>* objects = scene->GetObjects();

		float sx, sy;
		sw->GetPosition(sx, sy);

		for (LPGAMEOBJECT obj : *objects)
		{
			CYellowBrick* brick = dynamic_cast<CYellowBrick*>(obj);
			if (!brick || brick->IsDeleted()) continue;

			float bx, by;
			brick->GetPosition(bx, by);

			if (abs(bx - sx) <= 100 && abs(by - sy) <= 100)
			{
				CCoin* coin = new CCoin(bx, by);
				coin->SetState(COIN_STATE_IDLE);
				scene->AddObject(coin);

				brick->Break();
			}
		}
	}
}

void CMario::OnCollisionWithSuperLeaf(LPCOLLISIONEVENT e) {
	CSuperLeaf* leaf = dynamic_cast<CSuperLeaf*>(e->obj);
	float leafX, leafY;
	leaf->GetPosition(leafX, leafY);
	LPGAME game = CGame::GetInstance();
	LPPLAYSCENE scene = (LPPLAYSCENE)game->GetCurrentScene();
	CPoint* point = new CPoint(leafX, leafY);
	SetLevel(MARIO_LEVEL_RACOON);

	leaf->Delete();
	point->SetState(POINT_STATE_1000);
	scene->AddObject(point);
}

void CMario::DropItem() {
	if (item == nullptr) return;
	CKoopa* heldKoopa = dynamic_cast<CKoopa*>(item);
	float kx, ky;
	heldKoopa->GetPosition(kx, ky);

	if (!CGame::GetInstance()->IsKeyDown(DIK_A)) {
		if (dynamic_cast<CKoopa*>(item)) {
			heldKoopa->SetState(KOOPA_STATE_KICKED);
			heldKoopa->BeKicked(nx > 0 ? KOOPA_KICK_SPEED : -KOOPA_KICK_SPEED);
		}
		item = nullptr;
	}
	else if (heldKoopa->GetState() == KOOPA_STATE_WALKING) {
		float MARIO_WIDTH = (level > MARIO_LEVEL_SMALL) ? MARIO_BIG_BBOX_WIDTH : MARIO_SMALL_BBOX_WIDTH;
		float space = nx > 0 ? MARIO_WIDTH / 2 + 2 : -MARIO_WIDTH / 2 - 2;
		heldKoopa->SetPosition(kx + space, ky - 6);
		item = nullptr;
	}
}

//
// Get animation ID for small Mario
//
int CMario::GetAniIdSmall()
{
	int aniId = -1;
	if (!isOnPlatform)
	{
		if (abs(ax) == MARIO_ACCEL_RUN_X)
		{
			if (nx >= 0)
				aniId = ID_ANI_MARIO_SMALL_JUMP_RUN_RIGHT;
			else
				aniId = ID_ANI_MARIO_SMALL_JUMP_RUN_LEFT;
		}
		else
		{
			if (nx >= 0)
				aniId = ID_ANI_MARIO_SMALL_JUMP_WALK_RIGHT;
			else
				aniId = ID_ANI_MARIO_SMALL_JUMP_WALK_LEFT;
		}
	}
	else
		if (isSitting)
		{
			if (nx > 0)
				aniId = ID_ANI_MARIO_SIT_RIGHT;
			else
				aniId = ID_ANI_MARIO_SIT_LEFT;
		}
		else
			if (vx == 0)
			{
				if (nx > 0) aniId = ID_ANI_MARIO_SMALL_IDLE_RIGHT;
				else aniId = ID_ANI_MARIO_SMALL_IDLE_LEFT;
			}
			else if (vx > 0)
			{
				if (ax < 0)
					aniId = ID_ANI_MARIO_SMALL_BRACE_RIGHT;
				else if (ax == MARIO_ACCEL_RUN_X)
					aniId = ID_ANI_MARIO_SMALL_RUNNING_RIGHT;
				else if (ax == MARIO_ACCEL_WALK_X)
					aniId = ID_ANI_MARIO_SMALL_WALKING_RIGHT;
			}
			else // vx < 0
			{
				if (ax > 0)
					aniId = ID_ANI_MARIO_SMALL_BRACE_LEFT;
				else if (ax == -MARIO_ACCEL_RUN_X)
					aniId = ID_ANI_MARIO_SMALL_RUNNING_LEFT;
				else if (ax == -MARIO_ACCEL_WALK_X)
					aniId = ID_ANI_MARIO_SMALL_WALKING_LEFT;
			}

	if (aniId == -1) aniId = ID_ANI_MARIO_SMALL_IDLE_RIGHT;

	return aniId;
}


//
// Get animdation ID for big Mario
//
int CMario::GetAniIdBig()
{
	int aniId = -1;
	if (!isOnPlatform)
	{
		if (abs(ax) == MARIO_ACCEL_RUN_X)
		{
			if (nx >= 0)
				aniId = ID_ANI_MARIO_JUMP_RUN_RIGHT;
			else
				aniId = ID_ANI_MARIO_JUMP_RUN_LEFT;
		}
		else
		{
			if (nx >= 0)
				aniId = ID_ANI_MARIO_JUMP_WALK_RIGHT;
			else
				aniId = ID_ANI_MARIO_JUMP_WALK_LEFT;
		}
	}
	else
		if (isSitting)
		{
			if (nx > 0)
				aniId = ID_ANI_MARIO_SIT_RIGHT;
			else
				aniId = ID_ANI_MARIO_SIT_LEFT;
		}
		else
			if (vx == 0)
			{
				if (nx > 0) aniId = ID_ANI_MARIO_IDLE_RIGHT;
				else aniId = ID_ANI_MARIO_IDLE_LEFT;
			}
			else if (vx > 0)
			{
				if (ax < 0)
					aniId = ID_ANI_MARIO_BRACE_RIGHT;
				else if (ax == MARIO_ACCEL_RUN_X)
					aniId = ID_ANI_MARIO_RUNNING_RIGHT;
				else if (ax == MARIO_ACCEL_WALK_X)
					aniId = ID_ANI_MARIO_WALKING_RIGHT;
			}
			else // vx < 0
			{
				if (ax > 0)
					aniId = ID_ANI_MARIO_BRACE_LEFT;
				else if (ax == -MARIO_ACCEL_RUN_X)
					aniId = ID_ANI_MARIO_RUNNING_LEFT;
				else if (ax == -MARIO_ACCEL_WALK_X)
					aniId = ID_ANI_MARIO_WALKING_LEFT;
			}

	if (aniId == -1) aniId = ID_ANI_MARIO_IDLE_RIGHT;

	return aniId;
}

int CMario::GetAniIdRacoon()
{
	int aniId = -1;

	if (!isOnPlatform)
	{
		if (isFlying)
		{
			aniId = (nx > 0) ? ID_ANI_MARIO_FLY_RIGHT : ID_ANI_MARIO_FLY_LEFT;
		}
		else if (abs(ax) == MARIO_ACCEL_RUN_X)
		{
			aniId = (nx > 0) ? ID_ANI_MARIO_FLY_JUMP_RIGHT : ID_ANI_MARIO_FLY_JUMP_LEFT;
		}
		else
		{
			aniId = (nx > 0) ? ID_ANI_MARIO_FLY_JUMP_RIGHT : ID_ANI_MARIO_FLY_JUMP_LEFT;
		}
	}
	else {
		if (isSitting)
		{
			aniId = (nx > 0) ? ID_ANI_MARIO_FLY_SIT_RIGHT : ID_ANI_MARIO_FLY_SIT_LEFT;
		}
		else {
			if (vx == 0)
			{
				aniId = (nx > 0) ? ID_ANI_MARIO_FLY_IDLE_RIGHT : ID_ANI_MARIO_FLY_IDLE_LEFT;
			}
			else if (vx > 0)
			{
				if (ax < 0)
					aniId = ID_ANI_MARIO_FLY_BRACE_RIGHT;
				else if (ax == MARIO_ACCEL_RUN_X)
					aniId = ID_ANI_MARIO_FLY_RUNNING_RIGHT;
				else if (ax == MARIO_ACCEL_WALK_X)
					aniId = ID_ANI_MARIO_FLY_WALKING_RIGHT;
			}
			else
			{
				if (ax > 0)
					aniId = ID_ANI_MARIO_FLY_BRACE_LEFT;
				else if (ax == -MARIO_ACCEL_RUN_X)
					aniId = ID_ANI_MARIO_FLY_RUNNING_LEFT;
				else if (ax == -MARIO_ACCEL_WALK_X)
					aniId = ID_ANI_MARIO_FLY_WALKING_LEFT;
			}
		}
	}
	
	if (aniId == -1) aniId = ID_ANI_MARIO_FLY_IDLE_RIGHT;
	return aniId;
}

void CMario::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	int aniId = -1;

	if (state == MARIO_STATE_DIE)
		aniId = ID_ANI_MARIO_DIE;
	else if (isTransforming)
		aniId = (nx > 0) ? ID_ANI_MARIO_TRANSFORMING_RIGHT : ID_ANI_MARIO_TRANSFORMING_LEFT;
	else if (level == MARIO_LEVEL_BIG)
		aniId = GetAniIdBig();
	else if (level == MARIO_LEVEL_SMALL)
		aniId = GetAniIdSmall();
	else if (level == MARIO_LEVEL_RACOON)
		aniId = GetAniIdRacoon();

	animations->Get(aniId)->Render(x, y);

	//RenderBoundingBox();
	
	DebugOutTitle(L"Coins: %d", coin);
}

void CMario::SetState(int state)
{
	// DIE is the end state, cannot be changed! 
	if (this->state == MARIO_STATE_DIE) return; 

	if (level < MARIO_LEVEL_RACOON)
		ay = MARIO_GRAVITY;

	switch (state)
	{
	case MARIO_STATE_RUNNING_RIGHT:
		if (isSitting) break;
		maxVx = MARIO_RUNNING_SPEED;
		ax = MARIO_ACCEL_RUN_X;
		nx = 1;
		break;
	case MARIO_STATE_RUNNING_LEFT:
		if (isSitting) break;
		maxVx = -MARIO_RUNNING_SPEED;
		ax = -MARIO_ACCEL_RUN_X;
		nx = -1;
		break;
	case MARIO_STATE_WALKING_RIGHT:
		if (isSitting) break;
		maxVx = MARIO_WALKING_SPEED;
		ax = MARIO_ACCEL_WALK_X;
		nx = 1;
		break;
	case MARIO_STATE_WALKING_LEFT:
		if (isSitting) break;
		maxVx = -MARIO_WALKING_SPEED;
		ax = -MARIO_ACCEL_WALK_X;
		nx = -1;
		break;
	case MARIO_STATE_JUMP:
		if (isSitting) break;
		ay = MARIO_GRAVITY;
		if (isOnPlatform)
		{
			if (abs(this->vx) == MARIO_RUNNING_SPEED)
				vy = -MARIO_JUMP_RUN_SPEED_Y;
			else
				vy = -MARIO_JUMP_SPEED_Y;
		}
		break;

	case MARIO_STATE_RELEASE_JUMP:
		if (vy < 0) vy += MARIO_JUMP_SPEED_Y / 2;
		break;

	case MARIO_STATE_FLY_RIGHT:
		isFlying = true;
		nx = 1;
		vx = MARIO_WALKING_SPEED;
		vy = -MARIO_FLY_SPEED_Y;
		break;

	case MARIO_STATE_FLY_LEFT:
		isFlying = true;
		nx = -1;
		vx = -MARIO_WALKING_SPEED;
		vy = -MARIO_FLY_SPEED_Y;
		break;

	case MARIO_STATE_RELEASE_FLY:
		isFlying = false;
		ay = MARIO_RACOON_GRAVITY;
		if (vy < 0) vy += MARIO_FLY_SPEED_Y / 2;
		break;

	case MARIO_STATE_SIT:
		if (isOnPlatform && level != MARIO_LEVEL_SMALL)
		{
			state = MARIO_STATE_IDLE;
			isSitting = true;
			vx = 0; vy = 0.0f;
			y +=MARIO_SIT_HEIGHT_ADJUST;
		}
		break;

	case MARIO_STATE_SIT_RELEASE:
		if (isSitting)
		{
			isSitting = false;
			state = MARIO_STATE_IDLE;
			if (level == MARIO_LEVEL_RACOON)
				y -= MARIO_RACOON_SIT_HEIGHT_ADJUST;
			else
				y -= MARIO_SIT_HEIGHT_ADJUST;
		}
		break;

	case MARIO_STATE_IDLE:
		ax = 0.0f;
		vx = 0.0f;
		break;

	case MARIO_STATE_DIE:
		vy = -MARIO_JUMP_DEFLECT_SPEED;
		vx = 0;
		ax = 0;
		break;
	}

	CGameObject::SetState(state);
}

void CMario::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	if (level==MARIO_LEVEL_BIG)
	{
		if (isSitting)
		{
			left = x - MARIO_BIG_SITTING_BBOX_WIDTH / 2;
			top = y - MARIO_BIG_SITTING_BBOX_HEIGHT / 2;
			right = left + MARIO_BIG_SITTING_BBOX_WIDTH;
			bottom = top + MARIO_BIG_SITTING_BBOX_HEIGHT;
		}
		else 
		{
			left = x - MARIO_BIG_BBOX_WIDTH/2;
			top = y - MARIO_BIG_BBOX_HEIGHT/2;
			right = left + MARIO_BIG_BBOX_WIDTH;
			bottom = top + MARIO_BIG_BBOX_HEIGHT;
		}
	}
	else if (level == MARIO_LEVEL_SMALL)
	{
		left = x - MARIO_SMALL_BBOX_WIDTH/2;
		top = y - MARIO_SMALL_BBOX_HEIGHT/2;
		right = left + MARIO_SMALL_BBOX_WIDTH;
		bottom = top + MARIO_SMALL_BBOX_HEIGHT;
	}
	else {
		if (isSitting)
		{
			left = x - MARIO_RACOON_SITTING_BBOX_WIDTH / 2;
			top = y - MARIO_RACOON_SITTING_BBOX_HEIGHT / 2;
			right = left + MARIO_RACOON_SITTING_BBOX_WIDTH;
			bottom = top + MARIO_RACOON_SITTING_BBOX_HEIGHT;
		}
		else
		{
			left = x - MARIO_RACOON_BBOX_WIDTH / 2;
			top = y - MARIO_RACOON_BBOX_HEIGHT / 2;
			right = left + MARIO_RACOON_BBOX_WIDTH;
			bottom = top + MARIO_RACOON_BBOX_HEIGHT;
		}
	}
}

void CMario::SetLevel(int l)
{
	// Adjust position to avoid falling off platform
	if (this->level == MARIO_LEVEL_SMALL)
	{
		y -= (MARIO_BIG_BBOX_HEIGHT - MARIO_SMALL_BBOX_HEIGHT) / 2;
	}

	level = l;
}

void CMario::StartTransform()
{
	isTransforming = true;
	transform_start = GetTickCount64();
	vx = vy = ax = ay = 0; 
}