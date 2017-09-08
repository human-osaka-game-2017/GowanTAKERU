#include"Boss4Control.h"
#include"StageSelect.h"
#include"FileManagement.h"
#include"MapRender.h"
#include"MapControl.h"
#include"PlayerControl.h"
#include"CommonRender.h"
#include"BulletControl.h"
#include"DirectXSound.h"
#include"MainControl.h"

#include<d3dx9.h>

#define BOSS4SPEED 9.0f
#define ACTIONWAITMAX 40

struct Pos {
	float posX, posY;
};

//データ群
Pos posA = { 401 * TIPSIZE + (2 / TIPSIZE),18 * TIPSIZE };
Pos posB = { 404 * TIPSIZE + (2 / TIPSIZE),12 * TIPSIZE };
Pos posC = { 409 * TIPSIZE + (2 / TIPSIZE),11 * TIPSIZE };
Pos posD = { 409 * TIPSIZE + (2 / TIPSIZE),14 * TIPSIZE };
Pos posE = { 413 * TIPSIZE + (2 / TIPSIZE),12 * TIPSIZE };
Pos posF = { 417 * TIPSIZE + (2 / TIPSIZE),18 * TIPSIZE };
Pos posG = { 423 * TIPSIZE + (2 / TIPSIZE),11 * TIPSIZE };
Pos posH = { 423 * TIPSIZE + (2 / TIPSIZE),24 * TIPSIZE };

//プロトタイプ群
BOSS4ACTIONLIST DecidedAction();
void UpdateTeleport();
void DualShot();
bool CheckInPoint(const Pos& Point, const D3DXVECTOR2& LeftTop, const D3DXVECTOR2& RightBottom);
float CalculateDistance(float forPoint, float fromPoint);
void Boss4Action();
void Action_GO_XXX(Pos firstPos, BOSS4POINT firstPoint, Pos endPos, BOSS4POINT endPoint, bool mini);
void Action_GO_XG();
void Action_GO_XH();
//GHは省く
void Action_GO_GX(Pos endPos, BOSS4POINT endPoint);
void Action_GO_H_LARIAT_D();

Boss4Data g_Boss4;
//static bool g_isControlInitial = false;

Boss4Data* GetBoss4Data() {
	return &g_Boss4;
}

void Boss4Init() {
	g_Boss4.isExistence = false;
	//g_isControlInitial = false;

	STAGE_ID stage_ID = GetStage_ID();
	int MaxX = GetStageXYMAX(stage_ID, X);
	int MaxY = GetStageXYMAX(stage_ID, Y);
	int* gimmickData = (int*)malloc(sizeof(int)*MaxX*MaxY);

	switch (stage_ID) {
	case STAGE_4:

		CSVLoad("CSV/mainscene/stage4_gimmick.csv", gimmickData, MaxY, MaxX);//CSV呼び出し

		for (int i = 0; i < MaxY; i++) {
			for (int j = 0; j < MaxX; j++) {
				if (gimmickData[j + i*MaxX] == BOSS_STAGE4) {
					g_Boss4.isExistence = true;
					g_Boss4.WolrdPos.x = TIPSIZE*j;
					g_Boss4.WolrdPos.y = TIPSIZE*i + 5;
					goto BREAK;
				}
			}
		}
		break;

	case STAGE_5:

		CSVLoad("CSV/mainscene/stage5_gimmick.csv", gimmickData, MaxY, MaxX);//CSV呼び出し

		for (int i = 0; i < MaxY; i++) {
			for (int j = 0; j < MaxX; j++) {
				if (gimmickData[j + i*MaxX] == BOSS_STAGE4) {
					g_Boss4.isExistence = true;
					g_Boss4.WolrdPos.x = TIPSIZE*j;
					g_Boss4.WolrdPos.y = TIPSIZE*i;
					goto BREAK;
				}
			}
		}
	}

BREAK:
	if (g_Boss4.isExistence) {
		SetStopScrollPos(g_Boss4.WolrdPos.x);
		g_Boss4.Boss4TeleportState = NONMOVE;
		g_Boss4.BOSS4ACTIONLIST = NONACTION;
		g_Boss4.currentPoint = POINT_C;
		g_Boss4.currentActionState = ACTION_FIN;
		g_Boss4.MovementX = g_Boss4.MovementY = 0;
		g_Boss4.Hp = 100;
		g_Boss4.Atk = 20;
		g_Boss4.TeleportFrCnt = -1;
		g_Boss4.ActionWaitFrCnt = 0;
		g_Boss4.shotFrCnt = 0;
		g_Boss4.waitFrCnt = 0;
		g_Boss4.isLeft = true;
		g_Boss4.isDead = false;
		g_Boss4.isActive = false;
		g_Boss4.goNextStage = false;

	}
	else {
		g_Boss4.isDead = false;
		g_Boss4.isActive = false;
		g_Boss4.goNextStage = false;
	}

	free(gimmickData);
}

