#ifndef ENEMYCONTROL_H
#define ENEMYCONTROL_H
#include<d3dx9.h> 
#define MOVE_SUPEED 3.0f//エネミーのスピード
#define ENEMYNUMBER 3//エネミーの数
#define GRAVITY 1//エネミーにかかる重力



enum EnemyKind { enemyKind01 = 1,enemyKind02 = 2 ,enemyKind03 = 3 };


/*敵の種類
kind01＝通常地上MOB
kind02=通常空中MOB
*/

struct Enemy { 
	D3DXVECTOR2 WindowPos;
	D3DXVECTOR2 WorldPos;
	EnemyKind enemyKind;//エネミーの種類
	float Speed;
	float MovementX;//エネミーの1フレーム当たりの移動量
	float MovementY;
	int Atk;
	int Hp;
	int bulletFreamCount;//弾を撃つフラグを立てるためのフレームを図っている
	bool beDead;//死んでいるか
	bool beActive;//活動中か
	bool beLeft;//左（右）どうっち向いてるか

};

////エネミーデータ
Enemy* GetenemyData();
void EnemyInit();
void EnemyControl();
void MoveEnemy(int enemyNum);

#endif
