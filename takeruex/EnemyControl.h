#ifndef ENEMYCONTROL_H
#define ENEMYCONTROL_H
#include<d3dx9.h> 
#define ENEMYNUMBER 10//エネミーの数
//#define GRAVITY 1//エネミーにかかる重力






struct Enemy { 
	D3DXVECTOR2 WindowPos;
	D3DXVECTOR2 WorldPos;
	int  enemyKind;//エネミーの種類
	float Speed;
	float MovementX;//エネミーの1フレーム当たりの移動量
	float MovementY;
	int Atk;
	int Hp;
	int bulletFrameCount;//弾を撃つフラグを立てるためのフレームを図っている
	int firingInterval;//発射感覚
	bool beDead;//死んでいるか
	bool beActive;//活動中か
	bool beLeft;//左（右）どうっち向いてるか
	int size;


};

////エネミーデータ
Enemy* GetenemyData();
void EnemyInit();
void EnemyControl();
void MoveEnemy();

#endif
