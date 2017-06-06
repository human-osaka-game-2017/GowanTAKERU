#include"Player.h"
#include"Control.h"

#include"MapRender.h"
#include"AmmoRender.h"

struct Player g_player;
extern struct Ammo g_enemyAmmo;

void PlayerInit(){

	g_player.cx = 200.0f;
	g_player.cy = 300.0f;
	g_player.jump_v0 = -20.0f;
	g_player.hp = 100.0f;
	g_player.beshotUP = false;
	g_player.beshotDOWN = false;
	g_player.shot_cnt = 0;

}

enum KEYSTATE g_Key[KEYMAX];


void Player_control(){

	//-------------------------------------------------------------------------
	//プレイヤー重力かけるぜ
	//-------------------------------------------------------------------------
	//プレイヤーの真下のマップチップ番号
	int playerposX_map = (int)g_player.cx / TIPSIZE;
	int playerposY_map = (int)(g_player.cy+ PlayerSizeH/2) / TIPSIZE;

	//プレイヤーの下が何もなかったら
	if (g_map[playerposY_map][playerposX_map] == nothing) {
		//重力をかける
		g_player.jump_v0 += (float)Gravity;
		g_player.cy += g_player.jump_v0;
	}
	//床についたら
	else {
		g_player.cy = (playerposY_map-1)*TIPSIZE- TIPSIZE/2;
		g_player.jump_v0 = -20.0f;
	}

	//------------------------------------------------------------------------
	//左右に動かすぜ
	//------------------------------------------------------------------------
	KeyCheck(&g_Key[KEY_LEFT], DIK_LEFT);
	KeyCheck(&g_Key[KEY_RIGHT], DIK_RIGHT);
	if (g_Key[KEY_LEFT] == KEY_ON) {
		g_player.cx -= PlayerMoveSpeed;
	}
	if (g_Key[KEY_RIGHT] == KEY_ON) {
		g_player.cx += PlayerMoveSpeed;
	}


	//-------------------------------------------------------------------------
	//ジャンプさせる
	//-------------------------------------------------------------------------
	KeyCheck(&g_Key[KEY_C], DIK_C);
	if (g_Key[KEY_C] == KEY_ON) {
		g_player.cy += g_player.jump_v0;
	}

	//-------------------------------------------------------------------------
	//弾をはじき返す角度
	//-------------------------------------------------------------------------
	KeyCheck(&g_Key[KEY_Z],DIK_Z);
	KeyCheck(&g_Key[KEY_X], DIK_X);

	if(g_Key[KEY_Z] == KEY_ON)
	{
		g_player.beshotDOWN = true;
		
	}
	if (g_Key[KEY_X] == KEY_ON) {

		g_player.beshotUP = true;

	}

	if (g_player.beshotUP == true || g_player.beshotDOWN == true) {

		g_player.shot_cnt++;

	}

	if (g_player.shot_cnt != 0 && g_player.shot_cnt < 10) {
		if (Circle_Hit(g_player.cx + 10, g_player.cy + 10, 50.0f, g_enemyAmmo.cx, g_enemyAmmo.cy, AmmoSize / 2)) {
			if (g_enemyAmmo.wasReflect == false) {
				g_enemyAmmo.wasReflect = true;

				if (g_player.beshotUP == true) {
					g_enemyAmmo.rad = D3DXToRadian(0.0f);
				}
				if (g_player.beshotDOWN == true) {
					g_enemyAmmo.rad = D3DXToRadian(60.f);
				}
			}
		}
		
	}

	if (10 < g_player.shot_cnt && g_player.shot_cnt < 20) {
		if (Circle_Hit(g_player.cx + 10, g_player.cy, 50.0f, g_enemyAmmo.cx, g_enemyAmmo.cy, AmmoSize / 2)) {
			if (g_enemyAmmo.wasReflect == false) {
				g_enemyAmmo.wasReflect = true;
				g_enemyAmmo.rad = D3DXToRadian(30.f);
			}
		}
	}

	if (20 < g_player.shot_cnt && g_player.shot_cnt < 30) {
		if (Circle_Hit(g_player.cx + 10, g_player.cy - 10, 50.0f, g_enemyAmmo.cx, g_enemyAmmo.cy, AmmoSize / 2)) {
			if (g_enemyAmmo.wasReflect == false) {
				g_enemyAmmo.wasReflect = true;

				if (g_player.beshotUP == true) {
					g_enemyAmmo.rad = D3DXToRadian(60.f);
				}
				if (g_player.beshotDOWN == true) {
					g_enemyAmmo.rad = D3DXToRadian(0.f);
				}
			}
		}

	}

	if (g_player.shot_cnt >= 30) {
		g_player.beshotDOWN = false;
		g_player.beshotUP = false;
		g_player.shot_cnt = 0;
	}
	
}

