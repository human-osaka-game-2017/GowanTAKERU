#ifndef STAGE_1_BOSS_H
#define STAGE_1_BOSS_H

#include<d3dx9.h>

#define BOSS1WIDTH		150.0f
#define BOSS1HEIGHT		178.0f
#define BOSS1MAXSPEED	6.0f

enum BOSS1STATE {
	BOSS1_NON, NORMALSHOT, DUALSHOT,LARIAT, BOSS1STATE_MAX
};

struct Boss1Data {
	D3DXVECTOR2 WolrdPos;
	D3DXVECTOR2 WindowPos;
	BOSS1STATE Boss1State;
	float MovementX, MovementY;
	float a;//, ga;//加速度、重力加速度
	int Hp;
	int saveShotFrmcnt;
	int saveActionCntForNORMALSHOT;
	int saveActionCntForDUALSHOT;
	int saveActionCntForLARIAT;
	int Atk;
	//bool isJumping;
	bool isLeft;
	bool isDead;
	bool isActive;
	bool isExistence;//存在しているかどうか
	bool goNextStage;
};

Boss1Data* GetBoss1Data();

void Boss1Init();

void Boss1Control();

void MoveBoss1();

#endif
