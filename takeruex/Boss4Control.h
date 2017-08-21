#ifndef BOSS4CONTROL_H
#define BOSS4CONTROL_H

#include<d3dx9.h>

enum BOSS4STATE {

};

struct Boss4Data {
	D3DXVECTOR2 WolrdPos;
	D3DXVECTOR2 WindowPos;
	BOSS4STATE Boss3State;
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

Boss4Data* GetBoss4Data();

#endif
