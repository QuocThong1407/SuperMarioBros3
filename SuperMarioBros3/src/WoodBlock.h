#pragma once

#include "GameObject.h"

#define ID_ANI_WOOD_BLOCK 21000

#define WOOD_BLOCK_BBOX_WIDTH 16
#define WOOD_BLOCK_BBOX_HEIGHT 16

class CWoodBlock : public CGameObject {
public:
	CWoodBlock(float x, float y) : CGameObject(x, y) {}
	void Render();
	void Update(DWORD dt) {}
	void GetBoundingBox(float& l, float& t, float& r, float& b);
};