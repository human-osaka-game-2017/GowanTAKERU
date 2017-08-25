#include"PlayerControl.h"
#include"MapRender.h"
#include"MapControl.h"
#include"CommonRender.h"
#include"CommonControl.h"
#include"MainControl.h"
#include"PlayerRender.h"
#include"BulletControl.h"
#include"MainHitManagement.h"
#include"FileManagement.h"

#define COMMON_ANIM_INTERVAL	6
#define LAND__ANIM_INTERVAL		3
#define STANDBY_ANIM_INTERVAL	30
#define TAKEOFAIR_ANIM_INTERVAL	4
#define SWING_INTERVAL			12

//プロトタイプ群
void SetPlayerMovement();
void PlayerReflectMotion();
void DecidePlayerAnimMotion();

Player g_player;
static D3DXVECTOR2 g_BasePoint = D3DXVECTOR2(DISPLAY_WIDTH / 2, DISPLAY_HEIGHT / 2);

D3DXVECTOR2* GetBasePoint() {
	return &g_BasePoint;
}

Player* GetplayerData() {
	return &g_player;
}

void PlayerInit() {

	STAGE_ID stage_ID = GetStage_ID();
	int MaxX = GetStageXYMAX(stage_ID, X);
	int MaxY = GetStageXYMAX(stage_ID, Y);
	int* map = (int*)malloc(sizeof(int)*MaxX*MaxY);
	switch (stage_ID) {
	case STAGE_1:
		CSVLoad("CSV/mainscene/stage1_gimmick.csv",map, MaxY, MaxX);
		g_player.Hp = 100;
		g_player.LifeReduced = 2;
		break;

	case STAGE_2:
		CSVLoad("CSV/mainscene/stage2_gimmick.csv", map, MaxY, MaxX);
		break;

	case STAGE_3:
		CSVLoad("CSV/mainscene/stage3_gimmick.csv", map, MaxY, MaxX);
		break;

	case STAGE_4:
		CSVLoad("CSV/mainscene/stage4_gimmick.csv", map, MaxY, MaxX);
		break;

	case STAGE_5:
		g_player.Hp = 100;
		g_player.LifeReduced = 2;
		CSVLoad("CSV/mainscene/stage5_gimmick.csv", map, MaxY, MaxX);
		break;
	}
	
	for (int i = 0; i < MaxY; i++){
		for (int j = 0; j < MaxX; j++) {
			if (*(map + (i*MaxX + j)) == STARTLOCATION) {

				MapNumXY playerstartMapNum = { j,i };
				PosSpecifyForMapchipNumber(&g_player.WorldPos, &playerstartMapNum);
				MapNumXY mapnum = { j,i-5 };
				PosSpecifyForMapchipNumber(&g_BasePoint, &mapnum);
				goto Break;
			}
		}
	}
	Break:
	free(map);
	g_BasePoint.y = 544;
	D3DXVECTOR2 BasePoint0 = D3DXVECTOR2(DISPLAY_WIDTH / 2, DISPLAY_HEIGHT / 2);

	g_player.WindowPos.x = g_player.WorldPos.x - (g_BasePoint.x - BasePoint0.x);
	g_player.WindowPos.y = g_player.WorldPos.y - (g_BasePoint.y - BasePoint0.y);
	g_player.MovementX = g_player.MovementY = 0;
	g_player.JumpPower = 0.0f;
	g_player.Jumping = false;
	g_player.beActive = true;
	g_player.beInvincible = false;
	g_player.beLeft = false;
	g_player.beDownSwing = false;
	g_player.beUPSwing = false;
}

void PlayerControl() {
	if (g_player.beActive) {

		SetPlayerMovement();

		PlayerReflectMotion();
	}

}

