#pragma once
#include "Tunnel.h"

#define TUNNEL_PORTAL_DIRECTION_UP 1
#define TUNNEL_PORTAL_DIRECTION_DOWN -1

class CTunnelPortal : public CTunnel
{
protected:
    int sceneId;      
    int direction;     
    float exitX, exitY; 

public:
    CTunnelPortal(float x, float y, int tunnel_type, int sceneId, int direction, float exitX, float exitY)
        : CTunnel(x, y, tunnel_type)
    {
        this->sceneId = sceneId;
        this->direction = direction;
        this->exitX = exitX;
        this->exitY = exitY;
    }

    int IsCollidable() override { return 0; }
    int IsBlocking() override { return 1; }

    int GetSceneId() const { return sceneId; }
    int GetDirection() const { return direction; }

    void OnMarioEnter();
};
