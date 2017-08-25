#ifndef ENEMYCONTROL_H
#define ENEMYCONTROL_H
#include<d3dx9.h> 
#define ENEMYGRAVITY 3//エネミーにかかる重力

struct Enemy { 
	D3DXVECTOR2 WindowPos;
	D3DXVECTOR2 WorldPos;
	D3DXVECTOR2 EnemyBasePoint;
	int  enemyKind;//エネミーの種類
	float Speed;
	float MovementX;//エネミーの1フレーム当たりの移動量
	float MovementY;
	int Atk;
	int Hp;
	float bulletFrameCount;//弾を撃つフラグを立てるためのフレームを図っている
	float firingInterval;//発射感覚
	bool beDead;//死んでいるか
	bool beActive;//活動中か
	bool beLeft;//左（右）どっち向いてるか
	//float Height;//画像のｻｲｽﾞ高さ
	//float Width;//画像サイズの幅


};

////エネミーデータ
int GetEnemyMax();
Enemy* GetEnemyData();
void EnemyInit();
void EnemyControl();
void MoveEnemy();
void FreeEnemyData();

#endif