void SetPlayerMovement() {

	KEYSTATE* Key = GetKey();
	int* map = GetMapData();

	//常に重力をかける
	g_player.JumpPower += GRAVITY;

	//------------------------------------------------------------------------
	//左右に動かすぜ
	//------------------------------------------------------------------------
	KeyCheck(&Key[KEY_LEFT], DIK_LEFT);
	KeyCheck(&Key[KEY_RIGHT], DIK_RIGHT);
	KeyCheck(&Key[KEY_C], DIK_C);


	if (Key[KEY_LEFT] == KEY_ON) {
		if (!g_player.beDownSwing && !g_player.beUPSwing) {
			g_player.beLeft = true;
		}

		g_player.MovementX -= PLAYERMOVESPEED;

	}

	if (Key[KEY_RIGHT] == KEY_ON) {
		if (!g_player.beDownSwing && !g_player.beUPSwing) {
			g_player.beLeft = false;
		}

		g_player.MovementX += PLAYERMOVESPEED;

	}

	//プレイヤー四隅
	D3DXVECTOR2 PlayerLeftTop;
	D3DXVECTOR2 PlayerRightTop;
	D3DXVECTOR2 playerRightBottom;
	D3DXVECTOR2 playerLeftBottom;
	MapNumXY PlayerLeftTopMapNum;
	MapNumXY PlayerRightTopMapNum;
	MapNumXY PlayerRightBottomMapNum;
	MapNumXY PlayerLeftBottomMapNum;

	if (g_player.beLeft) {
		PlayerRightTop.x = playerRightBottom.x = g_player.WorldPos.x + PLAYERSIZEWIDTH / 2 - 30;
		PlayerLeftTop.x = playerLeftBottom.x = g_player.WorldPos.x - PLAYERSIZEWIDTH / 2;
	}
	else {
		PlayerRightTop.x = playerRightBottom.x = g_player.WorldPos.x + PLAYERSIZEWIDTH / 2;
		PlayerLeftTop.x = playerLeftBottom.x = g_player.WorldPos.x - PLAYERSIZEWIDTH / 2 + 30;
	}
	
	playerLeftBottom.y = playerRightBottom.y = g_player.WorldPos.y + PLAYERSIZEHEIGHT / 2 + 1;

	MapchipNumberSpecify(&PlayerRightBottomMapNum, &playerRightBottom);
	MapchipNumberSpecify(&PlayerLeftBottomMapNum, &playerLeftBottom);

	static int frcnt = 0;

	//プレイヤーの足下が床
	if (MapKindSpecify(&PlayerRightBottomMapNum) != NOTHING ||
		MapKindSpecify(&PlayerLeftBottomMapNum) != NOTHING) {
	
		g_player.JumpPower = 0.0f;
		g_player.Jumping = false;
		frcnt = 0;

		if (Key[KEY_C] == KEY_PUSH) {
			g_player.JumpPower = PLAYERJUMPPOWER;
			g_player.Jumping = true;
		}
	}

	if (g_player.Jumping) {
		frcnt++;
		if (frcnt != 0 && frcnt < 15) {
			if (Key[KEY_C] == KEY_ON) {
				g_player.JumpPower -= 0.25;
			}
		}
	}

	PlayerLeftTop.y = PlayerRightTop.y = g_player.WorldPos.y - PLAYERSIZEHEIGHT / 2 + 10 + g_player.JumpPower;
	MapchipNumberSpecify(&PlayerLeftTopMapNum, &PlayerLeftTop);
	MapchipNumberSpecify(&PlayerRightTopMapNum, &PlayerRightTop);

	if (MapKindSpecify(&PlayerLeftTopMapNum) == FLOOR || MapKindSpecify(&PlayerRightTopMapNum) != NOTHING) {
		g_player.JumpPower = 0;
	}

	g_player.MovementY += g_player.JumpPower;
}

void MovePlayer() {

	g_player.WorldPos.x += g_player.MovementX;
	if (g_player.WindowPos.x < 300 && g_player.beLeft) {
		g_BasePoint.x += g_player.MovementX;
	}
	else if(650 < g_player.WindowPos.x && !g_player.beLeft) {
		g_BasePoint.x += g_player.MovementX;
	}
	else {
		g_player.WindowPos.x += g_player.MovementX;
	}

	g_player.WorldPos.y += g_player.MovementY;

	/*if (g_player.WindowPos.y < 200 && g_player.MovementY < 0) {
		g_BasePoint.y += g_player.MovementY;
	}
	else if (400 < g_player.WindowPos.y && 0 < g_player.MovementY) {
		g_BasePoint.y += g_player.MovementY;
	}
	else {*/
		g_player.WindowPos.y += g_player.MovementY;
	

	DecidePlayerAnimMotion();

	g_player.MovementX = 0;
	g_player.MovementY = 0;
	
}

