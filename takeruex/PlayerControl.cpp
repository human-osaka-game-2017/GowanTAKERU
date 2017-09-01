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
#include"DirectXSound.h"
#include"StageGimmick.h"

#define COMMON_ANIM_INTERVAL	6
#define LAND__ANIM_INTERVAL		3
#define STANDBY_ANIM_INTERVAL	30
#define TAKEOFAIR_ANIM_INTERVAL	4
#define SWING_INTERVAL			12

//ƒvƒƒgƒ^ƒCƒvŒQ
void SetPlayerMovement();
void PlayerReflectMotion();
void DecidePlayerAnimMotion();

Player g_player;
static D3DXVECTOR2 g_BasePoint = D3DXVECTOR2(DISPLAY_WIDTH / 2, DISPLAY_HEIGHT / 2);
BossScroll g_ScrollRock;

//bool GetStopScroll() {
//	return g_ScrollRock.StopScroll;
//}

void SetStopScroll(bool scroll) {
	g_ScrollRock.StopScroll = scroll;
}

void SetStopScrollPos(float scrollPosX) {
	scrollPosX -= TIPSIZE * 14;
	g_ScrollRock.StopPos = scrollPosX;
}

D3DXVECTOR2* GetBasePoint() {
	return &g_BasePoint;
}

Player* GetplayerData() {
	return &g_player;
}

