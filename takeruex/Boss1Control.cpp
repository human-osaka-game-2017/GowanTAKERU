#include"Boss1Control.h"
#include"FileManagement.h"
#include"MapRender.h"
#include"PlayerControl.h"
#include"CommonRender.h"
#include"MainControl.h"
#include"BulletControl.h"
#include"MapControl.h"
#include"DirectXSound.h"

#define DUALSHOTINTERVALS 20
#define MAXHP 80.0f

//プロトタイプ群
BOSS1STATE DecidedBoss1State(float range, int bulletNum);
float CalculateNORMALSHOTDecidedValue(int bulletNum, float range);
float CalculateDUALSHOTDecidedValue(int bulletNum, float range);
float CalculateLARIATDecidedValue(float range);

Boss1Data g_Boss1;
static int g_frcnt = 0;

Boss1Data* GetBoss1Data() {
	return &g_Boss1;
}

void Boss1Init() {

	g_Boss1.isExistence = false;

	STAGE_ID stage_ID = GetStage_ID();
	int MaxX = GetStageXYMAX(stage_ID, X);
	int MaxY = GetStageXYMAX(stage_ID, Y);
	int* gimmickData = (int*)malloc(sizeof(int)*MaxX*MaxY);

	switch (stage_ID) {
	case STAGE_1:

		CSVLoad("CSV/mainscene/stage1_gimmick.csv", gimmickData, MaxY, MaxX);//CSV呼び出し

		for (int i = 0; i < MaxY; i++) {
			for (int j = 0; j < MaxX; j++) {
				if (gimmickData[j + i*MaxX] == BOSS_STAGE1) {
					g_Boss1.isExistence = true;
					g_Boss1.WolrdPos.x = TIPSIZE*j;
					g_Boss1.WolrdPos.y = TIPSIZE*i;

					goto BREAK;
				}
			}
		}
		break;

	case STAGE_5:

		CSVLoad("CSV/mainscene/stage5_gimmick.csv", gimmickData, MaxY, MaxX);//CSV呼び出し

		for (int i = 0; i < MaxY; i++) {
			for (int j = 0; j < MaxX; j++) {
				if (gimmickData[j + i*MaxX] == BOSS_STAGE1) {
					g_Boss1.isExistence = true;
					g_Boss1.WolrdPos.x = TIPSIZE*j;
					g_Boss1.WolrdPos.y = TIPSIZE*i;
					goto BREAK;
				}
			}
		}
		break;
	}

	

BREAK:
	if (g_Boss1.isExistence) {
		g_frcnt = 0;
		SetStopScrollPos(g_Boss1.WolrdPos.x);
		g_Boss1.Boss1State = BOSS1_NON;
		g_Boss1.MovementX = g_Boss1.MovementY = 0;
		g_Boss1.a = 0;//g_Boss1.ga = 0;
		g_Boss1.Hp = MAXHP;
		g_Boss1.Atk = 20;
		g_Boss1.saveShotFrmcnt = 0;
		g_Boss1.saveActionCntForNORMALSHOT = 0;
		g_Boss1.saveActionCntForDUALSHOT = 0;
		g_Boss1.saveActionCntForLARIAT = 0;
		//g_Boss1.isJumping = false;
		g_Boss1.isLeft = true;
		g_Boss1.isDead = false;
		g_Boss1.isActive = false;
		g_Boss1.goNextStage = false;
	}
	else {
		g_Boss1.isDead = false;
		g_Boss1.isActive = false;
		g_Boss1.goNextStage = false;
	}

	free(gimmickData);
}


