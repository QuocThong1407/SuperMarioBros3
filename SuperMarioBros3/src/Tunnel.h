#pragma once 
#include "GameObject.h"

#define TUNNEL_BBOX_WIDTH 32
#define TUNNEL_BODY_BBOX_WIDTH 30
#define TUNNEL_BODY_BBOX_HEIGHT 32
#define TUNNEL_SHORT_BBOX_HEIGHT 32
#define TUNNEL_LONG_BBOX_HEIGHT 48
#define BLACK_TUNNEL_BBOX_HEIGHT 16


#define ID_ANI_TUNNEL_SHORT 13001
#define ID_ANI_TUNNEL_LONG 13002
#define ID_ANI_TUNNEL_BODY 13003
#define ID_ANI_BLACK_TUNNEL_BODY 13004
#define ID_ANI_BLACK_TUNNEL_HEAD 13005

class CTunnel : public CGameObject {
	int tunnel_type;
public:
	CTunnel(float x, float y, int tunnel_type) : CGameObject(x, y) {
		this->tunnel_type = tunnel_type;
	}
	int IsCollidable() override { return 0; }
	int IsBlocking() override { return 1; }

	void GetBoundingBox(float& l, float& t, float& r, float& b) override;
	void Render() override;
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) override {};
};