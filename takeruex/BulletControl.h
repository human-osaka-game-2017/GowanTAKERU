#ifndef BULLETCONTROL
#define BULLETCONTROL

#include<d3dx9.h>

enum BulletKind{Bullet01};

struct Bullet {
	BulletKind BulletKind;
	D3DXVECTOR2 WindowPos;
	D3DXVECTOR2 WorldPos;
	float radius;//”¼Œa
	float rad;//Šp“x
	int dmg;
	float save_playercoordinateX, save_playercoordinateY;
	bool wasReflect;
	int reflect_max;
	int reflect_cnt;
	bool beActive;
};


#endif
