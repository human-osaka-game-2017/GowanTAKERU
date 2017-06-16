#ifndef BULLETCONTROL
#define BULLETCONTROL

#include<d3dx9.h>

enum BulletKind{bullet01};

struct Bullet {
	BulletKind BulletKind;
	D3DXVECTOR2 WindowPos;
	D3DXVECTOR2 WorldPos;
	float radius;//”¼Œa
	float rad;//Šp“x
	float save_playercoordinateX, save_playercoordinateY;
	int dmg;
	int reflect_max;
	int reflect_cnt;
	bool wasReflect;
	bool beActive;
};


#endif
