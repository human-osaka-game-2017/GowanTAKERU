#ifndef BULLETCONTROL
#define BULLETCONTROL

#include<d3dx9.h>

#define BULLETNUMBER 60

enum BULLETKIND{//	反射回数		スピード		威力
	BULLETNORMAL1,//	5				5			10
	BULLETNORMAL2,//	4				7			10
	BULLETNORMAL3,//	5				7			7
	BULLETNORMAL4,//
	BULLETNORMAL5,//
	BULLETTARGET1,//	1				6			10
	BULLETTARGET2,//	3				6			10
	BULLETTARGET3,//	5				6			10
	BULLETTARGET4,//	3				8			10
	BULLETTARGET5,//	3				5			15
	BULLETTARGET6,//
	NONREFLECTTARGET1,//3				5			7
	NONREFLECTTARGET2,//3				5			25
	HOMING		  //	1				5			10
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
	Bullet* next;
	Bullet* previous;
};

Bullet* GetBullet();

//下のやり方でbulletを探索できます
//Bullet* pFirstBullet = GetFirstBulletAddress();
//for (Bullet* pSearchBullet = pFirstBullet->next; pSearchBullet != NULL; pSearchBullet = pSearchBullet->next)
Bullet* GetFirstBulletAddress();

//指定された地点に弾を生成します
//第一引数 ワールドポジションの発射地点
//第二引数 生成する弾の種類
//第三引数 弾に角度を加えれます
void BulletCreate(const D3DXVECTOR2& launchingSite, BULLETKIND bulletKind, float plusDeg = 0.0f);

//角度からmovementをセットします
void SetBulletMovement(Bullet* pBullet);

//指定された弾を消します（前後をつなげてfreeします）
//渡されたアドレスは一つ前のアドレスに変更します
//第一引数 消したい弾のアドレスのアドレス
void DeleteBullet(Bullet** DeletionBullet);

//すべての弾をfreeします
void DeleteALLBullet();

void BulletControl();

void BulletInit();

void MoveBullet();


#endif