void Boss4Control() {

	if (g_Boss4.isExistence) {
			//活動状態かチェック
			D3DXVECTOR2 BasePoint0 = D3DXVECTOR2(DISPLAY_WIDTH / 2, DISPLAY_HEIGHT / 2);
			D3DXVECTOR2* basePoint = GetBasePoint();
			g_Boss4.WindowPos.x = g_Boss4.WolrdPos.x - (basePoint->x - BasePoint0.x);
			g_Boss4.WindowPos.y = g_Boss4.WolrdPos.y - (basePoint->y - BasePoint0.y);
			if (-(2 * TIPSIZE) < g_Boss4.WindowPos.x && g_Boss4.WindowPos.x < DISPLAY_WIDTH + (2 * TIPSIZE) &&
				-(2 * TIPSIZE) < g_Boss4.WindowPos.y && g_Boss4.WindowPos.y < DISPLAY_HEIGHT + (2 * TIPSIZE)) {
				g_Boss4.isActive = true;
			}
			else {
				g_Boss4.isActive = false;
			}

			//活動状態である
			if (g_Boss4.isActive) {
				if (!g_Boss4.isDead) {

					Player* pPlayer = GetplayerData();
					if (g_Boss4.WolrdPos.x < pPlayer->WorldPos.x) {
						g_Boss4.isLeft = false;
					}
					else {
						g_Boss4.isLeft = true;
					}

					//初期処理
					if (g_Boss4.TeleportFrCnt < 0) {
						g_Boss4.BOSS4ACTIONLIST = GO_ACF;
						g_Boss4.currentActionState = ACTION_START;
						g_Boss4.TeleportFrCnt = 0;
						//BulletCreate(g_Boss4.WolrdPos, FIREWORKS, 180.0f);
					}

					if (g_Boss4.shotFrCnt == 180) {
						BulletCreate(g_Boss4.WolrdPos, HOMING1);
						g_Boss4.shotFrCnt = 0;
					}
					else {
						g_Boss4.shotFrCnt++;
					}

					//コントロール
					if (g_Boss4.BOSS4ACTIONLIST == NONACTION) {
						if (g_Boss4.waitFrCnt == BOSS4WAAITTIME) {
							g_Boss4.BOSS4ACTIONLIST = DecidedAction();
							g_Boss4.currentActionState = ACTION_START;
							g_Boss4.waitFrCnt = 0;
						}
						g_Boss4.waitFrCnt++;
					}
					else {
						Boss4Action();

						if (g_Boss4.Boss4TeleportState != NONMOVE) {
							UpdateTeleport();
						}
					}
			}
			else {
				static int frcnt = 0;
				if (frcnt == 0) {
					StopSound(MAINSCENE_BOSSBGM03);
					DeleteALLBullet();
				}
				if (frcnt == 120) {
					PlayBackSound(MAINSCENE_SE_FANFARE, false, 0);
				}
				if (frcnt == 300) {
					frcnt = 0;
					g_Boss4.goNextStage = true;
				}
				frcnt++;
			}
		}
	}
}

float CalculateDistance(float forPoint, float fromPoint) {
	return (forPoint - fromPoint);
}

void MoveBoss4() {
	if (g_Boss4.isExistence && !g_Boss4.isDead) {
		D3DXVECTOR2 BasePoint0 = D3DXVECTOR2(DISPLAY_WIDTH / 2, DISPLAY_HEIGHT / 2);
		D3DXVECTOR2* basePoint = GetBasePoint();
		g_Boss4.WolrdPos.x += g_Boss4.MovementX;
		g_Boss4.WolrdPos.y += g_Boss4.MovementY;
		g_Boss4.WindowPos.x = g_Boss4.WolrdPos.x - (basePoint->x - BasePoint0.x);
		g_Boss4.WindowPos.y = g_Boss4.WolrdPos.y - (basePoint->y - BasePoint0.y);
		g_Boss4.MovementX = g_Boss4.MovementY = 0;
	}
}

