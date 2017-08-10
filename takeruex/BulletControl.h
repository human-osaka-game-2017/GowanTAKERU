#ifndef BULLETCONTROL
#define BULLETCONTROL

#include<d3dx9.h>

#define BULLETNUMBER 60

enum BULLETKIND{BULLET01, HOMING};

struct Bullet {
	BULLETKIND BulletKind;
	D3DXVECTOR2 WindowPos;
	D3DXVECTOR2 WorldPos;
	D3DXVECTOR2 SaveCoordinate;
	float MovementX;
	float MovementY;
	float Size;
	float Speed;
	float Radius;//îºåa
	float Rad;//äpìx
	int Atk;
	int ReflectMax;
	int ReflectCnt;
	bool wasReflect;
	bool beActive;
};

Bullet* GetBullet();

void BulletCreate(int bulletNum, int enemyNum, BULLETKIND bulletKind);

void BulletControl();

void BulletInit();

//bulletÇè¡ãéÇµÇ‹Ç∑
void DeactivateBullet(int num);

void MoveBullet();


#endif
