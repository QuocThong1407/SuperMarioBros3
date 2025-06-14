#include <iostream>
#include <fstream>
#include "AssetIDs.h"

#include "PlayScene.h"
#include "Utils.h"
#include "Textures.h"
#include "Sprites.h"
#include "Portal.h"
#include "Coin.h"
#include "Platform.h"
#include "QuestionBrick.h"
#include "Tunnel.h"
#include "SuperMushroom.h"
#include "PiranhaPlant.h"
#include "Paragoomba.h"
#include "Koopa.h"
#include "SuperLeaf.h"
#include "Point.h"
#include "CloudBlock.h"
#include "WoodBlock.h"
#include "YellowBrick.h"
#include "BrokenBrick.h"
#include "SwitchBlock.h"
#include "Hud.h"
#include "GameData.h"
#include "TunnelPortal.h"
#include "RandomCard.h"

#include "SampleKeyEventHandler.h"

using namespace std;

D3DXCOLOR backgroundColor = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

CPlayScene::CPlayScene(int id, LPCWSTR filePath):
	CScene(id, filePath)
{
	player = NULL;
	key_handler = new CSampleKeyHandler(this);
}


#define SCENE_SECTION_UNKNOWN -1
#define SCENE_SECTION_SETTINGS	0
#define SCENE_SECTION_ASSETS	1
#define SCENE_SECTION_OBJECTS	2

#define ASSETS_SECTION_UNKNOWN -1
#define ASSETS_SECTION_SPRITES 1
#define ASSETS_SECTION_ANIMATIONS 2

#define MAX_SCENE_LINE 1024

void CPlayScene::_ParseSection_SPRITES(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 6) return; // skip invalid lines

	int ID = atoi(tokens[0].c_str());
	int l = atoi(tokens[1].c_str());
	int t = atoi(tokens[2].c_str());
	int r = atoi(tokens[3].c_str());
	int b = atoi(tokens[4].c_str());
	int texID = atoi(tokens[5].c_str());

	LPTEXTURE tex = CTextures::GetInstance()->Get(texID);
	if (tex == NULL)
	{
		DebugOut(L"[ERROR] Texture ID %d not found!\n", texID);
		return; 
	}

	CSprites::GetInstance()->Add(ID, l, t, r, b, tex);
}

void CPlayScene::_ParseSection_ASSETS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 1) return;

	wstring path = ToWSTR(tokens[0]);
	
	LoadAssets(path.c_str());
}

void CPlayScene::_ParseSection_ANIMATIONS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 3) return; // skip invalid lines - an animation must at least has 1 frame and 1 frame time

	//DebugOut(L"--> %s\n",ToWSTR(line).c_str());

	LPANIMATION ani = new CAnimation();

	int ani_id = atoi(tokens[0].c_str());
	for (int i = 1; i < tokens.size(); i += 2)	// why i+=2 ?  sprite_id | frame_time  
	{
		int sprite_id = atoi(tokens[i].c_str());
		int frame_time = atoi(tokens[i+1].c_str());
		ani->Add(sprite_id, frame_time);
	}

	CAnimations::GetInstance()->Add(ani_id, ani);
}

void CPlayScene::_ParseSection_SETTINGS(string line)
{
	vector<string> tokens = split(line);
	if (tokens[0] == "background" && tokens.size() >= 5)
	{
		int r = atoi(tokens[1].c_str());
		int g = atoi(tokens[2].c_str());
		int b = atoi(tokens[3].c_str());
		int a = atoi(tokens[4].c_str());

		backgroundColor = D3DXCOLOR(
			r / 255.0f,
			g / 255.0f,
			b / 255.0f,
			a / 255.0f
		);
	}
}

