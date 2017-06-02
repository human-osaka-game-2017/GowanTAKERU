#include"Player.h"

struct Player g_player;

void PlayerInit(){

	g_player.cx = 200;
	g_player.cy = 300;
	g_player.hp = 100;
	g_player.beshot = false;
	g_player.shot_cnt = 0;

}

enum KEYSTATE g_Key[KEYMAX];

//void Player_control(){
//
//	KeyCheck(&g_Key[KEY_Z],DIK_Z);
//
//	if(g_Key[KEY_Z] == KEY_ON)
//	{
//		g_player.beshot = true;
//		
//	}
//	
//	g_player.shot_cnt++;
//	//if(){
//	if(g_player.shot_cnt<)
//
//
//
//}

