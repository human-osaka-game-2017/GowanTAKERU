#ifndef PLAYERCONTROL_H
#define PLAYERCONTROL_H

#include<d3dx9.h>

#define PLAYERMOVESPEED 5.0f
#define PLAYERJUMPPOWER -8.0f

struct Player {
	D3DXVECTOR2 WindowPos;
	D3DXVECTOR2 WorldPos;
	float MovementX;
	float MovementY;
	float JumpPower;
	int Hp;
	int LifeReduced;//残気
	int currentAnimState;
	bool Jumping;
	bool beUPSwing;
	bool beDownSwing;
	bool beInvincible;//無敵
	bool beLeft;
	bool beActive;
};

//playerのデータ使いたい人はつかってね
Player* GetplayerData();

D3DXVECTOR2* GetBasePoint();
//D3DXVECTOR2 BasePoint0 = D3DXVECTOR2(DISPLAY_WIDTH / 2, DISPLAY_HEIGHT / 2);

void PlayerControl();
void PlayerInit();
void MovePlayer();

#endif
