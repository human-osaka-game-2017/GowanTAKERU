#ifndef BOSS3CONTROL_H
#define BOSS3CONTROL_H

#include<d3dx9.h>

enum BOSS3STATE {
	
};

struct Boss3Data {
	D3DXVECTOR2 WolrdPos;
	D3DXVECTOR2 WindowPos;
	BOSS3STATE Boss3State;
	float MovementX, MovementY;
	float a;//, ga;//加速度、重力加速度
	int Hp;
	int Atk;
	//bool isJumping;
	bool isLeft;
	bool isDead;
	bool isActive;
	bool isExistence;//存在しているかどうか
};

Boss3Data* GetBoss3Data();

#endif