bool CheckInPoint(const Pos& Point, const D3DXVECTOR2& LeftTop, const D3DXVECTOR2& RightBottom) {
	if (LeftTop.x < Point.posX &&  Point.posX < RightBottom.x) {
		if (LeftTop.y < Point.posY && Point.posY < RightBottom.y) {
			return true;
		}
	}
	return false;
}

void GoPoint(const Pos& forPoint) {
	g_Boss4.MovementX = CalculateDistance(forPoint.posX, g_Boss4.WolrdPos.x);
	g_Boss4.MovementY = CalculateDistance(forPoint.posY, g_Boss4.WolrdPos.y);
}

BOSS4ACTIONLIST DecidedAction() {

	int random = Random(0, 99);

	BOSS4ACTIONLIST nextAction;

	/*if (random < 10) {
		nextAction = TELPORTCONTINUE;
	}
	else {*/
		random = Random(0, 99);
		switch (g_Boss4.currentPoint) {
		case POINT_A:
			if (random < 30) {
				nextAction = GO_ACF;
			}
			else if (random < 60) {
				nextAction = GO_ABD;
			}
			else if (random < 80) {
				nextAction = GO_AG;
			}
			else {
				nextAction = GO_AH;
			}
			break;

		case POINT_D:
			if (random < 30) {
				nextAction = GO_DBA;
			}
			else if (random < 60) {
				nextAction = GO_DEF;
			}
			else if (random < 80) {
				nextAction = GO_DG;
			}
			else {
				nextAction = GO_DH;
			}
			break;

		case POINT_F:
			if (random < 30) {
				nextAction = GO_FED;
			}
			else if (random < 60) {
				nextAction = GO_FCA;
			}
			else if (random < 80) {
				nextAction = GO_FG;
			}
			else {
				nextAction = GO_FH;
			}
			break;

		case POINT_G:
			if (random < 24) {
				nextAction = GO_GA;
			}
			else if (random < 50) {
				nextAction = GO_GD;
			}
			else if (random < 75) {
				nextAction = GO_GF;
			}
			else {
				nextAction = GO_GH;
			}
			break;

		case POINT_H:
			nextAction = GO_H_LARIAT_D;
			break;
		//}
	}

	return nextAction;
}

void UpdateTeleport() {

	if (g_Boss4.TeleportFrCnt < BOSS4TELEPORTFRM) {
		if (g_Boss4.Boss4TeleportState / 10) {
			g_Boss4.Boss4TeleportState = FADEOUT_FIRSTHALF;
		}
		else {
			g_Boss4.Boss4TeleportState = TELEPORT_FIRSTHALF;
		}
	}
	else if (g_Boss4.TeleportFrCnt == BOSS4TELEPORTFRM) {
		if (g_Boss4.Boss4TeleportState / 10) {
			g_Boss4.Boss4TeleportState = FADEOUT_HARF;
		}
		else {
			g_Boss4.Boss4TeleportState = TELEPORT_HARF;
		}
	}
	else if (g_Boss4.TeleportFrCnt < BOSS4TELEPORTFRM * 2) {
		if (g_Boss4.Boss4TeleportState / 10) {
			g_Boss4.Boss4TeleportState = FADEOUT_SECONDHALF;
		}
		else {
			g_Boss4.Boss4TeleportState = TELEPORT_SECONDHALF;
		}
	}
	else if (g_Boss4.TeleportFrCnt == BOSS4TELEPORTFRM * 2) {
		if (g_Boss4.Boss4TeleportState / 10) {
			g_Boss4.Boss4TeleportState = FADEOUT_FIN;
		}
		else {
			g_Boss4.Boss4TeleportState = TELEPORT_FIN;
		}
	}
	else {
		g_Boss4.Boss4TeleportState = NONMOVE;
		g_Boss4.TeleportFrCnt = 0;
	}

	g_Boss4.TeleportFrCnt++;
}

