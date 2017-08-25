#ifndef BOSS3CONTROL_H
#define BOSS3CONTROL_H

#include<d3dx9.h>

//シールドも含めた大きさ
#define BOSS3OBJWIDTH		240.0f
#define BOSS3OBJHEIGHT		256.0f

//ボス３本体の大きさ
#define BOSS3WIDTH			174.0f
#define BOSS3HEIGHT			220.0f
#define BOSS3MAXSPEED		0.5f

#define BOSS3SHIELD1WIDTH	22.0f
#define BOSS3SHIELD1HEIGHT	200.0f
#define BOSS3SHIELD2WIDTH	24.0f
#define BOSS3SHIELD2HEIGHT	180.0f
#define BOSS3SHIELD3WIDTH	24.0f
#define BOSS3SHIELD3HEIGHT	120.0f

enum BOSS3STATE {
	BOSS3_NON,SHOTNORMALBULLET,SHOTPOWERBULLET,
};

struct Boss3Data {
	D3DXVECTOR2 WolrdPos;
	D3DXVECTOR2 WindowPos;
	BOSS3STATE Boss3State;
	float MovementX, MovementY;
	int Hp;
	int Atk;
	bool isLeft;
	bool isDead;
	bool isActive;
	bool isExistence;//存在しているかどうか
};

Boss3Data* GetBoss3Data();

void Boss3Init();

void Boss3Control();

void MoveBoss3();

#endif