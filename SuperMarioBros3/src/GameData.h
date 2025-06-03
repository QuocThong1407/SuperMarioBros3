#pragma once
#include <windows.h>

class CGameData {
private:
	static CGameData* __instance;

	int life = 4;
	int point = 0;
	int coin = 0;

	ULONGLONG startTime = 0;
	DWORD remainTime = 300; 

public:
	CGameData();

	void Reset(); 

	void SetLife(int l) { life = l; }
	void AddLife(int l) { life += l; }
	int GetLife() const { return life; }

	void AddPoint(int p) { point += p; }
	void SetPoint(int p) { point = p; }
	int GetPoint() const { return point; }

	void AddCoin(int c);
	void SetCoin(int c) { coin = c; }
	int GetCoin() const { return coin; }

	void StartCountDown();
	void CountDownRemainTime();
	DWORD GetRemainTime() const { return remainTime; }

	static CGameData* GetInstance();
};
