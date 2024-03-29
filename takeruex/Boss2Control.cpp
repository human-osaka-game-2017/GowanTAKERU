#include"Boss2Control.h"
#include"FileManagement.h"
#include"MapRender.h"
#include"PlayerControl.h"
#include"CommonRender.h"
#include"MainControl.h"
#include"BulletControl.h"
#include"MapControl.h"
#include"BulletControl.h"
#include"DirectXSound.h"

#define SPEED_X 5.0f
#define SPEED_Y 2.45f

struct Pos {
	float posX, posY;
};

void GoPosB(D3DXVECTOR2* pPos);
void UPDOWN_MOVE(D3DXVECTOR2* pPos);
void HitMove(D3DXVECTOR2* pPos);

Boss2Data g_Boss2;
static int g_GoPosBFrcnt = 0;
static int g_HitMoveFrcnt = 0;
static bool g_Goleft = true;

Boss2Data* GetBoss2Data() {
	return &g_Boss2;
}


void Boss2Init() {
	g_Boss2.isExistence = false;

	STAGE_ID stage_ID = GetStage_ID();
	int MaxX = GetStageXYMAX(stage_ID, X);
	int MaxY = GetStageXYMAX(stage_ID, Y);
	int* gimmickData = (int*)malloc(sizeof(int)*MaxX*MaxY);

	switch (stage_ID) {
	case STAGE_2:

		CSVLoad("CSV/mainscene/stage2_gimmick.csv", gimmickData, MaxY, MaxX);//CSV呼び出し

		for (int i = 0; i < MaxY; i++) {
			for (int j = 0; j < MaxX; j++) {
				if (gimmickData[j + i*MaxX] == BOSS_STAGE2) {
					g_Boss2.isExistence = true;
					g_Boss2.WorldPos.x = TIPSIZE*j;
					g_Boss2.WorldPos.y = TIPSIZE*i;
					goto BREAK;
				}
			}
		}
		break;


	case STAGE_5:

		CSVLoad("CSV/mainscene/stage5_gimmick.csv", gimmickData, MaxY, MaxX);//CSV呼び出し

		for (int i = 0; i < MaxY; i++) {
			for (int j = 0; j < MaxX; j++) {
				if (gimmickData[j + i*MaxX] == BOSS_STAGE2) {
					g_Boss2.isExistence = true;
					g_Boss2.WorldPos.x = TIPSIZE*j;
					g_Boss2.WorldPos.y = TIPSIZE*i;
					goto BREAK;
				}
			}
		}
		break;
	}
BREAK:
	if (g_Boss2.isExistence) {
		SetStopScrollPos(g_Boss2.WorldPos.x);
		g_Boss2.MovementX = g_Boss2.MovementY = 0;
		g_Boss2.Hp = 180;
		g_Boss2.Atk = 20;
		g_Boss2.isLeft = true;
		g_Boss2.isDead = false;
		g_Boss2.isActive = false;
		g_Boss2.hasDamage = false;
		g_Boss2.goNextStage = false;
		g_Boss2.lastbullet = NOTHING;
		g_Boss2.boss2State = GOPOSB;

		g_GoPosBFrcnt = 0;
		g_HitMoveFrcnt = 0;
		g_Goleft = true;
	}
	else {
		g_Boss2.isDead = false;
		g_Boss2.isActive = false;
		g_Boss2.goNextStage = false;
	}

	free(gimmickData);
}

void Boss2Control() {
	if (g_Boss2.isExistence) {

		//活動状態かチェック
		D3DXVECTOR2 BasePoint0 = D3DXVECTOR2(DISPLAY_WIDTH / 2, DISPLAY_HEIGHT / 2);
		D3DXVECTOR2* basePoint = GetBasePoint();
		g_Boss2.WindowPos.x = g_Boss2.WorldPos.x - (basePoint->x - BasePoint0.x);
		g_Boss2.WindowPos.y = g_Boss2.WorldPos.y - (basePoint->y - BasePoint0.y);
		if (-(2 * TIPSIZE) < g_Boss2.WindowPos.x && g_Boss2.WindowPos.x < DISPLAY_WIDTH + (2 * TIPSIZE) &&
			-(2 * TIPSIZE) < g_Boss2.WindowPos.y && g_Boss2.WindowPos.y < DISPLAY_HEIGHT + (2 * TIPSIZE)) {
			g_Boss2.isActive = true;
		}
		else {
			g_Boss2.isActive = false;
		}

		//活動状態である
		if (g_Boss2.isActive) {

			if (!g_Boss2.isDead) {

				if (g_Boss2.hasDamage && g_Boss2.boss2State != GOPOSB) {
					g_Boss2.boss2State = GOLEFT;
				}

				switch (g_Boss2.boss2State) {
				case UDMOVE:
					UPDOWN_MOVE(&g_Boss2.WorldPos);
					break;
				case GOPOSB:
					GoPosB(&g_Boss2.WorldPos);
					break;
				case GOLEFT:
					HitMove(&g_Boss2.WorldPos);
					break;
				}
			}
			else {
				static int frcnt = 0;
				if (frcnt == 0) {
					StopSound(MAINSCENE_BOSSBGM02);
					DeleteALLBullet();
				}
				if (frcnt == 120) {
					PlayBackSound(MAINSCENE_SE_FANFARE, false, 0);
				}
				if (frcnt == 300) {
					frcnt = 0;
					g_Boss2.goNextStage = true;
				}
				frcnt++;
			}
		}
	}
}