/*
	Parse a line in section [OBJECTS] 
*/
void CPlayScene::_ParseSection_OBJECTS(string line)
{
	vector<string> tokens = split(line);

	// skip invalid lines - an object set must have at least id, x, y
	if (tokens.size() < 2) return;

	int object_type = atoi(tokens[0].c_str());
	float x = (float)atof(tokens[1].c_str());
	float y = (float)atof(tokens[2].c_str());

	CGameObject *obj = NULL;

	switch (object_type)
	{
	case OBJECT_TYPE_MARIO:
		if (player!=NULL) 
		{
			DebugOut(L"[ERROR] MARIO object was created before!\n");
			return;
		}
		obj = new CMario(x,y); 
		player = (CMario*)obj;  

		DebugOut(L"[INFO] Player object has been created!\n");
		break;
	case OBJECT_TYPE_GOOMBA: obj = new CGoomba(x,y); break;
	case OBJECT_TYPE_BRICK: obj = new CBrick(x,y); break;
	case OBJECT_TYPE_COIN: obj = new CCoin(x, y); break;

	case OBJECT_TYPE_PLATFORM:
	{
		
		float cell_width = (float)atof(tokens[3].c_str());
		float cell_height = (float)atof(tokens[4].c_str());
		int length = atoi(tokens[5].c_str());
		int sprite_begin = atoi(tokens[6].c_str());
		int sprite_middle = atoi(tokens[7].c_str());
		int sprite_end = atoi(tokens[8].c_str());
		int isBlocking = atoi(tokens[9].c_str());
		int isBlockingAllDirection = 0;

		if (tokens.size() > 10)
			isBlockingAllDirection = atoi(tokens[10].c_str());

		obj = new CPlatform(
			x, y,
			cell_width, cell_height, length,
			sprite_begin, sprite_middle, sprite_end,
			isBlocking, isBlockingAllDirection
		);

		break;
	}

	case OBJECT_TYPE_QUESTION_BRICK: 
	{
		int item = QUESTION_BRICK_ITEM_COIN;
		if (tokens.size() > 3)
			item = atoi(tokens[3].c_str());
		obj = new CQuestionBrick(x, y, item);
		break;
	}

	case OBJECT_TYPE_TUNNEL:
	{
		int tunnel_type = 1;
		if (tokens.size() > 3)
			tunnel_type = atoi(tokens[3].c_str());
		obj = new CTunnel(x, y, tunnel_type);
		break;
	}

	case OBJECT_TYPE_SUPER_MUSHROOM:
	{
		obj = new CSuperMushroom(x, y);
		break;
	}

	case OBJECT_TYPE_PIRANHA_PLANT:
	{
		int piranha_type = 1;
		if (tokens.size() > 3)
			piranha_type = atoi(tokens[3].c_str());

		obj = new CPiranhaPlant(x, y, piranha_type);
		break;
	}

	case OBJECT_TYPE_PARAGOOMBA:
	{
		obj = new CParagoomba(x, y);
		break;
	}

	case OBJECT_TYPE_KOOPA:
	{
		int koopaType = 1;

		if (tokens.size() > 3)
			koopaType = atoi(tokens[3].c_str());

		obj = new CKoopa(x, y, koopaType);
		break;
	}

	case OBJECT_TYPE_SUPER_LEAF:
	{
		obj = new CSuperLeaf(x, y);
		break;
	}

	case OBJECT_TYPE_POINT:
	{
		obj = new CPoint(x, y);
		break;
	}

	case OBJECT_TYPE_CLOUD_BLOCK:
	{
		obj = new CCloudBlock(x, y);
		break;
	}

	case OBJECT_TYPE_WOOD_BLOCK:
	{
		obj = new CWoodBlock(x, y);
		break;
	}

	case OBJECT_TYPE_YELLOW_BRICK:
	{
		int type = 1;

		if (tokens.size() > 3)
			type = atoi(tokens[3].c_str());

		obj = new CYellowBrick(x, y, type);
		break;
	}

	case OBJECT_TYPE_BROKEN_BRICK:
	{
		obj = new CBrokenBrick(x, y, 0, 0);
		break;
	}

	case OBJECT_TYPE_SWITCH_BLOCK:
	{
		obj = new CSwitchBlock(x, y);
		break;
	}

	case OBJECT_TYPE_HUD:
	{
		obj = new CHud(x, y);
		hud = dynamic_cast<CHud*>(obj);
		break;
	}

	case OBJECT_TYPE_RANDOM_CARD:
	{
		obj = new CRandomCard(x, y);
		break;
	}

	case OBJECT_TYPE_TUNNEL_PORTAL:
	{
		float x = stof(tokens[1]);
		float y = stof(tokens[2]);
		int tunnel_type = stoi(tokens[3]);
		int sceneId = stoi(tokens[4]);
		int direction = stoi(tokens[5]);
		float exitX = stof(tokens[6]);
		float exitY = stof(tokens[7]);

		CTunnelPortal* portal = new CTunnelPortal(x, y, tunnel_type, sceneId, direction, exitX, exitY);
		objects.push_back(portal);
		return;
	}

	case OBJECT_TYPE_PORTAL:
	{
		float r = (float)atof(tokens[3].c_str());
		float b = (float)atof(tokens[4].c_str());
		int scene_id = atoi(tokens[5].c_str());
		obj = new CPortal(x, y, r, b, scene_id);
	}
	break;


	default:
		DebugOut(L"[ERROR] Invalid object type: %d\n", object_type);
		return;
	}

	// General object setup
	obj->SetPosition(x, y);


	objects.push_back(obj);
}