void DualShot() {
	if (g_Boss4.isLeft) {
		BulletCreate(g_Boss4.WolrdPos, BULLETNORMAL2, 0.0f);
		BulletCreate(g_Boss4.WolrdPos, NONREFLECTNORMAL2, 45.0f);
	}
	else {
		BulletCreate(g_Boss4.WolrdPos, BULLETNORMAL2, 180.0f);
		BulletCreate(g_Boss4.WolrdPos, NONREFLECTNORMAL2, 125.0f);
	}
}

void SquadShot() {
	D3DXVECTOR2 tmp = g_Boss4.WolrdPos;
	tmp.x += g_Boss4.MovementX;
	BulletCreate(tmp, BULLETNORMAL1, 210.0f);
	BulletCreate(tmp, BULLETNORMAL1, 240.0f);
	BulletCreate(tmp, BULLETNORMAL1, 300.0f);
	BulletCreate(tmp, BULLETNORMAL1, 330.0f);
}

void Boss4Action() {

	switch (g_Boss4.BOSS4ACTIONLIST) {

	case GO_ACF:
		Action_GO_XXX(posC, POINT_C, posF, POINT_F, false);
		break;

	case GO_ABD:
		Action_GO_XXX(posB, POINT_B, posD, POINT_D, true);
		break;
	
	case GO_DBA:
		Action_GO_XXX(posB, POINT_B, posA, POINT_A, true);
		break;

	case GO_DEF:
		Action_GO_XXX(posE, POINT_E, posF, POINT_F, true);
		break;

	case GO_FED:
		Action_GO_XXX(posE, POINT_E, posD, POINT_D, true);
		break;

	case GO_FCA:
		Action_GO_XXX(posC, POINT_C, posA, POINT_A, false);
		break;

	case GO_GA:
		Action_GO_GX(posA, POINT_A);
		break;

	case GO_GD:
		Action_GO_GX(posD, POINT_D);
		break;

	case GO_GF:
		Action_GO_GX(posF, POINT_F);
		break;

	case GO_H_LARIAT_D:
		Action_GO_H_LARIAT_D();
		break;

	case GO_AG:
	case GO_DG:
	case GO_FG:
		Action_GO_XG();
		break;

	case GO_AH:
	case GO_DH:
	case GO_FH:
	case GO_GH:
		Action_GO_XH();
		break;
	}
}

void Action_GO_XG() {
	switch (g_Boss4.currentActionState) {
	case ACTION_START:
		g_Boss4.Boss4TeleportState = TELEPORT_START;
		g_Boss4.currentActionState = ACTION_FIRSTHALF;
		break;

	case ACTION_FIRSTHALF:
		if (g_Boss4.Boss4TeleportState == TELEPORT_HARF) {
			GoPoint(posG);
			g_Boss4.currentPoint = POINT_G;
		}
		if (g_Boss4.Boss4TeleportState == NONMOVE) {
			g_Boss4.ActionWaitFrCnt++;
			if (g_Boss4.ActionWaitFrCnt == ACTIONWAITMAX / 2) {
				BulletCreate(g_Boss4.WolrdPos, FIREWORKS, 180.0f);
			}
			if (g_Boss4.ActionWaitFrCnt == ACTIONWAITMAX) {
				g_Boss4.ActionWaitFrCnt = 0;
				g_Boss4.currentActionState = ACTION_FIN;
				g_Boss4.BOSS4ACTIONLIST = NONACTION;
			}
		}
		break;

	}
}

void Action_GO_XH() {
	switch (g_Boss4.currentActionState) {
	case ACTION_START:
		g_Boss4.Boss4TeleportState = FADEOUT_START;
		g_Boss4.currentActionState = ACTION_FIRSTHALF;
		break;

	case ACTION_FIRSTHALF:
		if (g_Boss4.Boss4TeleportState == FADEOUT_HARF) {
			GoPoint(posH);
			g_Boss4.currentPoint = POINT_H;
		}
		if (g_Boss4.Boss4TeleportState == NONMOVE) {
			g_Boss4.ActionWaitFrCnt++;
			if (g_Boss4.ActionWaitFrCnt == ACTIONWAITMAX) {
				g_Boss4.ActionWaitFrCnt = 0;
				g_Boss4.currentActionState = ACTION_FIN;
				g_Boss4.BOSS4ACTIONLIST = NONACTION;
			}
		}
		break;
	}
}

