#include"Boss3Control.h"
#include"StageSelect.h"
#include"FileManagement.h"
#include"MapRender.h"
#include"BulletControl.h"
#include"PlayerControl.h"
#include"CommonRender.h"
#include"DirectXSound.h"
#include"BulletControl.h"
#include"MainControl.h"

Boss3Data g_Boss3;
static int g_Frcnt = 0;
static int g_shotCnt = 0;


Boss3Data* GetBoss3Data() {
	return &g_Boss3;
}

void Boss3Init() {

	g_Boss3.isExistence = false;

	STAGE_ID stage_ID = GetStage_ID();
	int MaxX = GetStageXYMAX(stage_ID, X);
	int MaxY = GetStageXYMAX(stage_ID, Y);
	int* gimmickData = (int*)malloc(sizeof(int)*MaxX*MaxY);

	switch (stage_ID) {
	case STAGE_3:

		CSVLoad("CSV/mainscene/stage3_gimmick.csv", gimmickData, MaxY, MaxX);//CSV呼び出し

		for (int i = 0; i < MaxY; i++) {
			for (int j = 0; j < MaxX; j++) {
				if (gimmickData[j + i*MaxX] == BOSS_STAGE3) {
					g_Boss3.isExistence = true;
					g_Boss3.WolrdPos.x = TIPSIZE*j;
					g_Boss3.WolrdPos.y = TIPSIZE*i;
					goto BREAK;
				}
			}
		}
		break;

	case STAGE_5:

		CSVLoad("CSV/mainscene/stage5_gimmick.csv", gimmickData, MaxY, MaxX);//CSV呼び出し

		for (int i = 0; i < MaxY; i++) {
			for (int j = 0; j < MaxX; j++) {
				if (gimmickData[j + i*MaxX] == BOSS_STAGE3) {
					g_Boss3.isExistence = true;
					g_Boss3.WolrdPos.x = TIPSIZE*j;
					g_Boss3.WolrdPos.y = TIPSIZE*i;
					goto BREAK;
				}
			}
		}
		break;
	}

BREAK:
	if (g_Boss3.isExistence) {
		SetStopScrollPos(g_Boss3.WolrdPos.x);
		g_Boss3.Boss3State = BOSS3_NON;
		g_Boss3.MovementX = g_Boss3.MovementY = 0;
		g_Boss3.Hp = 100;
		g_Boss3.Atk = 20;
		//g_Boss1.isJumping = false;
		g_Boss3.isLeft = true;
		g_Boss3.isDead = false;
		g_Boss3.isActive = false;
		g_Boss3.goNextStage = false;

		g_Frcnt = 0;
		g_shotCnt = 0;
	}
	else {
		g_Boss3.isDead = false;
		g_Boss3.isActive = false;
		g_Boss3.goNextStage = false;
	}

	free(gimmickData);
}

void Boss3Control() {

	if (g_Boss3.isExistence) {

			//活動状態かチェック
			D3DXVECTOR2 BasePoint0 = D3DXVECTOR2(DISPLAY_WIDTH / 2, DISPLAY_HEIGHT / 2);
			D3DXVECTOR2* basePoint = GetBasePoint();
			g_Boss3.WindowPos.x = g_Boss3.WolrdPos.x - (basePoint->x - BasePoint0.x);
			g_Boss3.WindowPos.y = g_Boss3.WolrdPos.y - (basePoint->y - BasePoint0.y);
			if (-(2 * TIPSIZE) < g_Boss3.WindowPos.x && g_Boss3.WindowPos.x < DISPLAY_WIDTH + (2 * TIPSIZE) &&
				-(2 * TIPSIZE) < g_Boss3.WindowPos.y && g_Boss3.WindowPos.y < DISPLAY_HEIGHT + (2 * TIPSIZE)) {
				g_Boss3.isActive = true;
			}
			else {
				g_Boss3.isActive = false;
			}

			if (g_Boss3.isActive) {

				if (!g_Boss3.isDead) {

				static int shotInterval = 78.0f *  (Random(70, 130) / 100.0f);

				if (g_Frcnt == shotInterval) {

					shotInterval = 78.0f*(Random(70, 130) / 100.0f);

					//発射位置を決める
					D3DXVECTOR2 tmpPos;
					tmpPos = g_Boss3.WolrdPos;

					//弾を打つ
					if (g_shotCnt % 10 == 9) {
						if (g_Boss3.isLeft) {
							tmpPos.x += 32.0f;
							tmpPos.y += -20.0f;
						}
						else {
							tmpPos.x += -32.0f;
							tmpPos.y += -20.0f;
						}
						BulletCreate(tmpPos, NONREFLECTTARGET2);
					}
					else {
						if (g_Boss3.isLeft) {
							tmpPos.x += 32.0f;
							tmpPos.y += -20.0f;
							if (g_shotCnt % 3 == 0) {
								BulletCreate(tmpPos, BULLETNORMAL1,164.8f);
							}
							else if(g_shotCnt % 3 == 1){
								BulletCreate(tmpPos, BULLETNORMAL1,120.6f);
							}
							else {
								BulletCreate(tmpPos, BULLETTARGET3);
							}
						}
						else {
							tmpPos.x += -32.0f;
							tmpPos.y += -20.0f;
							if (g_shotCnt % 3 == 0) {
								BulletCreate(tmpPos, BULLETNORMAL1,59.4f);
							}
							else if(g_shotCnt % 3 == 1){
								BulletCreate(tmpPos, BULLETNORMAL1,15.2f);
							}
							else {
								BulletCreate(tmpPos, BULLETTARGET3);
							}
						}
					}

					g_shotCnt++;
					g_Frcnt = 0;
				}

				Player* pPlayer = GetplayerData();
				if (pPlayer->WorldPos.x < g_Boss3.WolrdPos.x) {
					g_Boss3.isLeft = true;
					g_Boss3.MovementX -= BOSS3MAXSPEED;
				}
				else {
					g_Boss3.isLeft = false;
					g_Boss3.MovementX += BOSS3MAXSPEED;
				}

				g_Boss3.MovementY = 20;
				g_Frcnt++;
			}
			else {
				static int frcnt = 0;
				if (frcnt == 0) {
					StopSound(MAINSCENE_BOSSBGM01);
					DeleteALLBullet();
				}
				if (frcnt == 120) {
					PlayBackSound(MAINSCENE_SE_FANFARE, false, 0);
				}
				if (frcnt == 300) {
					frcnt = 0;
					g_Boss3.goNextStage = true;
				}
				frcnt++;
			}
		}
	}
}

void MoveBoss3() {
	if (g_Boss3.isExistence && !g_Boss3.isDead) {
		D3DXVECTOR2 BasePoint0 = D3DXVECTOR2(DISPLAY_WIDTH / 2, DISPLAY_HEIGHT / 2);
		D3DXVECTOR2* basePoint = GetBasePoint();
		g_Boss3.WolrdPos.x += g_Boss3.MovementX;
		g_Boss3.WolrdPos.y += g_Boss3.MovementY;
		g_Boss3.WindowPos.x = g_Boss3.WolrdPos.x - (basePoint->x - BasePoint0.x);
		g_Boss3.WindowPos.y = g_Boss3.WolrdPos.y - (basePoint->y - BasePoint0.y);
		g_Boss3.MovementX = g_Boss3.MovementY = 0;
	}
}