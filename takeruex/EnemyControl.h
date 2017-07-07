#ifndef ENEMYCONTROL_H
#define ENEMYCONTROL_H
#include<d3dx9.h> 
#define ENEMYNUMBER 10//エネミーの数
//#define GRAVITY 1//エネミーにかかる重力



enum ENEMYKIND {FLYINGENEMY = 5 ,FLYINGENEMY_KYE01=6,FLYINGENEMY_KYE02=7, WALKINGENEMY=9, WALKINGENEMY_KYE02=8};
/*エネミーの種類
空中型
Flyingenemy  ノーマル空中型
Flyingenemy_Kye01 キー１番持ちの空中型
Flyingenemy_Kye02 キー2番持ちの空中型
地上型
Walkingenemy ノーマル地上型
Walkingenemy_Kye02 キー2番持ちの地上型
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
	int bulletFreamCount;//弾を撃つフラグを立てるためのフレームを図っている
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
