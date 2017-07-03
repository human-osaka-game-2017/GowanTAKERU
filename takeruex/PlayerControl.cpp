#include"PlayerControl.h"
#include"MapRender.h"
#include"MapControl.h"
#include"CommonRender.h"
#include"CommonControl.h"
#include"MainControl.h"
#include"PlayerRender.h"
#include"BulletControl.h"
#include"MainHitManagement.h"

void SetPlayerMovement();
void PlayerReflectMotion();

Player g_player;
static D3DXVECTOR2 g_BasePoint = D3DXVECTOR2(DISPLAY_WIDTH / 2, DISPLAY_HEIGHT / 2);

D3DXVECTOR2* GetBasePoint() {
	return &g_BasePoint;
}

Player* GetplayerData() {
	return &g_player;
}

void PlayerInit() {

	int* map = GetMapchipData();
	D3DXVECTOR2 BasePoint0 = D3DXVECTOR2(DISPLAY_WIDTH / 2, DISPLAY_HEIGHT / 2);
	for (int i = 0; i < MAPCHIPNUM_HEIGHT; i++){
		for (int j = 0; j < MAPCHIPNUM_WIDTH; j++) {
			if (*(map + (i*MAPCHIPNUM_WIDTH + j)) == START) {

				MapNumXY playerstartMapNum = { j,i };
				PosSpecifyForMapchipNumber(&g_player.WorldPos, &playerstartMapNum);
				MapNumXY mapnum = { j,i-3 };
				PosSpecifyForMapchipNumber(&g_BasePoint, &mapnum);
			}
		}
	}

	g_player.WindowPos.x = g_player.WorldPos.x - (g_BasePoint.x - BasePoint0.x);
	g_player.WindowPos.y = g_player.WorldPos.y - (g_BasePoint.y - BasePoint0.y);
	g_player.MovementX = g_player.MovementY = 0;
	g_player.JumpPower = 0.0f;
	g_player.Jumping = false;
	g_player.Hp = 100;
	g_player.beActive = true;
	g_player.LifeRedused = 2;
	g_player.beInvincible = false;
	g_player.beLeft = false;
	g_player.bePunchDOWN = false;
	g_player.bePunchUP = false;
}

void PlayerControl() {
	if (g_player.beActive) {

		SetPlayerMovement();

		PlayerReflectMotion();
	}

}

