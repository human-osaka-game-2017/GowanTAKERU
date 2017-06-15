#include"PlayerControl.h"
#include"MapRender.h"
#include"MapControl.h"
#include"CommonRender.h"
#include"CommonControl.h"
#include"MainControl.h"
#include"PlayerRender.h"

Player g_player;
static D3DXVECTOR2 g_BasePoint = D3DXVECTOR2(DISPLAY_WIDTH / 2, DISPLAY_HEIGHT / 2);

D3DXVECTOR2* GetBasePoint() {
	return &g_BasePoint;
}

Player* GetplayerData() {
	return &g_player;
}

void PlayerInit() {

	g_player.WindowPos.x = g_player.WorldPos.x = 200.0f;
	g_player.WindowPos.y = g_player.WorldPos.y = 150.0f;
	g_player.JumpPower = 0.0f;
	g_player.Jumping = false;
	g_player.hp = 100;
	g_player.beLeft = false;
	g_player.bePunchDOWN = false;
	g_player.bePunchUP = false;

}

void PlayerMove() {

	KEYSTATE* Key = GetKey();

	//------------------------------------------------------------------------
	//左右に動かすぜ
	//------------------------------------------------------------------------
	KeyCheck(&Key[KEY_LEFT], DIK_LEFT);
	KeyCheck(&Key[KEY_RIGHT], DIK_RIGHT);
	KeyCheck(&Key[KEY_C], DIK_C);

	if (Key[KEY_LEFT] == KEY_ON) {

		g_player.WorldPos.x -= PLAYERMOVESPEED;

		if (g_player.WindowPos.x < 300) {
			g_BasePoint.x -= PLAYERMOVESPEED;
		}
		else {
			g_player.WindowPos.x -= PLAYERMOVESPEED;
		}
	}

	if (Key[KEY_RIGHT] == KEY_ON) {

		g_player.WorldPos.x += PLAYERMOVESPEED;

		if (900 < g_player.WindowPos.x) {
			g_BasePoint.x += PLAYERMOVESPEED;
		}
		else {
			g_player.WindowPos.x += PLAYERMOVESPEED;
		}
	}

	//-------------------------------------------------------------------------
	//プレイヤー重力かけるぜ
	//-------------------------------------------------------------------------

	//常に重力をかける
	g_player.JumpPower += GRAVITY;
	g_player.WorldPos.y += g_player.JumpPower;

	if (g_player.WindowPos.y > 300) {
		g_BasePoint.y += g_player.JumpPower;
	}
	else {
		g_player.WindowPos.y += g_player.JumpPower;
	}

	//プレイヤーの右足、左足のマップチップ番号

	MapNumXY Player_rightfoot_MapNum;
	MapNumXY Player_leftfoot_MapNum;
	D3DXVECTOR2 player_rightfoot;
	D3DXVECTOR2 player_leftfoot;
	player_rightfoot.x = g_player.WorldPos.x + PLAYERSIZEWIDHE / 2;
	player_leftfoot.x = g_player.WorldPos.x -PLAYERSIZEWIDHE / 2;
	player_leftfoot.y = player_rightfoot.y = g_player.WorldPos.y + PLAYERSIZEHEIGHT / 2;

	MapchipNumberSpecify(&Player_rightfoot_MapNum, &player_rightfoot);
	MapchipNumberSpecify(&Player_leftfoot_MapNum, &player_leftfoot);

	static int frcnt = 0;

	//プレイヤーの下が床
	if (MapKindSpecify(&Player_rightfoot_MapNum, DOWN) == yuka || MapKindSpecify(&Player_leftfoot_MapNum, DOWN) == yuka) {
		
		g_player.Jumping = false;
		g_player.JumpPower = 0.0f;
		frcnt = 0;
		//垂直抗力をかける
		g_player.JumpPower -= GRAVITY;

		//めり込みをふせぐ
			float footdiff = player_rightfoot.y - (Player_rightfoot_MapNum.NumY)*(TIPSIZE);
			g_player.WindowPos.y += footdiff;
			g_player.WorldPos.y += footdiff;


		if (Key[KEY_C] == KEY_PUSH) {
			g_player.JumpPower = -10.0f;
			g_player.Jumping = true;
		}



	}

	else if (0) {
		
	}

	if (g_player.Jumping) {
		frcnt++;
		if (frcnt != 0 && frcnt < 20) {
			if (Key[KEY_C] == KEY_ON) {
				g_player.JumpPower -= GRAVITY-0.1;
			}
		}
	}
	
}

void PlayerControl() {

	PlayerMove();

	KEYSTATE* Key = GetKey();

	//-------------------------------------------------------------------------
	//弾をはじき返す角度
	//-------------------------------------------------------------------------
	KeyCheck(&Key[KEY_Z], DIK_Z);
	KeyCheck(&Key[KEY_X], DIK_X);

	if (Key[KEY_Z] == KEY_ON)
	{
		g_player.bePunchDOWN = true;

	}
	if (Key[KEY_X] == KEY_ON) {

		g_player.bePunchUP = true;

	}

	static int frcnt = 0;

	if (g_player.bePunchUP == true || g_player.bePunchDOWN == true) {

		frcnt++;

	}

	/*for (int i = 0; i < AmmoNumber; i++) {
		if (frcnt != 0 && frcnt < 10) {
			if (Circle_Hit(g_player.cx + 10, g_player.cy + 10, 50.0f, g_enemyAmmo[i].cx, g_enemyAmmo[i].cy, AmmoSize / 2)) {
				if (g_enemyAmmo[i].wasReflect == false) {
					g_enemyAmmo[i].wasReflect = true;

					if (g_player.bePunchUP == true) {
						g_enemyAmmo[i].rad = D3DXToRadian(0.0f);
					}
					if (g_player.bePunchDOWN == true) {
						g_enemyAmmo[i].rad = D3DXToRadian(60.f);
					}
				}
			}

		}

		if (10 < frcnt && frcnt < 20) {
			if (Circle_Hit(g_player.cx + 10, g_player.cy, 50.0f, g_enemyAmmo[i].cx, g_enemyAmmo[i].cy, AmmoSize / 2)) {
				if (g_enemyAmmo[i].wasReflect == false) {
					g_enemyAmmo[i].wasReflect = true;
					g_enemyAmmo[i].rad = D3DXToRadian(30.f);
				}
			}
		}

		if (20 < frcnt && frcnt < 30) {
			if (Circle_Hit(g_player.cx + 10, g_player.cy - 10, 50.0f, g_enemyAmmo[i].cx, g_enemyAmmo[i].cy, AmmoSize / 2)) {
				if (g_enemyAmmo[i].wasReflect == false) {
					g_enemyAmmo[i].wasReflect = true;

					if (g_player.bePunchUP == true) {
						g_enemyAmmo[i].rad = D3DXToRadian(60.f);
					}
					if (g_player.bePunchDOWN == true) {
						g_enemyAmmo[i].rad = D3DXToRadian(0.f);
					}
				}
			}

		}
	}

	if (frcnt >= 30) {
		g_player.bePunchDOWN = false;
		g_player.bePunchUP = false;
		frcnt = 0;
	}*/

}