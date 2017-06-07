#include"Ammo.h"
#include"Player.h"
#include "Control.h"
#include"AmmoRender.h"

struct Ammo g_enemyAmmo[];

void AmmoInit() {
	for (int i = 0; i < AmmoNumber; i++) {
		g_enemyAmmo[i].cx = 1000.0f;
		g_enemyAmmo[i].cy = 500.0f;
		g_enemyAmmo[i].save_playercoordinateX = -1.0f;
		g_enemyAmmo[i].dmg = 20.0f;
		g_enemyAmmo[i].rad = 0.0f;
		g_enemyAmmo[i].berender = false;
		g_enemyAmmo[i].radius = 15.0f;
		g_enemyAmmo[i].reflect_cnt = 0;
		g_enemyAmmo[i].reflect_max = 3;
		g_enemyAmmo[i].wasReflect = false;
	}
}

void Ammomove() {
	
	//その玉の配列番号
	static int ammovalue_flg = 0;
	//2００フレーム目に１枚目描画、、、を続ける
	if (g_frcnt == 200) {

		g_enemyAmmo[ammovalue_flg].berender = true;
	
		ammovalue_flg++;

		if (AmmoNumber <= ammovalue_flg) {
			ammovalue_flg = 0;
			AmmoInit();
		}

	}


	for (int i = 0; i < AmmoNumber; i++) {

		if (g_enemyAmmo[i].berender == true) {

			//プレイヤーめがけて（基本動作）
			if (g_enemyAmmo[i].wasReflect == false) {

				//その時のプレイヤー座標記憶。プレイヤーの座標は正だから初期値負で判定
				if (g_enemyAmmo[ammovalue_flg].save_playercoordinateX == -1.0f) {
					g_enemyAmmo[ammovalue_flg].save_playercoordinateX = g_player.cx;
					g_enemyAmmo[ammovalue_flg].save_playercoordinateY = g_player.cy;
				}
				
				double rad = Calculate_rad(
					g_enemyAmmo[ammovalue_flg].cx,
					g_enemyAmmo[ammovalue_flg].cy,
					g_enemyAmmo[ammovalue_flg].save_playercoordinateX,
					g_enemyAmmo[ammovalue_flg].save_playercoordinateY
				);
				g_enemyAmmo[i].cx += Ammo_MOVESPEED*cos(rad);
				g_enemyAmmo[i].cy += Ammo_MOVESPEED*sin(rad);
			}

			//弾かれたとき
			if (g_enemyAmmo[i].wasReflect == true) {
				g_enemyAmmo[i].cx += -Ammo_MOVESPEED*cos(g_enemyAmmo[i].rad);
				g_enemyAmmo[i].cy += Ammo_MOVESPEED*sin(g_enemyAmmo[i].rad);
			}

		}
	}
}