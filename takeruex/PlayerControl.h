#ifndef PLAYERCONTROL_H
#define PLAYERCONTROL_H

#include<d3dx9.h>

#define PLAYERMOVESPEED 5.0f
#define PLAYERJUMPPOWER -7.5f

struct Player {
	D3DXVECTOR2 WindowPos;
	D3DXVECTOR2 WorldPos;
	float MovementX;
	float MovementY;
	float JumpPower;
	int Hp;
	int LifeRedused;//残気
	bool Jumping;
	bool bePunchUP;
	bool bePunchDOWN;
	bool beInvincible;//無敵
	bool beLeft;
	bool beActive;
};

//playerのデータ使いたい人はつかってね
Player* GetplayerData();

D3DXVECTOR2* GetBasePoint();

void PlayerControl();
void PlayerInit();
void MovePlayer();

#endif