void MoveBoss2() {
	
	if (g_Boss2.isExistence && !g_Boss2.isDead) {
		D3DXVECTOR2 BasePoint0 = D3DXVECTOR2(DISPLAY_WIDTH / 2, DISPLAY_HEIGHT / 2);
		D3DXVECTOR2* basePoint = GetBasePoint();
		g_Boss2.WorldPos.x += g_Boss2.MovementX;
		g_Boss2.WorldPos.y += g_Boss2.MovementY;
		g_Boss2.WindowPos.x = g_Boss2.WorldPos.x - (basePoint->x - BasePoint0.x);
		g_Boss2.WindowPos.y = g_Boss2.WorldPos.y - (basePoint->y - BasePoint0.y);
		g_Boss2.MovementX = g_Boss2.MovementY = 0;
	}
}

void GoPosB(D3DXVECTOR2* pPos) {

	if ((g_GoPosBFrcnt % 12) == 0) {
		BulletCreate(*pPos, HOMING2);
	}

	if (g_Goleft) {
		g_Boss2.MovementX -= SPEED_X;
	}
	else {
		g_Boss2.MovementY += SPEED_Y;
	}

	if (g_GoPosBFrcnt == 26) {
		g_Goleft = false;
	}

	if (g_GoPosBFrcnt == 85) {
		g_Boss2.boss2State = UDMOVE;
		g_GoPosBFrcnt = 0;
	}
	g_GoPosBFrcnt++;
}

void UPDOWN_MOVE(D3DXVECTOR2* pPos) {

	//D3DXVECTOR2 LeftTop = { (g_Boss2.WorldPos.x - BOSS2WIDTH / 2),(g_Boss2.WorldPos.y - BOSS2HEIGHT / 2) - 1 };
	//D3DXVECTOR2 RightTop = { (g_Boss2.WorldPos.x + BOSS2WIDTH / 2),(g_Boss2.WorldPos.y - BOSS2HEIGHT / 2) - 1 };
	//D3DXVECTOR2 LeftBottom = { (g_Boss2.WorldPos.x - BOSS2WIDTH / 2),(g_Boss2.WorldPos.y + BOSS2HEIGHT / 2) + 1 };
	//D3DXVECTOR2 RightBottom = { (g_Boss2.WorldPos.x + BOSS2WIDTH / 2),(g_Boss2.WorldPos.y + BOSS2HEIGHT / 2) + 1 };

	static int frcnt = 0;
	if (frcnt%120 == 60) {
		BulletCreate(*pPos, HOMING1);
	}
	if (frcnt % 120 == 119) {
		BulletCreate(*pPos, BULLETNORMAL4, 210.0f);
	}
	
	if (frcnt % 240 < 120) {
		g_Boss2.MovementY = -SPEED_Y;
	}
	else if (frcnt % 240 < 240) {
		g_Boss2.MovementY = SPEED_Y;
	}
	if (frcnt == 240) {
		frcnt = 0;
	}
	frcnt++;

	if (g_Boss2.MovementY > 0) {
		int a = 0;
	}
}

void HitMove(D3DXVECTOR2* pPos) {

	if (6 < g_HitMoveFrcnt) {

		if (g_HitMoveFrcnt < 48) {
			g_Boss2.MovementX = -SPEED_X;
		}
		else if(g_HitMoveFrcnt < 90){
			g_Boss2.MovementX = SPEED_X;
		}

		if (g_HitMoveFrcnt == 30) {
			BulletCreate(*pPos, HOMING1);
		}

		if (g_HitMoveFrcnt == 60) {
			BulletCreate(*pPos, BULLETTARGET3);
		}

		if (g_HitMoveFrcnt == 90) {
			BulletCreate(*pPos, BULLETTARGET3);
			g_HitMoveFrcnt = 0;
			g_Boss2.hasDamage = false;
			g_Boss2.boss2State = UDMOVE;
		}
	}

	g_HitMoveFrcnt++;
}

void KeyBoss2Control(Enemy* pBoss2) {
	if (g_Boss2.boss2State == GOPOSB) {
		g_Boss2.boss2State = UDMOVE;
	}
	static int preHp = pBoss2->Hp;
	if (preHp != pBoss2->Hp) {
		g_Boss2.hasDamage = true;
	}
	preHp = pBoss2->Hp;
	if (g_Boss2.hasDamage) {
		g_Boss2.boss2State = GOLEFT;
	}

	switch (g_Boss2.boss2State) {
	case UDMOVE:
		UPDOWN_MOVE(&pBoss2->WorldPos);
		break;
	case GOPOSB:
		GoPosB(&pBoss2->WorldPos);
		break;
	case GOLEFT:
		HitMove(&pBoss2->WorldPos);
		break;
	}

	pBoss2->MovementX = g_Boss2.MovementX;
	pBoss2->MovementY = g_Boss2.MovementY;
	g_Boss2.MovementX = g_Boss2.MovementY = 0;
}

