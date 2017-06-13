#ifndef ENEMYCONTROL_H
#define ENEMYCONTROL_H

#include<d3dx9.h>

struct Enemy {
	D3DXVECTOR2 WindowPos;
	D3DXVECTOR2 WorldPos;
	int dmg;
	bool bedead;
	bool beActive;
	bool beLeft;
};


#endif
