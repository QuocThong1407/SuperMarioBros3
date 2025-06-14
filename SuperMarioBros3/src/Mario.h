#pragma once
#include "GameObject.h"

#include "Animation.h"
#include "Animations.h"

#include "debug.h"

#define MARIO_WALKING_SPEED		0.1f
#define MARIO_RUNNING_SPEED		0.2f
#define MARIO_ENTER_TUNNEL_SPEED   0.05f

#define MARIO_ACCEL_WALK_X	0.0005f
#define MARIO_ACCEL_RUN_X	0.0007f

#define MARIO_JUMP_SPEED_Y		0.5f
#define MARIO_JUMP_RUN_SPEED_Y	0.6f

#define MARIO_FLY_SPEED_Y 0.2f
#define MARIO_FLY_RUN_SPEED_Y 0.6f

#define MARIO_GRAVITY			0.002f
#define MARIO_RACOON_GRAVITY	0.0005f

#define MARIO_JUMP_DEFLECT_SPEED  0.4f

#define MARIO_TAIL_ATTACK_TIME 300
#define MARIO_TAIL_RADIUS 25

#define MARIO_STATE_DIE				-10
#define MARIO_STATE_IDLE			0
#define MARIO_STATE_WALKING_RIGHT	100
#define MARIO_STATE_WALKING_LEFT	200

#define MARIO_STATE_JUMP			300
#define MARIO_STATE_RELEASE_JUMP    301

#define MARIO_STATE_RUNNING_RIGHT	400
#define MARIO_STATE_RUNNING_LEFT	500

#define MARIO_STATE_SIT				600
#define MARIO_STATE_SIT_RELEASE		601

#define MARIO_STATE_FLY_RIGHT	700
#define MARIO_STATE_FLY_LEFT	701
#define MARIO_STATE_RELEASE_FLY 702

#define MARIO_STATE_ENTER_TUNNEL    800


#pragma region ANIMATION_ID

#define ID_ANI_MARIO_IDLE_RIGHT 400
#define ID_ANI_MARIO_IDLE_LEFT 401

#define ID_ANI_MARIO_WALKING_RIGHT 500
#define ID_ANI_MARIO_WALKING_LEFT 501

#define ID_ANI_MARIO_RUNNING_RIGHT 600
#define ID_ANI_MARIO_RUNNING_LEFT 601

#define ID_ANI_MARIO_JUMP_WALK_RIGHT 700
#define ID_ANI_MARIO_JUMP_WALK_LEFT 701

#define ID_ANI_MARIO_JUMP_RUN_RIGHT 800
#define ID_ANI_MARIO_JUMP_RUN_LEFT 801

#define ID_ANI_MARIO_SIT_RIGHT 900
#define ID_ANI_MARIO_SIT_LEFT 901

#define ID_ANI_MARIO_BRACE_RIGHT 1000
#define ID_ANI_MARIO_BRACE_LEFT 1001

#define ID_ANI_MARIO_DIE 999

// SMALL MARIO
#define ID_ANI_MARIO_SMALL_IDLE_RIGHT 1100
#define ID_ANI_MARIO_SMALL_IDLE_LEFT 1102

#define ID_ANI_MARIO_SMALL_WALKING_RIGHT 1200
#define ID_ANI_MARIO_SMALL_WALKING_LEFT 1201

#define ID_ANI_MARIO_SMALL_RUNNING_RIGHT 1300
#define ID_ANI_MARIO_SMALL_RUNNING_LEFT 1301

#define ID_ANI_MARIO_SMALL_BRACE_RIGHT 1400
#define ID_ANI_MARIO_SMALL_BRACE_LEFT 1401

#define ID_ANI_MARIO_SMALL_JUMP_WALK_RIGHT 1500
#define ID_ANI_MARIO_SMALL_JUMP_WALK_LEFT 1501

#define ID_ANI_MARIO_SMALL_JUMP_RUN_RIGHT 1600
#define ID_ANI_MARIO_SMALL_JUMP_RUN_LEFT 1601

// RACOON MARIO
#define ID_ANI_MARIO_FLY_IDLE_RIGHT 1700
#define ID_ANI_MARIO_FLY_IDLE_LEFT 1701

#define ID_ANI_MARIO_FLY_WALKING_RIGHT 1800
#define ID_ANI_MARIO_FLY_WALKING_LEFT 1801

#define ID_ANI_MARIO_FLY_RUNNING_RIGHT 1900
#define ID_ANI_MARIO_FLY_RUNNING_LEFT 1901

#define ID_ANI_MARIO_FLY_BRACE_RIGHT 2000
#define ID_ANI_MARIO_FLY_BRACE_LEFT 2001

#define ID_ANI_MARIO_FLY_SIT_RIGHT 2100
#define ID_ANI_MARIO_FLY_SIT_LEFT 2101

#define ID_ANI_MARIO_FLY_RIGHT 2200
#define ID_ANI_MARIO_FLY_LEFT 2201

#define ID_ANI_MARIO_FLY_JUMP_RIGHT 2300
#define ID_ANI_MARIO_FLY_JUMP_LEFT 2301

#define ID_ANI_MARIO_ENTER_TUNNEL 2400

#define ID_ANI_MARIO_WAGGING_TAIL_RIGHT 2500
#define ID_ANI_MARIO_WAGGING_TAIL_LEFT 2501