void PlayerInit() {

	g_ScrollRock.StopScroll = false;

	STAGE_ID stage_ID = GetStage_ID();
	int MaxX = GetStageXYMAX(stage_ID, X);
	int MaxY = GetStageXYMAX(stage_ID, Y);
	int* map = (int*)malloc(sizeof(int)*MaxX*MaxY);
	switch (stage_ID) {
	case STAGE_1:
		CSVLoad("CSV/mainscene/stage1_gimmick.csv",map, MaxY, MaxX);
		g_player.LifeReduced = 3;
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
		g_player.LifeReduced = 3;
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

	g_player.Hp = 100;
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

	//í‚Éd—Í‚ğ‚©‚¯‚é
	g_player.JumpPower += GRAVITY;

	//------------------------------------------------------------------------
	//¶‰E‚É“®‚©‚·‚º
	//------------------------------------------------------------------------
	KeyCheck(&Key[KEY_LEFT], DIK_LEFT);
	KeyCheck(&Key[KEY_RIGHT], DIK_RIGHT);
	KeyCheck(&Key[KEY_C], DIK_C);
	KeyCheck(&Key[KEY_UP], DIK_UP);


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

	//ƒvƒŒƒCƒ„[l‹÷
	D3DXVECTOR2 PlayerLeftTop;
	D3DXVECTOR2 PlayerRightTop;
	D3DXVECTOR2 playerRightBottom;
	D3DXVECTOR2 playerLeftBottom;
	D3DXVECTOR2 playerCenterTop;
	D3DXVECTOR2 playerCenterBottom;
	MapNumXY PlayerLeftTopMapNum;
	MapNumXY PlayerRightTopMapNum;
	MapNumXY PlayerRightBottomMapNum;
	MapNumXY PlayerLeftBottomMapNum;
	MapNumXY playerCenterTopMapNum;
	MapNumXY playerCenterBottomMapNum;

	PlayerRightTop.x = playerRightBottom.x = g_player.WorldPos.x + PLAYERSIZEWIDTH / 2;
	PlayerLeftTop.x = playerLeftBottom.x = g_player.WorldPos.x - PLAYERSIZEWIDTH / 2;
	playerCenterBottom.x = playerCenterTop.x = g_player.WorldPos.x;
	playerCenterBottom.y = playerLeftBottom.y = playerRightBottom.y = g_player.WorldPos.y + PLAYERSIZEHEIGHT / 2 + 1;

	MapchipNumberSpecify(&PlayerRightBottomMapNum, &playerRightBottom);
	MapchipNumberSpecify(&PlayerLeftBottomMapNum, &playerLeftBottom);
	MapchipNumberSpecify(&playerCenterBottomMapNum, &playerCenterBottom);

	static int frcnt = 0;

	//ƒvƒŒƒCƒ„[‚Ì‘«‰º‚ª°
	if (MapKindSpecify(&PlayerRightBottomMapNum) != NOTHING ||
		MapKindSpecify(&PlayerLeftBottomMapNum) != NOTHING ||
		MapKindSpecify(&playerCenterBottomMapNum) != NOTHING) {
	
		g_player.JumpPower = 0.0f;
		g_player.Jumping = false;
		frcnt = 0;

		if (Key[KEY_C] == KEY_PUSH || Key[KEY_UP] == KEY_PUSH) {
			g_player.JumpPower = PLAYERJUMPPOWER;
			g_player.Jumping = true;
			PlayBackSound(MAINSCENE_SE_JUMP, false, 10);
		}
	}

	if (g_player.Jumping) {
		frcnt++;
		if (frcnt != 0 && frcnt < 15) {
			if (Key[KEY_C] == KEY_ON || Key[KEY_UP] == KEY_ON) {
				g_player.JumpPower -= 0.25;
			}
		}
	}

	//“Vˆä‚É“–‚½‚Á‚½
	playerCenterTop.y = PlayerRightTop.y = PlayerLeftTop.y = g_player.WorldPos.y - PLAYERSIZEHEIGHT / 2 + g_player.JumpPower - 1;
	MapchipNumberSpecify(&PlayerLeftTopMapNum, &PlayerLeftTop);
	MapchipNumberSpecify(&PlayerRightTopMapNum, &PlayerRightTop);
	MapchipNumberSpecify(&playerCenterTopMapNum, &playerCenterTop);

	if (MapKindSpecify(&PlayerLeftTopMapNum) != NOTHING
		|| MapKindSpecify(&PlayerRightTopMapNum) != NOTHING
		|| MapKindSpecify(&playerCenterTopMapNum) != NOTHING) {
		g_player.JumpPower = 0;
	}

	g_player.MovementY += g_player.JumpPower;
}

void MovePlayer() {

	g_player.WorldPos.x += g_player.MovementX;
	g_player.WorldPos.y += g_player.MovementY;

	if (!g_ScrollRock.StopScroll) {
		g_ScrollRock.StopScroll = CheckBossActiveBoss();
	}

	if (g_ScrollRock.StopScroll) {
		if (g_BasePoint.x <= g_ScrollRock.StopPos) {
			g_BasePoint.x += 2.5f;
		}
	}
	else {
		if (g_player.WindowPos.x < 300 && g_player.beLeft) {

			//ˆê”Ô¶‚ÌŒÅ’è
			if (!(g_BasePoint.x < DISPLAY_WIDTH / 2 + 5.0f)) {
				g_BasePoint.x += g_player.MovementX;
			}
		}
		else if (650 < g_player.WindowPos.x && !g_player.beLeft) {
			g_BasePoint.x += g_player.MovementX;
		}
	}

	D3DXVECTOR2 BasePoint0 = D3DXVECTOR2(DISPLAY_WIDTH / 2, DISPLAY_HEIGHT / 2);

	g_player.WindowPos.x = g_player.WorldPos.x - (g_BasePoint.x - BasePoint0.x);
	g_player.WindowPos.y = g_player.WorldPos.y - (g_BasePoint.y - BasePoint0.y);

	DecidePlayerAnimMotion();

	g_player.MovementX = 0;
	g_player.MovementY = 0;
	
}

void PlayerReflectMotion() {

	KEYSTATE* Key = GetKey();

	//-------------------------------------------------------------------------
	//’e‚ğ‚Í‚¶‚«•Ô‚·Šp“x
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
			PlayBackSound(MAINSCENE_SE_SWING, false, 10);

		}
		if (Key[KEY_X] == KEY_PUSH) {

			g_player.beUPSwing = true;
			PlayBackSound(MAINSCENE_SE_SWING, false, 10);
		}
	}

	float armRadius = 25.0f;

	Bullet* pFirstBullet = GetFirstBulletAddress();
	for (Bullet* pSearchBullet = pFirstBullet->next; pSearchBullet != NULL; pSearchBullet = pSearchBullet->next) {

		//’e‚¯‚é’e‚©‚Ç‚¤‚©
		if (!pSearchBullet->wasReflect) {

			//ƒAƒbƒvƒXƒEƒBƒ“ƒO’†
			if (g_player.beUPSwing) {

				//¶Œü‚«’†
				if (g_player.beLeft) {

					if (frcnt < SWING_INTERVAL) {

						//‚ ‚½‚è”»’è
						if (CircleHit(g_player.WindowPos.x - 27.0f - 14.0f,
							g_player.WindowPos.y + 10,
							armRadius,
							pSearchBullet->WindowPos.x,
							pSearchBullet->WindowPos.y,
							pSearchBullet->Size / 2)) {

							//’e‚¯‚È‚¢’e‚Ì”»’è
							if (pSearchBullet->BulletKind == NONREFLECTTARGET1
								|| pSearchBullet->BulletKind == NONREFLECTTARGET2) {
								DeleteBullet(&pSearchBullet);
								continue;
							}

							pSearchBullet->wasReflect = true;
							pSearchBullet->Rad = D3DXToRadian(180.0f);
							pSearchBullet->SaveCoordinate = pSearchBullet->WorldPos;
							PlayBackSound(MAINSCENE_SE_ATTACK, false, 10);
						}
					}
					else if (frcnt < (SWING_INTERVAL * 2)) {

						//‚ ‚½‚è”»’è
						if (CircleHit(g_player.WindowPos.x - 30.0f - 14.0f,
							g_player.WindowPos.y - 20,
							armRadius,
							pSearchBullet->WindowPos.x,
							pSearchBullet->WindowPos.y,
							pSearchBullet->Size / 2)) {

							//’e‚¯‚È‚¢’e‚Ì”»’è
							if (pSearchBullet->BulletKind == NONREFLECTTARGET1
								|| pSearchBullet->BulletKind == NONREFLECTTARGET2) {
								DeleteBullet(&pSearchBullet);
								continue;
							}

							pSearchBullet->wasReflect = true;
							pSearchBullet->Rad = D3DXToRadian(150.0f);
							pSearchBullet->SaveCoordinate = pSearchBullet->WorldPos;
							PlayBackSound(MAINSCENE_SE_ATTACK, false, 10);
						}
					}
					else if (frcnt < (SWING_INTERVAL * 3)) {

						//‚ ‚½‚è”»’è
						if (CircleHit(g_player.WindowPos.x - 27 - 14.0f,
							g_player.WindowPos.y - 50,
							armRadius,
							pSearchBullet->WindowPos.x,
							pSearchBullet->WindowPos.y,
							pSearchBullet->Size / 2)) {

							//’e‚¯‚È‚¢’e‚Ì”»’è
							if (pSearchBullet->BulletKind == NONREFLECTTARGET1
								|| pSearchBullet->BulletKind == NONREFLECTTARGET2) {
								DeleteBullet(&pSearchBullet);
								continue;
							}

							pSearchBullet->wasReflect = true;
							pSearchBullet->Rad = D3DXToRadian(120.0f);
							pSearchBullet->SaveCoordinate = pSearchBullet->WorldPos;
							PlayBackSound(MAINSCENE_SE_ATTACK, false, 10);
						}
					}
				}

				//‰EŒü‚«’†
				else {
					if (frcnt < SWING_INTERVAL) {

						//‚ ‚½‚è”»’è
						if (CircleHit(g_player.WindowPos.x + 27.0f + 14.0f,
							g_player.WindowPos.y + 10,
							armRadius,
							pSearchBullet->WindowPos.x,
							pSearchBullet->WindowPos.y,
							pSearchBullet->Size / 2)) {

							//’e‚¯‚È‚¢’e‚Ì”»’è
							if (pSearchBullet->BulletKind == NONREFLECTTARGET1
								|| pSearchBullet->BulletKind == NONREFLECTTARGET2) {
								DeleteBullet(&pSearchBullet);
								continue;
							}

							pSearchBullet->wasReflect = true;
							pSearchBullet->Rad = D3DXToRadian(0.0f);
							pSearchBullet->SaveCoordinate = pSearchBullet->WorldPos;
							PlayBackSound(MAINSCENE_SE_ATTACK, false, 10);
						}
					}
					else if (frcnt < (SWING_INTERVAL * 2)) {

						//‚ ‚½‚è”»’è
						if (CircleHit(g_player.WindowPos.x + 30.0f + 14.0f,
							g_player.WindowPos.y - 20,
							armRadius,
							pSearchBullet->WindowPos.x,
							pSearchBullet->WindowPos.y,
							pSearchBullet->Size / 2)) {

							//’e‚¯‚È‚¢’e‚Ì”»’è
							if (pSearchBullet->BulletKind == NONREFLECTTARGET1
								|| pSearchBullet->BulletKind == NONREFLECTTARGET2) {
								DeleteBullet(&pSearchBullet);
								continue;
							}

							pSearchBullet->wasReflect = true;
							pSearchBullet->Rad = D3DXToRadian(30.0f);
							pSearchBullet->SaveCoordinate = pSearchBullet->WorldPos;
							PlayBackSound(MAINSCENE_SE_ATTACK, false, 10);
						}
					}
					else if (frcnt < (SWING_INTERVAL * 3)) {

						//‚ ‚½‚è”»’è
						if (CircleHit(g_player.WindowPos.x + 27.0f +14.0f,
							g_player.WindowPos.y - 50,
							armRadius,
							pSearchBullet->WindowPos.x,
							pSearchBullet->WindowPos.y,
							pSearchBullet->Size / 2)) {

							//’e‚¯‚È‚¢’e‚Ì”»’è
							if (pSearchBullet->BulletKind == NONREFLECTTARGET1
								|| pSearchBullet->BulletKind == NONREFLECTTARGET2) {
								DeleteBullet(&pSearchBullet);
								continue;
							}

							pSearchBullet->wasReflect = true;
							pSearchBullet->Rad = D3DXToRadian(60.0f);
							pSearchBullet->SaveCoordinate = pSearchBullet->WorldPos;
							PlayBackSound(MAINSCENE_SE_ATTACK, false, 10);
						}
					}
				}
			}

			//downswing’†
			else if (g_player.beDownSwing) {

				//¶Œü‚«’†
				if (g_player.beLeft) {

					if (frcnt < SWING_INTERVAL) {

						//‚ ‚½‚è”»’è
						if (CircleHit(g_player.WindowPos.x - 27.0f - 14.0f,
							g_player.WindowPos.y - 50,
							armRadius,
							pSearchBullet->WindowPos.x,
							pSearchBullet->WindowPos.y,
							pSearchBullet->Size / 2)) {

							//’e‚¯‚È‚¢’e‚Ì”»’è
							if (pSearchBullet->BulletKind == NONREFLECTTARGET1
								|| pSearchBullet->BulletKind == NONREFLECTTARGET2) {
								DeleteBullet(&pSearchBullet);
								continue;
							}

							pSearchBullet->wasReflect = true;
							pSearchBullet->Rad = D3DXToRadian(180.0f);
							pSearchBullet->SaveCoordinate = pSearchBullet->WorldPos;
							PlayBackSound(MAINSCENE_SE_ATTACK, false, 10);
						}
					}
					else if (frcnt < (SWING_INTERVAL * 2)) {

						//‚ ‚½‚è”»’è
						if (CircleHit(g_player.WindowPos.x - 27.0f - 14.0f,
							g_player.WindowPos.y - 20,
							armRadius,
							pSearchBullet->WindowPos.x,
							pSearchBullet->WindowPos.y,
							pSearchBullet->Size / 2)) {

							//’e‚¯‚È‚¢’e‚Ì”»’è
							if (pSearchBullet->BulletKind == NONREFLECTTARGET1
								|| pSearchBullet->BulletKind == NONREFLECTTARGET2) {
								DeleteBullet(&pSearchBullet);
								continue;
							}

							pSearchBullet->wasReflect = true;
							pSearchBullet->Rad = D3DXToRadian(210.0f);
							pSearchBullet->SaveCoordinate = pSearchBullet->WorldPos;
							PlayBackSound(MAINSCENE_SE_ATTACK, false, 10);
						}
					}
					else if (frcnt < (SWING_INTERVAL * 3)) {

						//‚ ‚½‚è”»’è
						if (CircleHit(g_player.WindowPos.x - 27.0f - 14.0f,
							g_player.WindowPos.y + 10,
							armRadius,
							pSearchBullet->WindowPos.x,
							pSearchBullet->WindowPos.y,
							pSearchBullet->Size / 2)) {

							//’e‚¯‚È‚¢’e‚Ì”»’è
							if (pSearchBullet->BulletKind == NONREFLECTTARGET1
								|| pSearchBullet->BulletKind == NONREFLECTTARGET2) {
								DeleteBullet(&pSearchBullet);
								continue;
							}

							pSearchBullet->wasReflect = true;
							pSearchBullet->Rad = D3DXToRadian(240.0f);
							pSearchBullet->SaveCoordinate = pSearchBullet->WorldPos;
							PlayBackSound(MAINSCENE_SE_ATTACK, false, 10);
						}
					}
				}

				//‰EŒü‚«’†
				else {
					if (frcnt < SWING_INTERVAL) {

						//‚ ‚½‚è”»’è
						if (CircleHit(g_player.WindowPos.x + 27.0f +14.0f,
							g_player.WindowPos.y - 50,
							armRadius,
							pSearchBullet->WindowPos.x,
							pSearchBullet->WindowPos.y,
							pSearchBullet->Size / 2)) {

							//’e‚¯‚È‚¢’e‚Ì”»’è
							if (pSearchBullet->BulletKind == NONREFLECTTARGET1
								|| pSearchBullet->BulletKind == NONREFLECTTARGET2) {
								DeleteBullet(&pSearchBullet);
								continue;
							}

							pSearchBullet->wasReflect = true;
							pSearchBullet->Rad = D3DXToRadian(0.0f);
							pSearchBullet->SaveCoordinate = pSearchBullet->WorldPos;
							PlayBackSound(MAINSCENE_SE_ATTACK, false, 10);
						}
					}
					else if (frcnt < (SWING_INTERVAL * 2)) {

						//‚ ‚½‚è”»’è
						if (CircleHit(g_player.WindowPos.x + 27.0f + 14.0f,
							g_player.WindowPos.y - 20,
							armRadius,
							pSearchBullet->WindowPos.x,
							pSearchBullet->WindowPos.y,
							pSearchBullet->Size / 2)) {

							//’e‚¯‚È‚¢’e‚Ì”»’è
							if (pSearchBullet->BulletKind == NONREFLECTTARGET1
								|| pSearchBullet->BulletKind == NONREFLECTTARGET2) {
								DeleteBullet(&pSearchBullet);
								continue;
							}

							pSearchBullet->wasReflect = true;
							pSearchBullet->Rad = D3DXToRadian(330.0f);
							pSearchBullet->SaveCoordinate = pSearchBullet->WorldPos;
							PlayBackSound(MAINSCENE_SE_ATTACK, false, 10);
						}
					}
					else if (frcnt < (SWING_INTERVAL * 3)) {

						//‚ ‚½‚è”»’è
						if (CircleHit(g_player.WindowPos.x + 27.0f + 14.0f,
							g_player.WindowPos.y + 10,
							armRadius,
							pSearchBullet->WindowPos.x,
							pSearchBullet->WindowPos.y,
							pSearchBullet->Size / 2)) {

							//’e‚¯‚È‚¢’e‚Ì”»’è
							if (pSearchBullet->BulletKind == NONREFLECTTARGET1
								|| pSearchBullet->BulletKind == NONREFLECTTARGET2) {
								DeleteBullet(&pSearchBullet);
								continue;
							}

							pSearchBullet->wasReflect = true;
							pSearchBullet->Rad = D3DXToRadian(300.0f);
							pSearchBullet->SaveCoordinate = pSearchBullet->WorldPos;
							PlayBackSound(MAINSCENE_SE_ATTACK, false, 10);
						}
					}
				}
			}
		}
	}

	if (frcnt == 36) {
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

	//swingƒ‚[ƒVƒ‡ƒ“’†‚Å‚È‚¢
	if (swingfrcnt == 0) {

		//upswing‚©”»’è
		if (g_player.beUPSwing) {

			//y‚ÉˆÚ“®—Ê‚ª‚ ‚é
			if (g_player.MovementY) {
				g_player.currentAnimState = JUMPUPSWING1;
			}

			//y‚ÉˆÚ“®—Ê‚ª‚È‚¢
			else {

				//x‚ÉˆÚ“®—Ê‚ª‚ ‚é
				if (g_player.MovementX) {
					g_player.currentAnimState = RUNUPSWING1;
				}

				//x‚ÉˆÚ“®—Ê‚ª‚È‚¢
				else {
					g_player.currentAnimState = STANDUPSWING1;
				}
			}

			swingfrcnt++;
		}

		//downswing‚©”»’è
		else if (g_player.beDownSwing) {

			//y‚ÉˆÚ“®—Ê‚ª‚ ‚é
			if (g_player.MovementY) {
				g_player.currentAnimState = JUMPDOWNSWING1;
			}

			//y‚ÉˆÚ“®—Ê‚ª‚È‚¢
			else {

				//x‚ÉˆÚ“®—Ê‚ª‚ ‚é
				if (g_player.MovementX) {
					g_player.currentAnimState = RUNDOWNSWING1;
				}

				//x‚ÉˆÚ“®—Ê‚ª‚È‚¢
				else {
					g_player.currentAnimState = STANDDOWNSWING1;
				}
			}

			swingfrcnt++;
		}

		//y‚ÉˆÚ“®—Ê‚ª‚ ‚é(jump‚©‚Ç‚¤‚©)
		else if (g_player.MovementY) {

			//—£—¤’†
			if (g_player.Jumping && (0 <= jumpfrcnt && jumpfrcnt < TAKEOFAIR_ANIM_INTERVAL)) {
				g_player.currentAnimState = JUMP1;
			}

			//ã¸’†
			else if (g_player.MovementY < 0) {
				if ((jumpfrcnt % (COMMON_ANIM_INTERVAL * 2)) < COMMON_ANIM_INTERVAL) {
					g_player.currentAnimState = JUMP2;
				}
				else {
					g_player.currentAnimState = JUMP3;
				}
			}

			//‰º~’†
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
		//y‚ÉˆÚ“®—Ê‚ª‚È‚¢(run or standby)
		else {

			//x‚ÉˆÚ“®—Ê‚ª‚ ‚é(run)
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

			//x‚ÉˆÚ“®—Ê‚ª‚È‚¢
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

			//’…—¤’†
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

	//swingƒ‚[ƒVƒ‡ƒ“’†‚Å‚ ‚é
	else {
		if ((swingfrcnt % COMMON_ANIM_INTERVAL) == (COMMON_ANIM_INTERVAL - 1)) {
			g_player.currentAnimState++;
		}
		swingfrcnt++;
	}

	//ƒŠƒZƒbƒgˆ—
	//swingcnt‚ÌƒŠƒZƒbƒg
	if (!(g_player.beUPSwing) && !(g_player.beDownSwing)) {
		swingfrcnt = 0;
	}
	//standby’†
	if (g_player.currentAnimState <= STANDBY2) {
		swingfrcnt = 0;
		jumpfrcnt = 0;
		runfrcnt = 0;
		land = 0;
	}
	//jump’†
	else if (g_player.currentAnimState <= JUMP7) {
		swingfrcnt = 0;
		runfrcnt = 0;
		standbycnt = 0;
	}
	//run’†
	else if (g_player.currentAnimState <= RUN8) {
		swingfrcnt = 0;
		jumpfrcnt = 0;
		standbycnt = 0;
		land = 0;
	}
	//swing’†
	else if (g_player.currentAnimState <= RUNUPSWING6) {
		jumpfrcnt = 0;
		runfrcnt = 0;
		standbycnt = 0;
		land = 0;
	}

	oldAnimState = (PLAYERANIM)g_player.currentAnimState;

	
}
