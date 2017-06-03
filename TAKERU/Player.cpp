#include"Player.h"
#include"Control.h"

struct Player g_player;
extern struct Ammo g_enemyAmmo;

void PlayerInit(){

	g_player.cx = 200;
	g_player.cy = 300;
	g_player.hp = 100;
	g_player.beshotUP = false;
	g_player.beshotDOWN = false;
	g_player.shot_cnt = 0;

}

enum KEYSTATE g_Key[KEYMAX];


void Player_control(){


	//’e‚ð‚Í‚¶‚«•Ô‚·Šp“x
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


	//if(‚ ‚½‚è”»’è){
	if (g_player.shot_cnt != 0 && g_player.shot_cnt < 10) {
		if (g_player.beshotUP == true){
			g_enemyAmmo.rad = D3DXToRadian(0.0f);
		}
		if (g_player.beshotDOWN == true) {
			g_enemyAmmo.rad = D3DXToRadian(60.f);
		}
		
	}
	if (10 < g_player.shot_cnt && g_player.shot_cnt < 20) {
		g_enemyAmmo.rad = D3DXToRadian(30.f);
	}

	if (20 < g_player.shot_cnt && g_player.shot_cnt < 30) {
		if (g_player.beshotUP == true) {
			g_enemyAmmo.rad = D3DXToRadian(60.f);
		}
		if (g_player.beshotDOWN == true) {
			g_enemyAmmo.rad = D3DXToRadian(0.f);
		}

	}

	if (g_player.shot_cnt >= 30) {
		g_player.beshotDOWN = false;
		g_player.beshotUP = false;
		g_player.shot_cnt = 0;
	}
	
}