void CPlayScene::LoadAssets(LPCWSTR assetFile)
{
	DebugOut(L"[INFO] Start loading assets from : %s \n", assetFile);

	ifstream f;
	f.open(assetFile);

	int section = ASSETS_SECTION_UNKNOWN;

	char str[MAX_SCENE_LINE];
	while (f.getline(str, MAX_SCENE_LINE))
	{
		string line(str);

		if (line[0] == '#') continue;	// skip comment lines	

		if (line == "[SPRITES]") { section = ASSETS_SECTION_SPRITES; continue; };
		if (line == "[ANIMATIONS]") { section = ASSETS_SECTION_ANIMATIONS; continue; };
		if (line[0] == '[') { section = SCENE_SECTION_UNKNOWN; continue; }

		//
		// data section
		//
		switch (section)
		{
		case ASSETS_SECTION_SPRITES: _ParseSection_SPRITES(line); break;
		case ASSETS_SECTION_ANIMATIONS: _ParseSection_ANIMATIONS(line); break;
		}
	}

	f.close();

	DebugOut(L"[INFO] Done loading assets from %s\n", assetFile);
}

void CPlayScene::Load()
{
	DebugOut(L"[INFO] Start loading scene from : %s \n", sceneFilePath);

	ifstream f;
	f.open(sceneFilePath);

	// current resource section flag
	int section = SCENE_SECTION_UNKNOWN;					

	char str[MAX_SCENE_LINE];
	while (f.getline(str, MAX_SCENE_LINE))
	{
		string line(str);

		if (line[0] == '#') continue;	// skip comment lines	
		if (line == "[SETTINGS]") { section = SCENE_SECTION_SETTINGS; continue; }
		if (line == "[ASSETS]") { section = SCENE_SECTION_ASSETS; continue; };
		if (line == "[OBJECTS]") { section = SCENE_SECTION_OBJECTS; continue; };
		if (line[0] == '[') { section = SCENE_SECTION_UNKNOWN; continue; }	

		//
		// data section
		//
		switch (section)
		{
		case SCENE_SECTION_SETTINGS: _ParseSection_SETTINGS(line); break;
			case SCENE_SECTION_ASSETS: _ParseSection_ASSETS(line); break;
			case SCENE_SECTION_OBJECTS: _ParseSection_OBJECTS(line); break;
		}
	}

	f.close();

	if (player && CGameData::GetInstance()->IsEnterTunnel() && (CGameData::GetInstance()->IsExitHiddenMap() || CGameData::GetInstance()->IsEntryHiddenMap()))
	{
		CMario* mario = dynamic_cast<CMario*>(player);
		if (mario)
		{
			CGameData* data = CGameData::GetInstance();
			mario->SetPosition(data->GetRespawnX(), data->GetRespawnY());

			mario->SetLevel(data->GetSavedMarioLevel());

			data->SetLife(data->GetSavedLife());
			data->SetPoint(data->GetSavedPoint());
			data->SetCoin(data->GetSavedCoin());
		}

		CGameData::GetInstance()->OutTunnel();
	}

	CGameData::GetInstance()->StartCountDown();
	DebugOut(L"[INFO] Done loading scene  %s\n", sceneFilePath);
}