void Boss1Control() {
	if (g_Boss1.isExistence) {

		//活動状態かチェック
		D3DXVECTOR2 BasePoint0 = D3DXVECTOR2(DISPLAY_WIDTH / 2, DISPLAY_HEIGHT / 2);
		D3DXVECTOR2* basePoint = GetBasePoint();
		g_Boss1.WindowPos.x = g_Boss1.WolrdPos.x - (basePoint->x - BasePoint0.x);
		g_Boss1.WindowPos.y = g_Boss1.WolrdPos.y - (basePoint->y - BasePoint0.y);
		if (-(2 * TIPSIZE) < g_Boss1.WindowPos.x && g_Boss1.WindowPos.x < DISPLAY_WIDTH + (2 * TIPSIZE) &&
			-(2 * TIPSIZE) < g_Boss1.WindowPos.y && g_Boss1.WindowPos.y < DISPLAY_HEIGHT + (2 * TIPSIZE)) {
			g_Boss1.isActive = true;
		}
		else {
			g_Boss1.isActive = false;
		}

		//活動状態である
		if (g_Boss1.isActive) {

			if (!g_Boss1.isDead) {

				//知覚する
				Player* pPlayer = GetplayerData();

				float range = Calculate_distance(g_Boss1.WolrdPos.x, g_Boss1.WolrdPos.y, pPlayer->WorldPos.x, pPlayer->WorldPos.y);
				if (range < 0) {
					range *= -1;
				}

				int bulletNum = 0;

				Bullet* pFirstBullet = GetFirstBulletAddress();
				for (Bullet* SearchBullet = pFirstBullet->next; SearchBullet != NULL; SearchBullet = SearchBullet->next) {

					bulletNum++;
				}

				//意思決定
				if (g_Boss1.Boss1State == BOSS1_NON) {
					g_Boss1.Boss1State = DecidedBoss1State(range, bulletNum);
				}

				//行動
				static int DUALSHOTMiddleFrcnt = 0;
				switch (g_Boss1.Boss1State) {
				case NORMALSHOT:
					BulletCreate(g_Boss1.WolrdPos, BULLETTARGET1);
					g_Boss1.saveActionCntForNORMALSHOT++;
					g_Boss1.saveShotFrmcnt = 0;
					g_Boss1.Boss1State = BOSS1_NON;
					break;

				case DUALSHOT:
					if (DUALSHOTMiddleFrcnt == 0) {
						BulletCreate(g_Boss1.WolrdPos, BULLETTARGET1);
					}

					DUALSHOTMiddleFrcnt++;

					if (DUALSHOTMiddleFrcnt == DUALSHOTINTERVALS) {
						g_Boss1.saveActionCntForDUALSHOT++;
						g_Boss1.saveShotFrmcnt = 0;
						g_Boss1.Boss1State = BOSS1_NON;
						DUALSHOTMiddleFrcnt = 0;
						if (g_Boss1.isLeft) {
							BulletCreate(g_Boss1.WolrdPos, BULLETNORMAL2, 150.0f);
						}
						else {
							BulletCreate(g_Boss1.WolrdPos, BULLETNORMAL2, 30.0f);
						}
					}
					break;

				case LARIAT:
					if (g_Boss1.a < BOSS1MAXSPEED) {
						g_Boss1.a += 0.2;
					}

					if (g_Boss1.isLeft) {
						g_Boss1.MovementX += -g_Boss1.a;
						D3DXVECTOR2 boss1Left = g_Boss1.WolrdPos;
						boss1Left.x = g_Boss1.WolrdPos.x - (BOSS1WIDTH / 2) + g_Boss1.MovementX;
						if (MapKindSpecifyForPos(&boss1Left) != NOTHING) {
							g_Boss1.Boss1State = BOSS1_NON;
							g_Boss1.a = 0;
							g_Boss1.saveActionCntForLARIAT++;
							g_Boss1.isLeft = false;
						}
					}
					else {
						g_Boss1.MovementX += g_Boss1.a;
						D3DXVECTOR2 boss1Right = g_Boss1.WolrdPos;
						boss1Right.x = g_Boss1.WolrdPos.x + (BOSS1WIDTH / 2) + g_Boss1.MovementX;
						if (MapKindSpecifyForPos(&boss1Right) != NOTHING) {
							g_Boss1.Boss1State = BOSS1_NON;
							g_Boss1.a = 0;
							g_Boss1.saveActionCntForLARIAT++;
							g_Boss1.isLeft = true;
						}
					}
					break;
				}

				if (g_Boss1.Boss1State != LARIAT) {
					if (pPlayer->WorldPos.x < g_Boss1.WolrdPos.x) {
						g_Boss1.isLeft = true;
						g_Boss1.MovementX = -BOSS1SPEED;
					}
					else {
						g_Boss1.isLeft = false;
						g_Boss1.MovementX = BOSS1SPEED;
					}
				}
				g_Boss1.MovementY = 20;
				g_Boss1.saveShotFrmcnt++;
			}
			else {
				if (g_frcnt == 0) {
					StopSound(MAINSCENE_BOSSBGM01);
					DeleteALLBullet();
				}
				if (g_frcnt == 120) {
					PlayBackSound(MAINSCENE_SE_FANFARE, false, 0);
				}
				if (g_frcnt == 300) {
					g_frcnt = 0;
					g_Boss1.goNextStage = true;
				}
				g_frcnt++;
			}
		}
	}
}

