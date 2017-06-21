#ifndef PLAYERCONTROL_H
#define PLAYERCONTROL_H

#include<d3dx9.h>

#define PLAYERMOVESPEED 5.0f
#define PLAYERJUMPPOWER -5.0f

struct Player {
	D3DXVECTOR2 WindowPos;
	D3DXVECTOR2 WorldPos;
	float JumpPower;
	bool Jumping;
	int Hp;
	bool bePunchUP;
	bool bePunchDOWN;
	bool beInvincible;
	bool beLeft;
	bool beActive;
};

//playerのデータ使いたい人はつかってね
Player* GetplayerData();

D3DXVECTOR2* GetBasePoint();

void PlayerControl();
void PlayerInit();

#endif