//GHは省く
void Action_GO_GX(Pos endPos, BOSS4POINT endPoint) {
	switch (g_Boss4.currentActionState) {
	case ACTION_START:
		g_Boss4.Boss4TeleportState = TELEPORT_START;
		g_Boss4.currentActionState = ACTION_FIRSTHALF;
		break;

	case ACTION_FIRSTHALF:
		if (g_Boss4.Boss4TeleportState == TELEPORT_HARF) {
			GoPoint(endPos);
			g_Boss4.currentPoint = endPoint;
		}
		if (g_Boss4.Boss4TeleportState == NONMOVE) {
			g_Boss4.ActionWaitFrCnt++;
			
			if (g_Boss4.ActionWaitFrCnt == ACTIONWAITMAX) {
				g_Boss4.ActionWaitFrCnt = 0;
				g_Boss4.currentActionState = ACTION_FIN;
				g_Boss4.BOSS4ACTIONLIST = NONACTION;
			}
		}
		break;

	}
}

void Action_GO_XXX(Pos firstPos,BOSS4POINT firstPoint, Pos endPos, BOSS4POINT endPoint, bool mini) {
	switch (g_Boss4.currentActionState) {

	case ACTION_START:
		g_Boss4.Boss4TeleportState = TELEPORT_START;
		g_Boss4.currentActionState = ACTION_FIRSTHALF;
		break;

	case ACTION_FIRSTHALF:
		if (g_Boss4.Boss4TeleportState == TELEPORT_HARF) {
			GoPoint(firstPos);
			g_Boss4.currentPoint = firstPoint;
		}
		if (g_Boss4.Boss4TeleportState == NONMOVE) {
			g_Boss4.ActionWaitFrCnt++;
			if (g_Boss4.ActionWaitFrCnt == ACTIONWAITMAX / 2 + 1) {
				if (mini) {
					BulletCreate(g_Boss4.WolrdPos, BULLETTARGET2);
				}
				else {
					SquadShot();
				}
			}
			if (g_Boss4.ActionWaitFrCnt == ACTIONWAITMAX) {
				g_Boss4.ActionWaitFrCnt = 0;
				g_Boss4.Boss4TeleportState = TELEPORT_START;
				g_Boss4.currentActionState = ACTION_SECONDHALF;
			}
		}
		break;

	case ACTION_SECONDHALF:
		if (g_Boss4.Boss4TeleportState == TELEPORT_HARF) {
			
			GoPoint(endPos);
			g_Boss4.currentPoint = endPoint;
			if (endPoint == POINT_D) {
				SquadShot();
			}
		}
		if (g_Boss4.Boss4TeleportState == TELEPORT_FIN) {
			g_Boss4.currentActionState = ACTION_FIN;
			g_Boss4.BOSS4ACTIONLIST = NONACTION;
		}
		break;


	}
}

void Action_GO_H_LARIAT_D() {

	D3DXVECTOR2 boss4Left = g_Boss4.WolrdPos;
	boss4Left.x = g_Boss4.WolrdPos.x - (BOSS4WIDTH) + g_Boss4.MovementX - 1;

	switch (g_Boss4.currentActionState) {

	case ACTION_START:
		g_Boss4.currentActionState = ACTION_FIRSTHALF;
		break;

	case ACTION_FIRSTHALF:

		g_Boss4.ActionWaitFrCnt++;
		//if (g_Boss4.ActionWaitFrCnt > ACTIONWAITMAX/2) {

			g_Boss4.MovementX -= BOSS4SPEED;

			if (MapKindSpecifyForPos(&boss4Left) != NOTHING) {
				g_Boss4.ActionWaitFrCnt = 0;
				g_Boss4.currentActionState = ACTION_HALF;
			}
	//	}
		break;

	case ACTION_HALF:
		g_Boss4.Boss4TeleportState = TELEPORT_START;
		g_Boss4.currentActionState = ACTION_SECONDHALF;
		break;

	case ACTION_SECONDHALF:
		if (g_Boss4.Boss4TeleportState == TELEPORT_HARF) {
			GoPoint(posD);
			g_Boss4.currentPoint = POINT_D;
		}
		if (g_Boss4.Boss4TeleportState == TELEPORT_FIN) {

			g_Boss4.ActionWaitFrCnt = 0;
			g_Boss4.currentActionState = ACTION_FIN;
			g_Boss4.BOSS4ACTIONLIST = NONACTION;

		}
	}
}