void MoveBoss1() {
	if (g_Boss1.isExistence && !g_Boss1.isDead) {
		D3DXVECTOR2 BasePoint0 = D3DXVECTOR2(DISPLAY_WIDTH / 2, DISPLAY_HEIGHT / 2);
		D3DXVECTOR2* basePoint = GetBasePoint();
		g_Boss1.WolrdPos.x += g_Boss1.MovementX;
		g_Boss1.WolrdPos.y += g_Boss1.MovementY;
		g_Boss1.WindowPos.x = g_Boss1.WolrdPos.x - (basePoint->x - BasePoint0.x);
		g_Boss1.WindowPos.y = g_Boss1.WolrdPos.y - (basePoint->y - BasePoint0.y);
		g_Boss1.MovementX = g_Boss1.MovementY = 0;
	}
}


BOSS1STATE DecidedBoss1State(float range, int bulletNum) {

	float DecidedValue[BOSS1STATE_MAX];

	DecidedValue[NORMALSHOT] = CalculateNORMALSHOTDecidedValue(bulletNum, range);

	DecidedValue[DUALSHOT] = CalculateDUALSHOTDecidedValue(bulletNum, range);

	DecidedValue[LARIAT] = CalculateLARIATDecidedValue(range);

	DecidedValue[BOSS1_NON] = ((100.0f - DecidedValue[NORMALSHOT]) + (100.0f - DecidedValue[NORMALSHOT])) / 2;

	//最も大きい値を決定
	float* Max = &DecidedValue[BOSS1_NON];
	for (int i = 0; i < BOSS1STATE_MAX-1; i++) {
		if (*Max < DecidedValue[i]) {
			Max = &DecidedValue[i];
		}
	}

	BOSS1STATE boss1State = BOSS1_NON;
	if (Max == &DecidedValue[NORMALSHOT] || Max == &DecidedValue[DUALSHOT]) {
		if (*Max < DecidedValue[LARIAT]) {
			boss1State = LARIAT;
		}
		else if (Max == &DecidedValue[DUALSHOT]) {
			boss1State = DUALSHOT;
		}
		else if (Max == &DecidedValue[NORMALSHOT]) {
			boss1State = NORMALSHOT;
		}
	}else {
		boss1State = BOSS1_NON;
	}

	return boss1State;
}


float CalculateNORMALSHOTDecidedValue(int bulletNum, float range) {

	//弾の発射間隔による判定値の計算
	float decidedValue1 = ((5.0f / 9.0f) * (g_Boss1.saveShotFrmcnt - 360.0f)) + 100.0f;

	//画面上の弾の数による判定値の計算
	float decidedValue2 = -(bulletNum * 15.0f) + 100.0f;

	//プレイヤーとの距離による判定値の計算
	float decidedValue3 =100.0f -(range - 800.0f)*(range - 800.0f) / 13500.0f;

	//前回のNORMALSHOTからのアクション数による判定値の計算
	float decidedValue4 = -5.2f * g_Boss1.saveActionCntForNORMALSHOT + 20.8f * g_Boss1.saveActionCntForDUALSHOT;

	//平均値による一意な値の決定
	float decidedValue = (decidedValue1 + decidedValue2 + decidedValue3 + decidedValue4) / 4;

	return decidedValue;
}


float CalculateDUALSHOTDecidedValue(int bulletNum, float range) {

	//弾の発射間隔による判定値の計算
	float decidedValue1 = ((0.7f) * (g_Boss1.saveShotFrmcnt - 360.0f)) + 100.0f;

	//画面上の弾の数による判定値の計算
	float decidedValue2 = -(bulletNum * 15.0f) + 100.0f;

	//プレイヤーとの距離による判定値の計算
	float decidedValue3 = 100.0f - (range * range) / 27300.0f;

	//前回のDUALSHOTからのアクション数による判定値の計算
	float decidedValue4 = 5.2f * g_Boss1.saveActionCntForNORMALSHOT - 20.8f * g_Boss1.saveActionCntForDUALSHOT;

	//平均値による一意な値の決定
	float decidedValue = (decidedValue1 + decidedValue2 + decidedValue3 + decidedValue4) / 4;

	return decidedValue;
}

float CalculateLARIATDecidedValue(float range) {

	//自身の体力による判定値の計算
	float decidedValue1 = 1000.0f / ((g_Boss1.Hp / MAXHP * 100.0f) + 1) + 20.0f;

	//プレイヤーとの距離による判定値の計算
	float decidedValue2 = 80.0f - ((range - 980.0f)*(range - 980.0f)) / 16000.0f;

	//前回のLARIATからのDUALSHOT数による判定値の計算
	float decidedValue3 = (3.5f * g_Boss1.saveActionCntForNORMALSHOT) + (12.0f*g_Boss1.saveActionCntForDUALSHOT) - (35.0f*g_Boss1.saveActionCntForLARIAT);

	//平均値による一意な値の決定
	float decidedValue = (decidedValue1 + decidedValue2 + decidedValue3 + decidedValue3) / 4;

	return decidedValue;
}