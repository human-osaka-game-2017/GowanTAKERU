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
		g_Boss2.MovementX = g_Boss2.MovementY = 0;
		g_Boss2.Hp = 70;
		g_Boss2.Atk = 20;
		g_Boss2.isLeft = true;
		g_Boss2.isDead = false;
		g_Boss2.isActive = false;
	}
	else {
		g_Boss2.isDead = false;
		g_Boss2.isActive = false;
	}

	free(gimmickData);


}

void Boss2Control() {
		static int Boss2FrameCount = 0;
		if (Boss2FrameCount <= 121) {//121フレーム以降はフレームカウントされないようにしている
			Boss2FrameCount++;
		}
		//0.2秒感覚（12フレーム）でホーミング弾発射
		if (Boss2FrameCount == 12 || Boss2FrameCount == 24 || Boss2FrameCount == 36 || Boss2FrameCount == 48 || Boss2FrameCount == 60) {
			BulletCreate(g_Boss2.WorldPos, HOMING);
		}
		//1フレームで3.2ピクセル盾に動かす(1秒で192ピクセル分)
		if (Boss2FrameCount >= 120) {
			g_Boss2.MovementX = -3.2f;
		}
		if (Boss2FrameCount > 120) {

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