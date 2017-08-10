#ifndef ENEMYCONTROL_H
#define ENEMYCONTROL_H
#include<d3dx9.h> 
#define ENEMYNUMBER 10//エネミーの数
//#define GRAVITY 1//エネミーにかかる重力



enum ENEMYKIND { 
	WALKINGENEMY_1=21,//地上敵
	WALKINGENEMY_2,
	WALKINGENEMY_3,
	WALKINGENEMY_4,
	WALKINGENEMY_5,
	WALKINGENEMY_HAS_KEY_1,
	WALKINGENEMY_HAS_KEY_2,
	WALKINGENEMY_HAS_KEY_3,
	FixedBattery1=31,//固定砲台
	FLYINGENEMY1,//空中敵
	FLYINGENEMY2,
	FLYINGENEMY3,
	FLYINGENEMY4,
	FLYINGENEMY5,
	FLYINGENEMY_HAS_KEY1,
	FLYINGENEMY_HAS_KEY2,
	FLYINGENEMY_HAS_KEY3,
	FixedBattery2,
};
/*エネミーの種類

*/



struct Enemy { 
	D3DXVECTOR2 WindowPos;
	D3DXVECTOR2 WorldPos;
	ENEMYKIND enemyKind;//エネミーの種類
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


};

////エネミーデータ
Enemy* GetenemyData();
void EnemyInit();
void EnemyControl();
void MoveEnemy();

#endif
