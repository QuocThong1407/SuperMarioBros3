#include "GameObject.h"

#define SWITCH_BLOCK_BBOX_WITDH 16
#define SWITCH_BLOCK_BBOX_HEIGHT 16
#define SWITCH_BLOCK_DISABLED_BBOX_HEIGHT 6

#define SWITCH_BLOCK_SPEED -0.05f

#define SWITCH_BLOCK_STATE_DEFAULT 100
#define SWITCH_BLOCK_STATE_RISING 200
#define SWITCH_BLOCK_STATE_DISABLED 300

#define ID_ANI_SWITCH_BLOCK_DEFAULT 24000
#define ID_ANI_SWITCH_BLOCK_DISABLED 24001

class CSwitchBlock : public CGameObject {
    float maxY;
public:
    bool isActivated;

    CSwitchBlock(float x, float y) : CGameObject(x, y) {
        maxY = y - 16;
        isActivated = false;
        SetState(SWITCH_BLOCK_STATE_RISING);
    }

    void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) override;
    void Render() override;
    void GetBoundingBox(float& l, float& t, float& r, float& b) override;

    void SetState(int state) override;
};