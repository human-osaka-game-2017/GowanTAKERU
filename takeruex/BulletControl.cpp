#include "BulletControl.h"
#include"PlayerControl.h"

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
//	//その玉の配列番号
//	static int ammovalue_flg = 0;
//	//2００フレーム目に１枚目描画、、、を続ける
//	if (frcnt == 200) {
//
//		g_enemyAmmo[ammovalue_flg].berender = true;
//
//		ammovalue_flg++;
//
//		if (AmmoNumber <= ammovalue_flg) {
//			ammovalue_flg = 0;
//			AmmoInit();
//		}
//
//	}
//
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