#define ID_ANI_MARIO_TRANSFORMING_RIGHT 3000
#define ID_ANI_MARIO_TRANSFORMING_LEFT 3001
#define MARIO_TRANSFORM_TIME 700

#pragma endregion

#define GROUND_Y 160.0f


#define	MARIO_LEVEL_SMALL	1
#define	MARIO_LEVEL_BIG		2
#define MARIO_LEVEL_RACOON	3

#define MARIO_BIG_BBOX_WIDTH  14
#define MARIO_BIG_BBOX_HEIGHT 24
#define MARIO_BIG_SITTING_BBOX_WIDTH  14
#define MARIO_BIG_SITTING_BBOX_HEIGHT 16

#define MARIO_SIT_HEIGHT_ADJUST ((MARIO_BIG_BBOX_HEIGHT-MARIO_BIG_SITTING_BBOX_HEIGHT)/2)
#define MARIO_RACOON_SIT_HEIGHT_ADJUST ((MARIO_RACOON_BBOX_HEIGHT - MARIO_RACOON_SITTING_BBOX_HEIGHT)/2)

#define MARIO_SMALL_BBOX_WIDTH  13
#define MARIO_SMALL_BBOX_HEIGHT 12

#define MARIO_RACOON_BBOX_WIDTH 14
#define MARIO_RACOON_BBOX_HEIGHT 28

#define MARIO_RACOON_SITTING_BBOX_WIDTH 14
#define MARIO_RACOON_SITTING_BBOX_HEIGHT 18

#define MARIO_UNTOUCHABLE_TIME 2500

#define MARIO_POWER_MAX 6
#define MARIO_POWER_INCREMENT_TIME 200
#define MARIO_POWER_DECREMENT_TIME 300

class CMario : public CGameObject
{
	BOOLEAN isSitting;
	BOOLEAN isFlying;
	BOOLEAN isEnteringTunnel;
	bool isTailAttacking;

	ULONGLONG tail_attack_start = 0;

	int enterDirection = 0; 
	float enterTargetY = 0;

	bool isTransforming;
	ULONGLONG transform_start;
	float maxVx;
	float ax;				// acceleration on x 
	float ay;				// acceleration on y 

	CGameObject* item;

	int level; 
	int untouchable; 
	ULONGLONG untouchable_start;
	BOOLEAN isOnPlatform;
	int coin; 

	int power;
	ULONGLONG lastPowerUpdate = 0;

	void OnCollisionWithGoomba(LPCOLLISIONEVENT e);
	void OnCollisionWithCoin(LPCOLLISIONEVENT e);
	void OnCollisionWithPortal(LPCOLLISIONEVENT e);
	void OnCollisionWithQuestionBrick(LPCOLLISIONEVENT e);
	void OnCollisionWithSuperMushroom(LPCOLLISIONEVENT e);
	void OnCollisionWithPiranhaPlant(LPCOLLISIONEVENT e);
	void OnCollisionWithBullet(LPCOLLISIONEVENT e);
	void OnCollisionWithParagoomba(LPCOLLISIONEVENT e);
	void OnCollisionWithKoopa(LPCOLLISIONEVENT e);
	void OnCollisionWithSuperLeaf(LPCOLLISIONEVENT e);
	void OnCollisionWithYellowBrick(LPCOLLISIONEVENT e);
	void OnCollisionWithSwitchBlock(LPCOLLISIONEVENT e);
	void OnCollisionWithTunnelPortal(LPCOLLISIONEVENT e);
	void OnCollisionWithRandomCard(LPCOLLISIONEVENT e);

	int GetAniIdBig();
	int GetAniIdSmall();
	int GetAniIdRacoon();

public:
	CMario(float x, float y) : CGameObject(x, y)
	{
		isSitting = false;
		isFlying = false;
		isEnteringTunnel = false;
		isTailAttacking = false;

		maxVx = 0.0f;
		ax = 0.0f;
		ay = MARIO_GRAVITY; 

		item = nullptr;

		power = 0;

		level = MARIO_LEVEL_SMALL;
		untouchable = 0;
		untouchable_start = -1;
		isOnPlatform = false;

		isTransforming = false;
		transform_start = 0;

		coin = 0;
	}
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render();
	void SetState(int state);

	int IsCollidable()
	{ 
		return (state != MARIO_STATE_DIE); 
	}

	int IsBlocking() { return (state != MARIO_STATE_DIE && untouchable==0); }

	void OnNoCollision(DWORD dt);
	void OnCollisionWith(LPCOLLISIONEVENT e);

	int GetLevel() { return level; }
	void SetLevel(int l);
	void StartUntouchable() { untouchable = 1; untouchable_start = GetTickCount64(); }
	bool GetFlying() { return isFlying; }
	int GetPower() { return power; }
	bool IsFullPower() { return power == MARIO_POWER_MAX; }

	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	bool IsOnPlatform() { return isOnPlatform; }

	void StartTransform();
	bool IsTransforming() { return isTransforming; }

	void StartEnterTunnel();
	bool IsEnteringTunnel() { return isEnteringTunnel; }

	void StartTailAttack();
	int IsTailAttacking() { return isTailAttacking; }

	void DropItem();
};