void PlayerReflectMotion() {

	KEYSTATE* Key = GetKey();

	//-------------------------------------------------------------------------
	//弾をはじき返す角度
	//-------------------------------------------------------------------------
	KeyCheck(&Key[KEY_Z], DIK_Z);
	KeyCheck(&Key[KEY_X], DIK_X);

	static int frcnt = 0;

	if (g_player.beUPSwing == true || g_player.beDownSwing == true) {

		frcnt++;

	}
	else {
		if (Key[KEY_Z] == KEY_PUSH)
		{
			g_player.beDownSwing = true;

		}
		if (Key[KEY_X] == KEY_PUSH) {

			g_player.beUPSwing = true;
		}
	}
	Bullet* pFirstBullet = GetFirstBulletAddress();
	for (Bullet* pSearchBullet = pFirstBullet->next; pSearchBullet != NULL; pSearchBullet = pSearchBullet->next){

		if (g_player.beLeft) {
			if (frcnt != 0 && frcnt < 12) {
				if (CircleHit(g_player.WindowPos.x - 30,
					g_player.WindowPos.y,
					20.0f,
					pSearchBullet->WindowPos.x,
					pSearchBullet->WindowPos.y,
					pSearchBullet->Size / 2))
				{
					if (pSearchBullet->BulletKind == NONREFLECTTARGET1 
						|| pSearchBullet->BulletKind == NONREFLECTTARGET2) {
						DeleteBullet(&pSearchBullet);
						continue;
					}

					if (pSearchBullet->wasReflect == false) {
						pSearchBullet->wasReflect = true;
						pSearchBullet->Rad = D3DXToRadian(180.0f);
						pSearchBullet->SaveCoordinate = pSearchBullet->WorldPos;
					}
				}
			}

			if (12 < frcnt && frcnt < 24) {
				if (CircleHit(g_player.WindowPos.x - 30,
					g_player.WindowPos.y - 20,
					20.0f,
					pSearchBullet->WindowPos.x,
					pSearchBullet->WindowPos.y,
					pSearchBullet->Size / 2))
				{
					if (pSearchBullet->BulletKind == NONREFLECTTARGET1
						|| pSearchBullet->BulletKind == NONREFLECTTARGET2) {
						DeleteBullet(&pSearchBullet);
						continue;
					}

					if (pSearchBullet->wasReflect == false) {
						pSearchBullet->wasReflect = true;

						if (g_player.beUPSwing == true) {
							pSearchBullet->Rad = D3DXToRadian(150.0f);
							pSearchBullet->SaveCoordinate = pSearchBullet->WorldPos;
						}
						if (g_player.beDownSwing == true) {
							pSearchBullet->Rad = D3DXToRadian(210.0f);
							pSearchBullet->SaveCoordinate = pSearchBullet->WorldPos;
						}
					}
				}
			}

			if (24 < frcnt && frcnt < 36) {
				if (CircleHit(g_player.WindowPos.x - 30,
					g_player.WindowPos.y - 50,
					20.0f,
					pSearchBullet->WindowPos.x,
					pSearchBullet->WindowPos.y,
					pSearchBullet->Size / 2)) 
				{
					if (pSearchBullet->BulletKind == NONREFLECTTARGET1
						|| pSearchBullet->BulletKind == NONREFLECTTARGET2) {
						DeleteBullet(&pSearchBullet);
						continue;
					}
					if (pSearchBullet->wasReflect == false) {
						pSearchBullet->wasReflect = true;

						if (g_player.beUPSwing == true) {
							pSearchBullet->Rad = D3DXToRadian(120.0f);
							pSearchBullet->SaveCoordinate = pSearchBullet->WorldPos;
						}
						if (g_player.beDownSwing == true) {
							pSearchBullet->Rad = D3DXToRadian(240.0f);
							pSearchBullet->SaveCoordinate = pSearchBullet->WorldPos;
						}
					}
				}
			}
		}
		else {

			if (frcnt != 0 && frcnt < 12) {
				if (CircleHit(g_player.WindowPos.x + 30,
					g_player.WindowPos.y,
					20.0f,
					pSearchBullet->WindowPos.x,
					pSearchBullet->WindowPos.y,
					pSearchBullet->Size / 2))
				{
					if (pSearchBullet->BulletKind == NONREFLECTTARGET1
						|| pSearchBullet->BulletKind == NONREFLECTTARGET2) {
						DeleteBullet(&pSearchBullet);
						continue;
					}
					if (pSearchBullet->wasReflect == false) {
						pSearchBullet->wasReflect = true;
						pSearchBullet->Rad = D3DXToRadian(0.0f);
						pSearchBullet->SaveCoordinate = pSearchBullet->WorldPos;

					}
				}
			}

			if (12 < frcnt && frcnt < 24) {
				if (CircleHit(g_player.WindowPos.x + 30,
					g_player.WindowPos.y - 20,
					20.0f,
					pSearchBullet->WindowPos.x,
					pSearchBullet->WindowPos.y,
					pSearchBullet->Size / 2))
				{
					if (pSearchBullet->BulletKind == NONREFLECTTARGET1
						|| pSearchBullet->BulletKind == NONREFLECTTARGET2) {
						DeleteBullet(&pSearchBullet);
						continue;
					}
					if (pSearchBullet->wasReflect == false) {
						pSearchBullet->wasReflect = true;

						if (g_player.beUPSwing == true) {
							pSearchBullet->Rad = D3DXToRadian(30.f);
							pSearchBullet->SaveCoordinate = pSearchBullet->WorldPos;
						}
						if (g_player.beDownSwing == true) {
							pSearchBullet->Rad = D3DXToRadian(-30.f);
							pSearchBullet->SaveCoordinate = pSearchBullet->WorldPos;
						}
					}
				}
			}

			if (24 < frcnt && frcnt < 36) {
				if (CircleHit(g_player.WindowPos.x + 30,
					g_player.WindowPos.y - 50,
					20.0f,
					pSearchBullet->WindowPos.x,
					pSearchBullet->WindowPos.y,
					pSearchBullet->Size / 2)) 
				{
					if (pSearchBullet->BulletKind == NONREFLECTTARGET1
						|| pSearchBullet->BulletKind == NONREFLECTTARGET2) {
						DeleteBullet(&pSearchBullet);
						continue;
					}

					if (pSearchBullet->wasReflect == false) {
						pSearchBullet->wasReflect = true;

						if (g_player.beUPSwing == true) {
							pSearchBullet->Rad = D3DXToRadian(60.f);
							pSearchBullet->SaveCoordinate = pSearchBullet->WorldPos;
						}
						if (g_player.beDownSwing == true) {
							pSearchBullet->Rad = D3DXToRadian(-60.f);
							pSearchBullet->SaveCoordinate = pSearchBullet->WorldPos;
						}
					}
				}
			}
		}
	}

	if (frcnt >= 36) {
		g_player.beDownSwing = false;
		g_player.beUPSwing = false;
		frcnt = 0;
	}
}

