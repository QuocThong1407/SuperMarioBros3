#include "Tunnel.h"
#include "PlayScene.h"

void CTunnel::Render() {
	int aniId = ID_ANI_TUNNEL_LONG;
	if (state == TUNNEL_STATE_SHORT) aniId = ID_ANI_TUNNEL_SHORT;
	else if (state == TUNNEL_STATE_LONG) aniId = ID_ANI_TUNNEL_LONG;
	else if (state == TUNNEL_STATE_BODY) aniId = ID_ANI_TUNNEL_BODY;

	CAnimations* animations = CAnimations::GetInstance();
	animations->Get(aniId)->Render(x, y);
}

void CTunnel::GetBoundingBox(float& l, float& t, float& r, float& b) {
	if (state == TUNNEL_STATE_SHORT) {
		l = x - TUNNEL_BBOX_WIDTH / 2;
		t = y - TUNNEL_SHORT_BBOX_HEIGHT / 2;
		r = l + TUNNEL_BBOX_WIDTH;
		b = t + TUNNEL_SHORT_BBOX_HEIGHT;
	}
	else if (state == TUNNEL_STATE_LONG) {
		l = x - TUNNEL_BBOX_WIDTH / 2;
		t = y - TUNNEL_LONG_BBOX_HEIGHT / 2;
		r = l + TUNNEL_BBOX_WIDTH;
		b = t + TUNNEL_LONG_BBOX_HEIGHT;
	}
	else if (state == TUNNEL_STATE_BODY) {
		l = x - TUNNEL_BODY_BBOX_WIDTH / 2;
		t = y - TUNNEL_BODY_BBOX_HEIGHT / 2;
		r = l + TUNNEL_BODY_BBOX_WIDTH;
		b = t + TUNNEL_BODY_BBOX_HEIGHT;
	}
}