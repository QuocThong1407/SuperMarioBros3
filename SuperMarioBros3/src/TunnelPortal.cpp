#include "TunnelPortal.h"
#include "GameData.h"
#include "Game.h"

#include "TunnelPortal.h"
#include "GameData.h"
#include "PlayScene.h"
#include "Game.h"
#include "Mario.h"

void CTunnelPortal::OnMarioEnter() {
	CGameData::GetInstance()->SetRespawn(exitX, exitY);
	CGameData::GetInstance()->SetCurrentSceneId(sceneId);
	CGameData::GetInstance()->EnterTunnel();
}