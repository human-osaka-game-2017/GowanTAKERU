#include"Boss2Control.h"
#include"FileManagement.h"
#include"MapRender.h"
#include"PlayerControl.h"
#include"CommonRender.h"
#include"MainControl.h"
#include"BulletControl.h"
#include"MapControl.h"


Boss2Data g_Boss2;

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

		CSVLoad("CSV/mainscene/stage2_gimmick.csv", gimmickData, MaxY, MaxX);//CSVåƒÇ—èoÇµ

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

		CSVLoad("CSV/mainscene/stage5_gimmick.csv", gimmickData, MaxY, MaxX);//CSVåƒÇ—èoÇµ

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
		g_Boss2.MovementX = g_Boss2.MovementY = 0;
		g_Boss2.Hp = 70;
		g_Boss2.Atk = 20;
		g_Boss2.isLeft = true;
		g_Boss2.isDead = false;
		g_Boss2.isActive = false;
		g_Boss2.hasLanding = false;
		g_Boss2.hasDamage = false;
		g_Boss2.lastbullet = NOTHING;
	}
	else {
		g_Boss2.isDead = false;
		g_Boss2.isActive = false;
	}

	free(gimmickData);



}

void Boss2Control() {
	if (g_Boss2.isExistence && !g_Boss2.isDead) {
	//äàìÆèÛë‘Ç©Ç«Ç§Ç©
	D3DXVECTOR2 LeftTop = { (g_Boss2.WorldPos.x - BOSS2WIDTH / 2),(g_Boss2.WorldPos.y - BOSS2HEIGHT / 2) };
	D3DXVECTOR2 RightTop = { (g_Boss2.WorldPos.x + BOSS2WIDTH / 2),(g_Boss2.WorldPos.y - BOSS2HEIGHT / 2) };
	D3DXVECTOR2 LeftBottom = { (g_Boss2.WorldPos.x - BOSS2WIDTH / 2),(g_Boss2.WorldPos.y + BOSS2HEIGHT / 2) + 1 };
	D3DXVECTOR2 RightBottom = { (g_Boss2.WorldPos.x + BOSS2WIDTH / 2),(g_Boss2.WorldPos.y + BOSS2HEIGHT / 2) + 1 };
	if (-(2 * TIPSIZE) < g_Boss2.WindowPos.x && g_Boss2.WindowPos.x < DISPLAY_WIDTH + (2 * TIPSIZE) &&
		-(2 * TIPSIZE) < g_Boss2.WindowPos.y && g_Boss2.WindowPos.y < DISPLAY_HEIGHT + (2 * TIPSIZE)) {
		g_Boss2.isActive = true;
	}
	else {
		g_Boss2.isActive = false;
	}
	static int Count;
	
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