#include "BulletControl.h"
#include"PlayerControl.h"
#include"EnemyControl.h"
#include"MainControl.h"
#include"MapControl.h"
#include"CommonRender.h"

void SetBulletMovement(int bulletNum);

static Bullet g_bullet[BULLETNUMBER];

Bullet* GetBullet() {
	return g_bullet;
}

void BulletInit() {

	for (int i = 0; i < BULLETNUMBER; i++) {

		g_bullet[i].beActive = false;
		g_bullet[i].BulletKind = BULLET01;
		g_bullet[i].Atk = 10;
		g_bullet[i].Size = 0;
		g_bullet[i].Rad = 0;
		g_bullet[i].Radius = 0;
		g_bullet[i].Speed = 0;
		g_bullet[i].MovementX = 0;
		g_bullet[i].MovementY = 0;
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

void BulletCreate(int bulletNum,int enemyNum, BULLETKIND bulletKind) {

	Enemy* enemy = GetenemyData();
	Player* player = GetplayerData();

	g_bullet[bulletNum].beActive = true;
	g_bullet[bulletNum].WindowPos = enemy[enemyNum].WindowPos;
	g_bullet[bulletNum].WorldPos = enemy[enemyNum].WorldPos;

	switch (bulletKind) {

	case BULLET01:
		g_bullet[bulletNum].Speed = 6.0f;
		g_bullet[bulletNum].Size = 30;
		g_bullet[bulletNum].ReflectMax = 3;
		g_bullet[bulletNum].SaveCoordinate = player->WindowPos;

		g_bullet[bulletNum].Rad = Calculate_rad(
			g_bullet[bulletNum].WindowPos.x,
			g_bullet[bulletNum].WindowPos.y,
			g_bullet[bulletNum].SaveCoordinate.x,
			g_bullet[bulletNum].SaveCoordinate.y
		);
		break;

	}

}

void BulletControl() {
	
	MapNumXY mapNum;

	for (int i = 0; i < BULLETNUMBER; i++) {
		if (g_bullet[i].beActive) {

			SetBulletMovement(i);

			MapchipNumberSpecify(&mapNum, &g_bullet->WorldPos);

			int map = MapKindSpecify(&mapNum);

			//âÊñ äOÇ≈è¡Ç¶ÇÈ
			if (g_bullet[i].WindowPos.x < -64 || DISPLAY_WIDTH + 64 < g_bullet[i].WindowPos.x) {
				g_bullet[i].beActive = false;
			}
			if (g_bullet[i].WindowPos.y < -64 || DISPLAY_HEIGHT + 64 < g_bullet[i].WindowPos.y) {
				g_bullet[i].beActive = false;
			}
		}
	}
}

void MoveBullet() {
	D3DXVECTOR2 BasePoint0 = D3DXVECTOR2(DISPLAY_WIDTH / 2, DISPLAY_HEIGHT / 2);
	D3DXVECTOR2* basePoint = GetBasePoint();
	for (int i = 0; i < BULLETNUMBER; i++) {
		if (g_bullet[i].beActive) {
			g_bullet[i].WorldPos.x += g_bullet[i].MovementX;
			g_bullet[i].WorldPos.y += g_bullet[i].MovementY;
			g_bullet[i].WindowPos.x = g_bullet[i].WorldPos.x - (basePoint->x - BasePoint0.x);
			g_bullet[i].WindowPos.y = g_bullet[i].WorldPos.y - (basePoint->y - BasePoint0.y);;
			g_bullet[i].MovementX = g_bullet[i].MovementY = 0;
		}
	}
}

void SetBulletMovement(int bulletNum) {
	g_bullet[bulletNum].MovementX += g_bullet[bulletNum].Speed*cos(g_bullet[bulletNum].Rad);
	g_bullet[bulletNum].MovementY -= g_bullet[bulletNum].Speed*sin(g_bullet[bulletNum].Rad);
}

void DeactivateBullet(int num) {
	g_bullet[num].beActive = false;
	g_bullet[num].wasReflect = false;
}