#include "BulletControl.h"
#include"PlayerControl.h"
#include"EnemyControl.h"
#include"MainControl.h"
#include"MapControl.h"

static Bullet g_bullet[BULLETNUMBER];

Bullet* GetBullet() {
	return g_bullet;
}

void BulletInit() {

	for (int i = 0; i < BULLETNUMBER; i++) {

		g_bullet[i].beActive = false;
		g_bullet[i].BulletKind = bullet01;
		g_bullet[i].Atk = 10;
		g_bullet[i].Size = 0;
		g_bullet[i].Rad = 0;
		g_bullet[i].Radius = 0;
		g_bullet[i].Speed = 0;
		g_bullet[i].ReflectCnt = 0;
		g_bullet[i].ReflectMax = 0;
		g_bullet[i].SaveCoordinate.x = 0;
		g_bullet[i].SaveCoordinate.y = 0;
		g_bullet[i].wasReflect = false;
		g_bullet[i].WindowPos.x = 0;
		g_bullet[i].WindowPos.y = 0;
		g_bullet[i].WorldPos.x = 0;
		g_bullet[i].WorldPos.y = 0;
	}
}

void BulletCreate(int num, BulletKind bulletKind) {

	Enemy* enemy = GetenemyData();
	Player* player = GetplayerData();

	g_bullet[num].beActive = true;
	g_bullet[num].WindowPos = enemy->WindowPos;
	g_bullet[num].WorldPos = enemy->WorldPos;

	switch (bulletKind) {

	case bullet01:
		g_bullet[num].Speed = 6.0f;
		g_bullet[num].Size = 30;
		g_bullet[num].ReflectMax = 3;
		g_bullet[num].SaveCoordinate = player->WorldPos;

		g_bullet[num].Rad = Calculate_rad(
			g_bullet[num].WorldPos.x,
			g_bullet[num].WorldPos.y,
			g_bullet[num].SaveCoordinate.x,
			g_bullet[num].SaveCoordinate.y
		);
		break;

	}

}

void BulletMoveNomal(int num) {

	g_bullet[num].WindowPos.x += g_bullet[num].Speed*cos(g_bullet[num].Rad);
	g_bullet[num].WindowPos.y += g_bullet[num].Speed*sin(g_bullet[num].Rad);
	g_bullet[num].WorldPos.x += g_bullet[num].Speed*cos(g_bullet[num].Rad);
	g_bullet[num].WorldPos.y += g_bullet[num].Speed*sin(g_bullet[num].Rad);
}


void BulletControl() {
	
	MapNumXY mapNum;

	for (int i = 0; i < BULLETNUMBER; i++) {
		if (g_bullet[i].beActive) {
			BulletMoveNomal(i);
		}
	}

	MapchipNumberSpecify(&mapNum, &g_bullet->WorldPos);
	int map = MapKindSpecify(&mapNum);


}