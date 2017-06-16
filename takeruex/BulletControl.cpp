#include "BulletControl.h"
#include"BulletRender.h"
#include"PlayerControl.h"

static Bullet bullet[BULLETNUMBER];

void BulletInit() {
	for (int i = 0; i < BULLETNUMBER; i++) {
		bullet->beActive = false;
		bullet->BulletKind = bullet01;
		bullet->dmg = 10;
		bullet->rad = 0;
		bullet->radius = BULLETSIZE / 2;
		bullet->reflect_cnt = 0;
		bullet->reflect_max = 3;
		bullet->save_playercoordinateX = 0;
		bullet->save_playercoordinateY = 0;
		bullet->wasReflect = false;
		bullet->WorldPos.x = 1152;
		bullet->WorldPos.y = 448;


	}
}

//void BulletMove() {
//
//	static int frcnt = 0;
//	if (frcnt == 200) {
//		frcnt = 0;
//	}
//	else {
//		frcnt++;
//	}
//
//	for (int i = 0; i < AmmoNumber; i++) {
//
//		if (g_enemyAmmo[i].berender == true) {
//
//			//プレイヤーめがけて（基本動作）
//			if (g_enemyAmmo[i].wasReflect == false) {
//
//				//その時のプレイヤー座標記憶。プレイヤーの座標は正だから初期値負で判定
//				if (g_enemyAmmo[ammovalue_flg].save_playercoordinateX == -1.0f) {
//					g_enemyAmmo[ammovalue_flg].save_playercoordinateX = g_player.cx;
//					g_enemyAmmo[ammovalue_flg].save_playercoordinateY = g_player.cy;
//				}
//
//				double rad = Calculate_rad(
//					g_enemyAmmo[ammovalue_flg].cx,
//					g_enemyAmmo[ammovalue_flg].cy,
//					g_enemyAmmo[ammovalue_flg].save_playercoordinateX,
//					g_enemyAmmo[ammovalue_flg].save_playercoordinateY
//				);
//				g_enemyAmmo[i].cx += Ammo_MOVESPEED*cos(rad);
//				g_enemyAmmo[i].cy += Ammo_MOVESPEED*sin(rad);
//			}
//
//			//弾かれたとき
//			if (g_enemyAmmo[i].wasReflect == true) {
//				g_enemyAmmo[i].cx += -Ammo_MOVESPEED*cos(g_enemyAmmo[i].rad);
//				g_enemyAmmo[i].cy += Ammo_MOVESPEED*sin(g_enemyAmmo[i].rad);
//			}
//
//		}
//	}
//}