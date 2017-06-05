#include"Player.h"
#include"Control.h"

#include"MapRender.h"

struct Player g_player;
extern struct Ammo g_enemyAmmo;

void PlayerInit(){

	g_player.cx = 200.0f;
	g_player.cy = 300.0f;
	g_player.jump_v0 = 10.0f;
	g_player.hp = 100.0f;
	g_player.beshotUP = false;
	g_player.beshotDOWN = false;
	g_player.shot_cnt = 0;

}

enum KEYSTATE g_Key[KEYMAX];


void Player_control(){

	int playerposX_map = (int)g_player.cx / TIPSIZE;
	int playerposY_map = (int)(g_player.cy+ PlayerSizeH/2) / TIPSIZE;

	//プレイヤーの下が何もなかったら
	if (g_map[playerposY_map][playerposX_map] == nothing) {
		//重力をかける
		g_player.jump_v0 += (float)Gravity;
		g_player.cy += (float)Gravity;
	}
	else {
		g_player.jump_v0 = 10.0f;
	}

	//弾をはじき返す角度
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


	//if(あたり判定){
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