void CPlayScene::UpdateHud(DWORD dt)
{
	CGame* game = CGame::GetInstance();

	float camX, camY;
	game->GetCamPos(camX, camY);

	float hudX = camX + game->GetBackBufferWidth() / 2;
	float hudY = camY + game->GetBackBufferHeight() / 2 + 101;

	hud->SetPosition(hudX, hudY);
	hud->Update(dt);
}

void CPlayScene::Update(DWORD dt)
{
	if (player == nullptr) return;

	float mx, my;
	player->GetPosition(mx, my);

	CGame* game = CGame::GetInstance();
	CMario* mario = (CMario*)player;

	int screenW = game->GetBackBufferWidth();
	int screenH = game->GetBackBufferHeight();

	float cx = (mx < 2654) ? mx - screenW / 2 : 2654 - screenW / 2;

	if (cx < 0) cx = 0;

	float cy;

	if (mario->GetLevel() == MARIO_LEVEL_RACOON) {
		if (mario->IsEnteringTunnel() == false && CGameData::GetInstance()->GetCurrentSceneId() == ID_HIDDEN_MAP)
			cy = 0;
		else if (mario->IsEnteringTunnel() && CGameData::GetInstance()->GetCurrentSceneId() != ID_HIDDEN_MAP)
			cy = 0;
		else {
			if (my > 78)
				cy = 0;
			else
				cy = (my >= -113) ? my - screenH / 2 + 42 : -113 - screenH / 2 + 42;
		}
	}
	else
		cy = 0;


	game->SetCamPos(cx, cy);

	vector<LPGAMEOBJECT> coObjects;
	for (size_t i = 0; i < objects.size(); i++)
	{
		LPGAMEOBJECT obj = objects[i];

		if (obj == player) continue;

		float ox, oy;
		obj->GetPosition(ox, oy);

		bool alwaysActive =
			dynamic_cast<CPlatform*>(obj) != nullptr ||
			dynamic_cast<CTunnel*>(obj) != nullptr;

		if (ox >= cx - 32 && ox <= cx + screenW + 32 || alwaysActive)
		{
			obj->SetActive(true);
			coObjects.push_back(obj);
		}
		else
		{
			obj->SetActive(false);
		}
	}

	bool isTransforming = mario->IsTransforming();

	for (size_t i = 0; i < objects.size(); i++)
	{
		if (objects[i] == player || isTransforming == false)
		{
			if (objects[i]->IsActive())
				objects[i]->Update(dt, &coObjects);
		}
	}

	player->Update(dt, &coObjects);
	UpdateHud(dt);

	PurgeDeletedObjects();
}

void CPlayScene::Render()
{
	for (int i = 0; i < objects.size(); i++)
		objects[i]->Render();
}

/*
*	Clear all objects from this scene
*/
void CPlayScene::Clear()
{
	vector<LPGAMEOBJECT>::iterator it;
	for (it = objects.begin(); it != objects.end(); it++)
	{
		delete (*it);
	}
	objects.clear();
}

/*
	Unload scene

	TODO: Beside objects, we need to clean up sprites, animations and textures as well 

*/
void CPlayScene::Unload()
{
	for (int i = 0; i < objects.size(); i++)
		delete objects[i];

	objects.clear();
	player = NULL;

	DebugOut(L"[INFO] Scene %d unloaded! \n", id);
}

bool CPlayScene::IsGameObjectDeleted(const LPGAMEOBJECT& o) { return o == NULL; }

void CPlayScene::PurgeDeletedObjects()
{
	vector<LPGAMEOBJECT>::iterator it;
	for (it = objects.begin(); it != objects.end(); it++)
	{
		LPGAMEOBJECT o = *it;
		if (o->IsDeleted())
		{
			delete o;
			*it = NULL;
		}
	}

	// NOTE: remove_if will swap all deleted items to the end of the vector
	// then simply trim the vector, this is much more efficient than deleting individual items
	objects.erase(
		std::remove_if(objects.begin(), objects.end(), CPlayScene::IsGameObjectDeleted),
		objects.end());
}

void CPlayScene::AddObject(LPGAMEOBJECT obj) 
{
	objects.push_back(obj);
}