void DecidePlayerAnimMotion() {

	static PLAYERANIM oldAnimState = STANDBY1;

	static int swingfrcnt = 0;
	static int jumpfrcnt = 0;
	static int runfrcnt = 0;
	static int standbycnt = 0;
	static int land = 0;

	//swingモーション中でない
	if (swingfrcnt == 0) {

		//upswingか判定
		if (g_player.beUPSwing) {

			//yに移動量がある
			if (g_player.MovementY) {
				g_player.currentAnimState = JUMPUPSWING1;
			}

			//yに移動量がない
			else {

				//xに移動量がある
				if (g_player.MovementX) {
					g_player.currentAnimState = RUNUPSWING1;
				}

				//xに移動量がない
				else {
					g_player.currentAnimState = STANDUPSWING1;
				}
			}

			swingfrcnt++;
		}

		//downswingか判定
		else if (g_player.beDownSwing) {

			//yに移動量がある
			if (g_player.MovementY) {
				g_player.currentAnimState = JUMPDOWNSWING1;
			}

			//yに移動量がない
			else {

				//xに移動量がある
				if (g_player.MovementX) {
					g_player.currentAnimState = RUNDOWNSWING1;
				}

				//xに移動量がない
				else {
					g_player.currentAnimState = STANDDOWNSWING1;
				}
			}

			swingfrcnt++;
		}

		//yに移動量がある(jumpかどうか)
		else if (g_player.MovementY) {

			//離陸中
			if (g_player.Jumping && (0 <= jumpfrcnt && jumpfrcnt < TAKEOFAIR_ANIM_INTERVAL)) {
				g_player.currentAnimState = JUMP1;
			}

			//上昇中
			else if (g_player.MovementY < 0) {
				if ((jumpfrcnt % (COMMON_ANIM_INTERVAL * 2)) < COMMON_ANIM_INTERVAL) {
					g_player.currentAnimState = JUMP2;
				}
				else {
					g_player.currentAnimState = JUMP3;
				}
			}

			//下降中
			else {
				int tmp = jumpfrcnt % (COMMON_ANIM_INTERVAL * 3);
				if (tmp < COMMON_ANIM_INTERVAL) {
					g_player.currentAnimState = JUMP4;
				}
				else if (tmp < (COMMON_ANIM_INTERVAL * 2)) {
					g_player.currentAnimState = JUMP5;
				}
				else if (tmp < (COMMON_ANIM_INTERVAL * 3)) {
					g_player.currentAnimState = JUMP6;
				}
			}

			jumpfrcnt++;
		}
		//yに移動量がない(run or standby)
		else {

			//xに移動量がある(run)
			if (g_player.MovementX) {
				int tmp = runfrcnt % (COMMON_ANIM_INTERVAL * 8);
				if (tmp < COMMON_ANIM_INTERVAL) {
					g_player.currentAnimState = RUN1;
				}
				else if (tmp < COMMON_ANIM_INTERVAL * 2) {
					g_player.currentAnimState = RUN2;
				}
				else if (tmp < COMMON_ANIM_INTERVAL * 3) {
					g_player.currentAnimState = RUN3;
				}
				else if (tmp < COMMON_ANIM_INTERVAL * 4) {
					g_player.currentAnimState = RUN4;
				}
				else if (tmp < COMMON_ANIM_INTERVAL * 5) {
					g_player.currentAnimState = RUN5;
				}
				else if (tmp < COMMON_ANIM_INTERVAL * 6) {
					g_player.currentAnimState = RUN6;
				}
				else if (tmp < COMMON_ANIM_INTERVAL * 7) {
					g_player.currentAnimState = RUN7;
				}
				else if (tmp < COMMON_ANIM_INTERVAL * 8) {
					g_player.currentAnimState = RUN8;
				}
				runfrcnt++;
			}

			//xに移動量がない
			else {
				//g_player.currentAnimState = STANDBY1;
				int tmp = standbycnt % STANDBY_ANIM_INTERVAL * 2;
				if (tmp < STANDBY_ANIM_INTERVAL) {
					g_player.currentAnimState = STANDBY1;
				}
				else {
					g_player.currentAnimState = STANDBY2;
				}
				standbycnt++;
			}

			//着陸中
			if (land < 0) {
				g_player.currentAnimState = JUMP7;
				land++;
			}
			if (oldAnimState == JUMP4 ||
				oldAnimState == JUMP5 ||
				oldAnimState == JUMP6) {
				g_player.currentAnimState = JUMP7;
				land = -LAND__ANIM_INTERVAL;
			}
		}
	}

	//swingモーション中である
	else {
		if ((swingfrcnt % COMMON_ANIM_INTERVAL) == (COMMON_ANIM_INTERVAL - 1)) {
			g_player.currentAnimState++;
		}
		swingfrcnt++;
	}

	//リセット処理
	//swingcntのリセット
	if (!(g_player.beUPSwing) && !(g_player.beDownSwing)) {
		swingfrcnt = 0;
	}
	//standby中
	if (g_player.currentAnimState <= STANDBY2) {
		swingfrcnt = 0;
		jumpfrcnt = 0;
		runfrcnt = 0;
		land = 0;
	}
	//jump中
	else if (g_player.currentAnimState <= JUMP7) {
		swingfrcnt = 0;
		runfrcnt = 0;
		standbycnt = 0;
	}
	//run中
	else if (g_player.currentAnimState <= RUN8) {
		swingfrcnt = 0;
		jumpfrcnt = 0;
		standbycnt = 0;
		land = 0;
	}
	//swing中
	else if (g_player.currentAnimState <= RUNUPSWING6) {
		jumpfrcnt = 0;
		runfrcnt = 0;
		standbycnt = 0;
		land = 0;
	}

	oldAnimState = (PLAYERANIM)g_player.currentAnimState;

	
}
