#pragma once 
#include "GameObject.h"

#define TUNNEL_BBOX_WIDTH 32
#define TUNNEL_BODY_BBOX_WIDTH 30
#define TUNNEL_BODY_BBOX_HEIGHT 32
#define TUNNEL_SHORT_BBOX_HEIGHT 32
#define TUNNEL_LONG_BBOX_HEIGHT 48

#define TUNNEL_STATE_SHORT 1
#define TUNNEL_STATE_LONG 2
#define TUNNEL_STATE_BODY 3

#define ID_ANI_TUNNEL_SHORT 13001
#define ID_ANI_TUNNEL_LONG 13002
#define ID_ANI_TUNNEL_BODY 13003

class CTunnel : public CGameObject {
public:
	CTunnel(float x, float y, int tunnel_type) : CGameObject(x, y) {
		state = tunnel_type;
	}
	int IsCollidable() override { return 0; }
	int IsBlocking() override { return 1; }

	void GetBoundingBox(float& l, float& t, float& r, float& b) override;
	void Render() override;
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) override {};
};