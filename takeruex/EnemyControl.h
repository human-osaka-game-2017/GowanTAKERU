#ifndef ENEMYCONTROL_H
#define ENEMYCONTROL_H
#include"BulletControl.h"
#include<d3dx9.h> 

#define ENEMY_KIND_MAX 31

#define ENEMYGRAVITY 3//エネミーにかかる重力

#define INTMAX 2147483647

enum enemyRushDistance {
	RushDistance =300//突進距離
};

struct Enemy { 
	D3DXVECTOR2 WindowPos;
	D3DXVECTOR2 WorldPos;
	D3DXVECTOR2 EnemyBasePoint;
	D3DXVECTOR2 FrontWorldPos;
	int BulletKind;
	int  enemyKind;//エネミーの種類
	float Speed;
	float MovementX;//エネミーの1フレーム当たりの移動量
	float MovementY;
	int Atk;
	int Hp;
	int bulletFrameCount;//弾を撃つフラグを立てるためのフレームを図っている
	int firingInterval;//発射感覚
	float BulletDeg;
	bool beDead;//死んでいるか
	bool beActive;//活動中か
	bool beLeft;//左（右）どっち向いてるか
	float Height;//画像のｻｲｽﾞ高さ
	float Width;//画像サイズの幅
	float tu, tv;
	bool Rush;//突進のフラグ

};

struct EditableEnemyData {
	int BulletKind;
	float BulletDeg;
	float ShotInterval;//発射間隔（秒数）
	float Speed;
	float tu, tv, Width, Height;
	int	Hp;
	int Atk;
	
};

void LeadEnemyData();

////エネミーデータ
int GetEnemyMax();
Enemy* GetEnemyData();
void EnemyInit();
void EnemyControl();
void MoveEnemy();
void FreeEnemyData();

#endif
