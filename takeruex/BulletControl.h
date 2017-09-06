#ifndef BULLETCONTROL
#define BULLETCONTROL

#include<d3dx9.h>

#define BULLETNUMBER 60

enum BULLETKIND{
	BULLETNORMAL1,
	BULLETNORMAL2,
	BULLETNORMAL3,
	BULLETNORMAL4,
	BULLETNORMAL5,
	BULLETNORMAL6,
	BULLETNORMAL7,
	BULLETNORMAL8,
	BULLETNORMAL9,
	BULLETNORMAL10,
	BULLETTARGET1,
	BULLETTARGET2,
	BULLETTARGET3,
	BULLETTARGET4,
	BULLETTARGET5,
	BULLETTARGET6,
	BULLETTARGET7,
	BULLETTARGET8,
	BULLETTARGET9,
	BULLETTARGET10,
	NONREFLECTNORMAL1,
	NONREFLECTNORMAL2,
	NONREFLECTNORMAL3,
	NONREFLECTNORMAL4,
	NONREFLECTNORMAL5,
	NONREFLECTNORMAL6,
	NONREFLECTNORMAL7,
	NONREFLECTNORMAL8,
	NONREFLECTNORMAL9,
	NONREFLECTNORMAL10,
	NONREFLECTTARGET1,
	NONREFLECTTARGET2,
	NONREFLECTTARGET3,
	NONREFLECTTARGET4,
	NONREFLECTTARGET5,
	NONREFLECTTARGET6,
	NONREFLECTTARGET7,
	NONREFLECTTARGET8,
	NONREFLECTTARGET9,
	NONREFLECTTARGET10,
	HOMING1,
	HOMING2,
	HOMING3,
	HOMING4,
	HOMING5,
	HOMING6,
	HOMING7,
	HOMING8,
	HOMING9,
	HOMING10,
	BULLET_MAX
};

struct Bullet {
	int BulletKind;
	D3DXVECTOR2 WindowPos;
	D3DXVECTOR2 WorldPos;
	D3DXVECTOR2 SaveCoordinate;
	float MovementX;
	float MovementY;
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

struct EditableBulletData {
	int Atk;
	int ReflectMax;
	float Speed;
	float Size;
	float tu, tv;
};

void LoadBulletData();

EditableBulletData* GetEditableBulletData();

//下のやり方でbulletを探索できます
//Bullet* pFirstBullet = GetFirstBulletAddress();
//for (Bullet* pSearchBullet = pFirstBullet->next; pSearchBullet != NULL; pSearchBullet = pSearchBullet->next)
Bullet* GetFirstBulletAddress();

//指定された地点に弾を生成します
//第一引数 ワールドポジションの発射地点
//第二引数 生成する弾の種類
//第三引数 弾に角度を加えれます
void BulletCreate(const D3DXVECTOR2& launchingSite, int bulletKind, float plusDeg = 0.0f);

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
