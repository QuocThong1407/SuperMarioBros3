#include "GameObject.h"

#define POINT_BBOX_WIDTH 16
#define POINT_BBOX_HEIGHT 8

#define POINT_SPEED_Y 0.05f
#define POINT_GRAVITY 0.03f

#define TIME_OUT 300

#define POINT_STATE_100	100
#define POINT_STATE_200 200
#define POINT_STATE_400 300
#define POINT_STATE_800 400
#define POINT_STATE_1000 500
#define POINT_STATE_2000 600
#define POINT_STATE_4000 700
#define POINT_STATE_8000 800

#define ID_ANI_POINT_100 100
#define ID_ANI_POINT_200 101
#define ID_ANI_POINT_400 102
#define ID_ANI_POINT_800 103
#define ID_ANI_POINT_1000 104
#define ID_ANI_POINT_2000 105
#define ID_ANI_POINT_4000 106
#define ID_ANI_POINT_8000 107

class CPoint : public CGameObject {
	float ax, ay;
	DWORD timeOut;
public:
	CPoint(float x, float y) : CGameObject(x, y) {
		ax = 0;
		ay = 0;
		vy = -POINT_SPEED_Y;
		timeOut = GetTickCount64();
	}

	void GetBoundingBox(float& l, float& t, float& r, float& b) override;
	void Render() override;
	void Update(DWORD dt, std::vector<LPGAMEOBJECT>* coObjects) override;
	void OnNoCollision(DWORD dt) override;
	void SetState(int state) override;
};

