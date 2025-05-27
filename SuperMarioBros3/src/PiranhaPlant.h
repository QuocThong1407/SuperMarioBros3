#pragma once
#include "GameObject.h"

#define PIRANHA_PLANT_BBOX_WIDTH 16
#define PIRANHA_PLANT_BBOX_HEIGHT 32

#define PIRANHA_PLANT_SPEED_Y 0.025f
#define PIRANHA_PLANT_STATE_UP 100
#define PIRANHA_PLANT_STATE_DOWN 200

#define PIRANHA_PLANT_TIME_UP 3000
#define PIRANHA_PLANT_TIME_DOWN 3000
#define PIRANHA_PLANT_RELOAD_TIME 500

#define ID_ANI_RED_PIRANHA_PLANT_LEFT_DOWN 15001
#define ID_ANI_RED_PIRANHA_PLANT_LEFT_UP 15002

#define ID_ANI_RED_PIRANHA_PLANT_RIGHT_DOWN 15003
#define ID_ANI_RED_PIRANHA_PLANT_RIGHT_UP 15004

#define ID_ANI_GREEN_PIRANHA_PLANT_LEFT_DOWN 15005
#define ID_ANI_GREEN_PIRANHA_PLANT_LEFT_UP 15006

#define ID_ANI_GREEN_PIRANHA_PLANT_RIGHT_DOWN 15007
#define ID_ANI_GREEN_PIRANHA_PLANT_RIGHT_UP 15008

#define ID_ANI_GREEN_HANGING_PIRANHA_PLANT 15009


class CPiranhaPlant : public CGameObject
{
protected:
	float ay;
	float minY;
	float maxY;
	int type;
	bool isUpping = false;
	bool isDowning = false;
	bool hasShot = false;

	ULONGLONG timeUpStart = 0;
	ULONGLONG timeDownStart = 0;
	ULONGLONG timeShootStart = 0;

public:
	CPiranhaPlant(float x, float y, int type) : CGameObject(x, y) {
		ay = 0;
		minY = y - PIRANHA_PLANT_BBOX_HEIGHT;
		maxY = y;
		this->type = type;
		SetState(PIRANHA_PLANT_STATE_DOWN);
	}

	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = nullptr) override;
	void Render() override;
	void GetBoundingBox(float& l, float& t, float& r, float& b) override;
	void SetState(int state) override;
	void Shoot();
};