void SetPlayerMovement() {

	KEYSTATE* Key = GetKey();
	int* map = GetMapchipData();

	//常に重力をかける
	g_player.JumpPower += GRAVITY;

	//------------------------------------------------------------------------
	//左右に動かすぜ
	//------------------------------------------------------------------------
	KeyCheck(&Key[KEY_LEFT], DIK_LEFT);
	KeyCheck(&Key[KEY_RIGHT], DIK_RIGHT);
	KeyCheck(&Key[KEY_C], DIK_C);


	if (Key[KEY_LEFT] == KEY_ON) {
		if (!g_player.bePunchDOWN && !g_player.bePunchUP) {
			g_player.beLeft = true;
		}

		g_player.MovementX -= PLAYERMOVESPEED;

	}

	if (Key[KEY_RIGHT] == KEY_ON) {
		if (!g_player.bePunchDOWN && !g_player.bePunchUP) {
			g_player.beLeft = false;
		}

		g_player.MovementX += PLAYERMOVESPEED;

	}

	////プレイヤーの右足、左足のマップチップ番号
	D3DXVECTOR2 PlayerLeftTop;
	D3DXVECTOR2 PlayerRightTop;
	D3DXVECTOR2 playerRightBottom;
	D3DXVECTOR2 playerLeftBottom;
	MapNumXY PlayerLeftTopMapNum;
	MapNumXY PlayerRightTopMapNum;
	MapNumXY PlayerRightBottomMapNum;
	MapNumXY PlayerLeftBottomMapNum;

	PlayerRightTop.x = playerRightBottom.x = g_player.WorldPos.x + PLAYERSIZEWIDHE / 2 + g_player.MovementX;
	PlayerLeftTop.x = playerLeftBottom.x = g_player.WorldPos.x - PLAYERSIZEWIDHE / 2 + g_player.MovementX;
	PlayerLeftTop.y = PlayerRightTop.y = g_player.WorldPos.y - PLAYERSIZEHEIGHT / 2 + g_player.MovementY;
	playerLeftBottom.y = playerRightBottom.y = g_player.WorldPos.y + PLAYERSIZEHEIGHT / 2 + g_player.MovementY;

	MapchipNumberSpecify(&PlayerLeftTopMapNum, &PlayerLeftTop);
	MapchipNumberSpecify(&PlayerRightTopMapNum, &PlayerRightTop);
	MapchipNumberSpecify(&PlayerRightBottomMapNum, &playerRightBottom);
	MapchipNumberSpecify(&PlayerLeftBottomMapNum, &playerLeftBottom);

	//めり込み防止
	if (MapKindSpecify_Plus1(&PlayerLeftTopMapNum, DOWN) == FLOOR || MapKindSpecify_Plus1(&PlayerLeftBottomMapNum, UP) == FLOOR) {
		float diff = (PlayerLeftBottomMapNum.NumX + 1)*(TIPSIZE)-playerLeftBottom.x;
		g_player.MovementX += diff;
	}
	if (MapKindSpecify_Plus1(&PlayerRightTopMapNum, DOWN) == FLOOR || MapKindSpecify_Plus1(&PlayerRightBottomMapNum, UP) == FLOOR) {
		float diff = playerRightBottom.x - (PlayerRightBottomMapNum.NumX)*(TIPSIZE);
		g_player.MovementX -= diff + 1;

	}

	//データの更新
	PlayerRightTop.x = playerRightBottom.x = g_player.WorldPos.x + PLAYERSIZEWIDHE / 2 + g_player.MovementX;
	PlayerLeftTop.x = playerLeftBottom.x = g_player.WorldPos.x - PLAYERSIZEWIDHE / 2 + g_player.MovementX;
	PlayerLeftTop.y = PlayerRightTop.y = g_player.WorldPos.y - PLAYERSIZEHEIGHT / 2 + g_player.MovementY;
	playerLeftBottom.y = playerRightBottom.y = g_player.WorldPos.y + PLAYERSIZEHEIGHT / 2 + g_player.MovementY;

	MapchipNumberSpecify(&PlayerLeftTopMapNum, &PlayerLeftTop);
	MapchipNumberSpecify(&PlayerRightTopMapNum, &PlayerRightTop);
	MapchipNumberSpecify(&PlayerRightBottomMapNum, &playerRightBottom);
	MapchipNumberSpecify(&PlayerLeftBottomMapNum, &playerLeftBottom);

	static int frcnt = 0;

	//プレイヤーの足下が床
	if (MapKindSpecify(&PlayerRightBottomMapNum) != NOTHING ||
		MapKindSpecify(&PlayerLeftBottomMapNum) != NOTHING) {

		g_player.Jumping = false;
		g_player.JumpPower = 0.0f;
		frcnt = 0;

		if (Key[KEY_C] == KEY_PUSH) {
			g_player.JumpPower = PLAYERJUMPPOWER;
			g_player.Jumping = true;
		}

		//めり込みをふせぐ
		float footdiff = (PlayerRightBottomMapNum.NumY)*(TIPSIZE)-playerRightBottom.y;
		g_player.MovementY += footdiff;
	}

	/*if (MapKindSpecify_Plus1(&PlayerRightBottomMapNum,DOWN) == FLOOR ||
	MapKindSpecify_Plus1(&PlayerLeftBottomMapNum,DOWN) == FLOOR)
	{*/

	if (g_player.Jumping) {
		frcnt++;
		if (frcnt != 0 && frcnt < 15) {
			if (Key[KEY_C] == KEY_ON) {
				g_player.JumpPower -= 0.25;
			}
		}
	}
	if (MapKindSpecify(&PlayerLeftTopMapNum) == FLOOR || MapKindSpecify_Plus1(&PlayerRightTopMapNum, LEFT) == FLOOR) {
		g_player.JumpPower = 0;
	}

	g_player.MovementY += g_player.JumpPower;
}

void MovePlayer() {

	g_player.WorldPos.x += g_player.MovementX;
	if (g_player.WindowPos.x < 300 && g_player.beLeft) {
		g_BasePoint.x += g_player.MovementX;
	}
	else if(900 < g_player.WindowPos.x && !g_player.beLeft) {
		g_BasePoint.x += g_player.MovementX;
	}
	else {
		g_player.WindowPos.x += g_player.MovementX;
	}

	g_player.WorldPos.y += g_player.MovementY;
	g_player.WindowPos.y += g_player.MovementY;

	g_player.MovementX = 0;
	g_player.MovementY = 0;
}

