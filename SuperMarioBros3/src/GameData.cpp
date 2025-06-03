#include "GameData.h"
#include "debug.h"

CGameData* CGameData::__instance = nullptr;

CGameData::CGameData() {
	Reset();
}

void CGameData::Reset() {
	life = 4;
	point = 0;
	coin = 0;
	startTime = GetTickCount64();
	remainTime = 300;
}

void CGameData::AddCoin(int c) {
	coin += c;
	if (coin >= 100) {
		coin -= 100;
		life += 1;
	}
}

void CGameData::StartCountDown() {
	startTime = GetTickCount64();
}

void CGameData::CountDownRemainTime() {
	if (startTime == 0)
		startTime = GetTickCount64();
	DWORD now = GetTickCount64();
	DWORD elapsed = (now - startTime) / 1000;

	DebugOut(L"[HUD] Now: %d - Start: %d = %d => Remain: %d\n", now, startTime, elapsed, remainTime);

	if (elapsed < 300)
		remainTime = 300 - elapsed;
	else
		remainTime = 0;
}

CGameData* CGameData::GetInstance() {
	if (!__instance)
		__instance = new CGameData();
	return __instance;
}
