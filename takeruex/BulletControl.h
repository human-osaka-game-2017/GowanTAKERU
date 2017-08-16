#ifndef BULLETCONTROL
#define BULLETCONTROL

#include<d3dx9.h>

#define BULLETNUMBER 60

enum BULLETKIND{
	BULLET01,//壁に当たると消える
	BULLET02,//壁で2回反射する
	BULLET03,//壁で4回反射する
	FASTBULLET,//スピード１．３倍
	POWERBULLET,//高威力
	HOMING
};

struct Bullet {
	BULLETKIND BulletKind;
	D3DXVECTOR2 WindowPos;
	D3DXVECTOR2 WorldPos;
	D3DXVECTOR2 SaveCoordinate;
	float MovementX;
	float MovementY;
	float Size;
	float Speed;
	//float Radius;//半径
	float Rad;//角度
	int Atk;
	int ReflectMax;
	int ReflectCnt;
	bool wasReflect;
	bool beActive;
	Bullet* next;
	Bullet* previous;
};

Bullet* GetBullet();

//下のやり方でbulletを探索できます
//Bullet* pFirstBullet = GetFirstBulletAddress();
//for (Bullet* pSearchBullet = pFirstBullet->next; pSearchBullet != NULL; pSearchBullet = pSearchBullet->next)
Bullet* GetFirstBulletAddress();

//指定された地点に弾を生成します
//第一引数 発射地点
//第二引数 生成する弾の種類
void BulletCreate(const D3DXVECTOR2& launchingSite, BULLETKIND bulletKind);

//角度からmovementをセットします
void SetBulletMovement(Bullet* pBullet);

//指定された弾を消します（前後をつなげてfreeします）
//渡されたアドレスは一つ前のアドレスに変更します
//第一引数 消したい弾のアドレスのアドレス
void DeleteBullet(Bullet** DeletionBullet);

//すべての弾をfreeします
void DeleteALLBullet();

void BulletCreate(int bulletNum, int enemyNum, BULLETKIND bulletKind);

void BulletControl();

void BulletInit();

//bulletを消去します
void DeactivateBullet(int num);

void MoveBullet();


#endif
