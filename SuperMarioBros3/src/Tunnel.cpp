#include "Tunnel.h"
#include "PlayScene.h"

void CTunnel::Render() {
	int aniId = ID_ANI_TUNNEL_LONG;
	if (tunnel_type == 1) aniId = ID_ANI_TUNNEL_SHORT;
	else if (tunnel_type == 2) aniId = ID_ANI_TUNNEL_LONG;
	else if (tunnel_type == 3) aniId = ID_ANI_TUNNEL_BODY;
	else if (tunnel_type == 4) aniId = ID_ANI_BLACK_TUNNEL_BODY;
	else if (tunnel_type == 5) aniId = ID_ANI_BLACK_TUNNEL_HEAD;

	CAnimations* animations = CAnimations::GetInstance();
	animations->Get(aniId)->Render(x, y);
}

void CTunnel::GetBoundingBox(float& l, float& t, float& r, float& b) {
	if (tunnel_type == 1) {
		l = x - TUNNEL_BBOX_WIDTH / 2;
		t = y - TUNNEL_SHORT_BBOX_HEIGHT / 2;
		r = l + TUNNEL_BBOX_WIDTH;
		b = t + TUNNEL_SHORT_BBOX_HEIGHT;
	}
	else if (tunnel_type == 2) {
		l = x - TUNNEL_BBOX_WIDTH / 2;
		t = y - TUNNEL_LONG_BBOX_HEIGHT / 2;
		r = l + TUNNEL_BBOX_WIDTH;
		b = t + TUNNEL_LONG_BBOX_HEIGHT;
	}
	else if (tunnel_type == 3) {
		l = x - TUNNEL_BODY_BBOX_WIDTH / 2;
		t = y - TUNNEL_BODY_BBOX_HEIGHT / 2;
		r = l + TUNNEL_BODY_BBOX_WIDTH;
		b = t + TUNNEL_BODY_BBOX_HEIGHT;
	}
	else if (tunnel_type == 4) {
		l = x - TUNNEL_BBOX_WIDTH / 2;
		t = y - BLACK_TUNNEL_BBOX_HEIGHT / 2;
		r = l + TUNNEL_BBOX_WIDTH;
		b = t + BLACK_TUNNEL_BBOX_HEIGHT;
	}
	else if (tunnel_type == 5) {
		l = x - TUNNEL_BBOX_WIDTH / 2;
		t = y - BLACK_TUNNEL_BBOX_HEIGHT / 2;
		r = l + TUNNEL_BBOX_WIDTH;
		b = t + BLACK_TUNNEL_BBOX_HEIGHT;
	}
}