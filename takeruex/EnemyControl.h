#ifndef ENEMYCONTROL_H
#define ENEMYCONTROL_H

#include<d3dx9.h>
#define MOVE_SUPEED 3.0f
#define ENEMYNUMBER 2



struct Enemy { 
	D3DXVECTOR2 WindowPos;
	D3DXVECTOR2 WorldPos;
	int Atk;
	int Hp;
	int Kind;
	int bulletFreamCount;//弾を撃つフラグを立てるためのフレームを図っている
	bool beDead;//死んでいるか
	bool beActive;//活動中か
	bool beLeft;//左（右）どうっち向いてるか

};

////エネミーデータ
Enemy* GetenemyData();
void EnemyInit();
void EnemyControl();

#endif