void PlayerReflectMotion() {

	KEYSTATE* Key = GetKey();
	Bullet* bullet = GetBullet();

	//-------------------------------------------------------------------------
	//弾をはじき返す角度
	//-------------------------------------------------------------------------
	KeyCheck(&Key[KEY_Z], DIK_Z);
	KeyCheck(&Key[KEY_X], DIK_X);

	static int frcnt = 0;

	if (g_player.bePunchUP == true || g_player.bePunchDOWN == true) {

		frcnt++;

	}
	else {
		if (Key[KEY_Z] == KEY_PUSH)
		{
			g_player.bePunchDOWN = true;

		}
		if (Key[KEY_X] == KEY_PUSH) {

			g_player.bePunchUP = true;
		}
	}
	for (int i = 0; i < BULLETNUMBER; i++) {

		if (g_player.beLeft) {
			if (frcnt != 0 && frcnt < 12) {
				if (CircleHit(g_player.WindowPos.x - 10,
					g_player.WindowPos.y - 10,
					50.0f,
					(bullet + i)->WindowPos.x,
					(bullet + i)->WindowPos.y,
					(bullet + i)->Size / 2))
				{
					if ((bullet + i)->wasReflect == false) {
						(bullet + i)->wasReflect = true;
						(bullet + i)->Rad = D3DXToRadian(180.0f);

					}
				}
			}

			if (12 < frcnt && frcnt < 24) {
				if (CircleHit(g_player.WindowPos.x - 10,
					g_player.WindowPos.y,
					50.0f,
					(bullet + i)->WindowPos.x,
					(bullet + i)->WindowPos.y,
					(bullet + i)->Size / 2))
				{
					if ((bullet + i)->wasReflect == false) {
						(bullet + i)->wasReflect = true;

						if (g_player.bePunchUP == true) {
							(bullet + i)->Rad = D3DXToRadian(150.0f);
						}
						if (g_player.bePunchDOWN == true) {
							(bullet + i)->Rad = D3DXToRadian(210.0f);
						}
					}
				}
			}

			if (24 < frcnt && frcnt < 36) {
				if (CircleHit(g_player.WindowPos.x - 10,
					g_player.WindowPos.y - 10,
					50.0f,
					(bullet + i)->WindowPos.x,
					(bullet + i)->WindowPos.y,
					(bullet + i)->Size / 2)) {
					if ((bullet + i)->wasReflect == false) {
						(bullet + i)->wasReflect = true;

						if (g_player.bePunchUP == true) {
							(bullet + i)->Rad = D3DXToRadian(120.0f);
						}
						if (g_player.bePunchDOWN == true) {
							(bullet + i)->Rad = D3DXToRadian(240.0f);
						}
					}
				}
			}
		}
		else {

			if (frcnt != 0 && frcnt < 12) {
				if (CircleHit(g_player.WindowPos.x + 10,
					g_player.WindowPos.y + 10,
					50.0f,
					(bullet + i)->WindowPos.x,
					(bullet + i)->WindowPos.y,
					(bullet + i)->Size / 2))
				{
					if ((bullet + i)->wasReflect == false) {
						(bullet + i)->wasReflect = true;
						(bullet + i)->Rad = D3DXToRadian(0.0f);

					}
				}
			}

			if (12 < frcnt && frcnt < 24) {
				if (CircleHit(g_player.WindowPos.x + 10,
					g_player.WindowPos.y,
					50.0f,
					(bullet + i)->WindowPos.x,
					(bullet + i)->WindowPos.y,
					(bullet + i)->Size / 2))
				{
					if ((bullet + i)->wasReflect == false) {
						(bullet + i)->wasReflect = true;

						if (g_player.bePunchUP == true) {
							(bullet + i)->Rad = D3DXToRadian(30.f);
						}
						if (g_player.bePunchDOWN == true) {
							(bullet + i)->Rad = D3DXToRadian(-30.f);
						}
					}
				}
			}

			if (24 < frcnt && frcnt < 36) {
				if (CircleHit(g_player.WindowPos.x + 10,
					g_player.WindowPos.y - 10,
					50.0f,
					(bullet + i)->WindowPos.x,
					(bullet + i)->WindowPos.y,
					(bullet + i)->Size / 2)) {
					if ((bullet + i)->wasReflect == false) {
						(bullet + i)->wasReflect = true;

						if (g_player.bePunchUP == true) {
							(bullet + i)->Rad = D3DXToRadian(60.f);
						}
						if (g_player.bePunchDOWN == true) {
							(bullet + i)->Rad = D3DXToRadian(-60.f);
						}
					}
				}
			}
		}
	}

	if (frcnt >= 36) {
		g_player.bePunchDOWN = false;
		g_player.bePunchUP = false;
		frcnt = 0;
	}
}

