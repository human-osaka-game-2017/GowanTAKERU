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

//プロトタイプ群
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
		g_player.LifeReduced++;
		break;

	case STAGE_3:
		CSVLoad("CSV/mainscene/stage3_gimmick.csv", map, MaxY, MaxX);
		g_player.LifeReduced++;
		break;

	case STAGE_4:
		CSVLoad("CSV/mainscene/stage4_gimmick.csv", map, MaxY, MaxX);
		g_player.LifeReduced++;
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
	g_BasePoint.y = 539;
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

	//常に重力をかける
	g_player.JumpPower += GRAVITY;

	//------------------------------------------------------------------------
	//左右に動かすぜ
	//------------------------------------------------------------------------
	KeyCheck(&Key[KEY_LEFT], DIK_LEFT);
	KeyCheck(&Key[KEY_RIGHT], DIK_RIGHT);
	KeyCheck(&Key[KEY_C], DIK_C);
	KeyCheck(&Key[KEY_UP], DIK_UP);
	GamePadCheckButton(GAMEPAD_DANALOG_LEFT, XINPUT_GAMEPAD_DPAD_LEFT);
	GamePadCheckButton(GAMEPAD_DANALOG_RIGHT, XINPUT_GAMEPAD_DPAD_RIGHT);
	GamePadCheckButton(GAMEPAD_DANALOG_UP, XINPUT_GAMEPAD_DPAD_UP);
	GamePadCheckButton(GAMEPAD_A, XINPUT_GAMEPAD_A);

	if (Key[KEY_LEFT] == KEY_ON || GetButtonState(GAMEPAD_DANALOG_LEFT) == PAD_ON) {
		if (!g_player.beDownSwing && !g_player.beUPSwing) {
			g_player.beLeft = true;
		}

		g_player.MovementX -= PLAYERMOVESPEED;

	}

	if (Key[KEY_RIGHT] == KEY_ON || GetButtonState(GAMEPAD_DANALOG_RIGHT) == PAD_ON) {
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

	//プレイヤーの足下が床
	if (MapKindSpecify(&PlayerRightBottomMapNum) != NOTHING ||
		MapKindSpecify(&PlayerLeftBottomMapNum) != NOTHING ||
		MapKindSpecify(&playerCenterBottomMapNum) != NOTHING) {
	
		g_player.JumpPower = 0.0f;
		g_player.Jumping = false;
		frcnt = 0;

		if (Key[KEY_C] == KEY_PUSH || Key[KEY_UP] == KEY_PUSH || 
			GetButtonState(GAMEPAD_DANALOG_UP) == PAD_PUSH || GetButtonState(GAMEPAD_A) == PAD_PUSH) {
			g_player.JumpPower = PLAYERJUMPPOWER;
			g_player.Jumping = true;
			PlayBackSound(MAINSCENE_SE_JUMP, false, 10);
		}
	}

	if (g_player.Jumping) {
		frcnt++;
		if (frcnt != 0 && frcnt < 15) {
			if (Key[KEY_C] == KEY_ON || Key[KEY_UP] == KEY_ON ||
				GetButtonState(GAMEPAD_DANALOG_UP) == PAD_ON || GetButtonState(GAMEPAD_A) == PAD_ON) {
				g_player.JumpPower -= 0.25;
			}
		}
	}

	//天井に当たった時
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

			//一番左の固定
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
	//弾をはじき返す角度
	//-------------------------------------------------------------------------
	KeyCheck(&Key[KEY_Z], DIK_Z);
	KeyCheck(&Key[KEY_X], DIK_X);
	GamePadCheckButton(GAMEANALOG_LEFT_SHOULDER, XINPUT_GAMEPAD_LEFT_SHOULDER);
	GamePadCheckButton(GAMEANALOG_RIGHT_SHOULDER, XINPUT_GAMEPAD_RIGHT_SHOULDER);
	GamePadCheckButton(GAMEANALOG_LEFT_THUMB, XINPUT_GAMEPAD_LEFT_THUMB);
	GamePadCheckButton(GAMEANALOG_RIGHT_THUMB, XINPUT_GAMEPAD_RIGHT_THUMB);
	GamePadCheckButton(GAMEPAD_Y, XINPUT_GAMEPAD_Y);
	GamePadCheckButton(GAMEPAD_X, XINPUT_GAMEPAD_X);

	static int frcnt = 0;

	if (g_player.beUPSwing == true || g_player.beDownSwing == true) {

		frcnt++;

	}
	else {
		if (Key[KEY_Z] == KEY_PUSH ||
			GetButtonState(GAMEANALOG_LEFT_SHOULDER) == PAD_PUSH ||
			GetButtonState(GAMEANALOG_LEFT_THUMB) == PAD_PUSH ||
			GetButtonState(GAMEPAD_Y) == PAD_PUSH)
		{
			g_player.beDownSwing = true;
			PlayBackSound(MAINSCENE_SE_SWING, false, 10);

		}
		if (Key[KEY_X] == KEY_PUSH ||
			GetButtonState(GAMEANALOG_RIGHT_SHOULDER) == PAD_PUSH ||
			GetButtonState(GAMEANALOG_RIGHT_THUMB) == PAD_PUSH ||
			GetButtonState(GAMEPAD_X) == PAD_PUSH) {

			g_player.beUPSwing = true;
			PlayBackSound(MAINSCENE_SE_SWING, false, 10);
		}
	}

	float armRadius = 25.0f;

	Bullet* pFirstBullet = GetFirstBulletAddress();
	EditableBulletData* BulletInitializeData = GetEditableBulletData();

	for (Bullet* pSearchBullet = pFirstBullet->next; pSearchBullet != NULL; pSearchBullet = pSearchBullet->next) {

		int bulletKind = pFirstBullet->BulletKind;

		//弾ける弾かどうか
		if (!pSearchBullet->wasReflect) {

			//アップスウィング中
			if (g_player.beUPSwing) {

				//左向き中
				if (g_player.beLeft) {

					if (frcnt < SWING_INTERVAL) {

						//あたり判定
						if (CircleHit(g_player.WindowPos.x - 27.0f - 14.0f,
							g_player.WindowPos.y + 10,
							armRadius,
							pSearchBullet->WindowPos.x,
							pSearchBullet->WindowPos.y,
							BulletInitializeData[bulletKind].Size / 2)) {

							//弾けない弾の判定
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

						//あたり判定
						if (CircleHit(g_player.WindowPos.x - 30.0f - 14.0f,
							g_player.WindowPos.y - 20,
							armRadius,
							pSearchBullet->WindowPos.x,
							pSearchBullet->WindowPos.y,
							BulletInitializeData[bulletKind].Size / 2)) {

							//弾けない弾の判定
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

						//あたり判定
						if (CircleHit(g_player.WindowPos.x - 27 - 14.0f,
							g_player.WindowPos.y - 50,
							armRadius,
							pSearchBullet->WindowPos.x,
							pSearchBullet->WindowPos.y,
							BulletInitializeData[bulletKind].Size / 2)) {

							//弾けない弾の判定
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

				//右向き中
				else {
					if (frcnt < SWING_INTERVAL) {

						//あたり判定
						if (CircleHit(g_player.WindowPos.x + 27.0f + 14.0f,
							g_player.WindowPos.y + 10,
							armRadius,
							pSearchBullet->WindowPos.x,
							pSearchBullet->WindowPos.y,
							BulletInitializeData[bulletKind].Size / 2)) {

							//弾けない弾の判定
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

						//あたり判定
						if (CircleHit(g_player.WindowPos.x + 30.0f + 14.0f,
							g_player.WindowPos.y - 20,
							armRadius,
							pSearchBullet->WindowPos.x,
							pSearchBullet->WindowPos.y,
							BulletInitializeData[bulletKind].Size / 2)) {

							//弾けない弾の判定
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

						//あたり判定
						if (CircleHit(g_player.WindowPos.x + 27.0f +14.0f,
							g_player.WindowPos.y - 50,
							armRadius,
							pSearchBullet->WindowPos.x,
							pSearchBullet->WindowPos.y,
							BulletInitializeData[bulletKind].Size / 2)) {

							//弾けない弾の判定
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

			//downswing中
			else if (g_player.beDownSwing) {

				//左向き中
				if (g_player.beLeft) {

					if (frcnt < SWING_INTERVAL) {

						//あたり判定
						if (CircleHit(g_player.WindowPos.x - 27.0f - 14.0f,
							g_player.WindowPos.y - 50,
							armRadius,
							pSearchBullet->WindowPos.x,
							pSearchBullet->WindowPos.y,
							BulletInitializeData[bulletKind].Size / 2)) {

							//弾けない弾の判定
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

						//あたり判定
						if (CircleHit(g_player.WindowPos.x - 27.0f - 14.0f,
							g_player.WindowPos.y - 20,
							armRadius,
							pSearchBullet->WindowPos.x,
							pSearchBullet->WindowPos.y,
							BulletInitializeData[bulletKind].Size / 2)) {

							//弾けない弾の判定
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

						//あたり判定
						if (CircleHit(g_player.WindowPos.x - 27.0f - 14.0f,
							g_player.WindowPos.y + 10,
							armRadius,
							pSearchBullet->WindowPos.x,
							pSearchBullet->WindowPos.y,
							BulletInitializeData[bulletKind].Size / 2)) {

							//弾けない弾の判定
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

				//右向き中
				else {
					if (frcnt < SWING_INTERVAL) {

						//あたり判定
						if (CircleHit(g_player.WindowPos.x + 27.0f +14.0f,
							g_player.WindowPos.y - 50,
							armRadius,
							pSearchBullet->WindowPos.x,
							pSearchBullet->WindowPos.y,
							BulletInitializeData[bulletKind].Size / 2)) {

							//弾けない弾の判定
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

						//あたり判定
						if (CircleHit(g_player.WindowPos.x + 27.0f + 14.0f,
							g_player.WindowPos.y - 20,
							armRadius,
							pSearchBullet->WindowPos.x,
							pSearchBullet->WindowPos.y,
							BulletInitializeData[bulletKind].Size / 2)) {

							//弾けない弾の判定
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

						//あたり判定
						if (CircleHit(g_player.WindowPos.x + 27.0f + 14.0f,
							g_player.WindowPos.y + 10,
							armRadius,
							pSearchBullet->WindowPos.x,
							pSearchBullet->WindowPos.y,
							BulletInitializeData[bulletKind].Size / 2)) {

							//弾けない弾の判定
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
