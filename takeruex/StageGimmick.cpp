#include"StageGimmick.h"
#include"StageSelect.h"
#include"PlayerControl.h"
#include"FileManagement.h"
#include"MapRender.h"
#include"CommonRender.h"
#include"MapControl.h"
#include"BulletControl.h"
#include"EnemyControl.h"
#include"Boss1Control.h"
#include"Boss3Control.h"
#include"Boss4Control.h"
#include"MainScene.h"

#define KEYNUM_WALKINGENEMY_HAS_KEY_1_FLG	0x0001
#define KEYNUM_WALKINGENEMY_HAS_KEY_2_FLG	0x0002
#define KEYNUM_WALKINGENEMY_HAS_KEY_3_FLG	0x0004
#define KEYNUM_FLYINGENEMY_HAS_KEY1_FLG		0x0008
#define KEYNUM_FLYINGENEMY_HAS_KEY2_FLG		0x0010
#define KEYNUM_FLYINGENEMY_HAS_KEY3_FLG		0x0020

//何度も行わないためのフラグ
static bool g_BossGateflg = false;

//プロトタイプ宣言
void OpenGate(MapKind mapKind);
void CloseGate(MapKind beforeMapKind, MapKind afterMapKind);

enum KEYNUM_KIND {
	KEYNUM_WALKINGENEMY_HAS_KEY_1,
	KEYNUM_WALKINGENEMY_HAS_KEY_2,
	KEYNUM_WALKINGENEMY_HAS_KEY_3,
	KEYNUM_FLYINGENEMY_HAS_KEY1,
	KEYNUM_FLYINGENEMY_HAS_KEY2,
	KEYNUM_FLYINGENEMY_HAS_KEY3,
	KEYNUM_KIND_MAX
};

int g_GateKeyNum[KEYNUM_KIND_MAX] = { 0 };


void StageGimmickInit() {

	g_BossGateflg = false;

	for (int i = 0; i < KEYNUM_KIND_MAX; i++) {
		g_GateKeyNum[i] = 0;
	}
	
	//それぞれの敵のカギ持ちを数え、設定する
	Enemy* pEnemy = GetEnemyData();
	int enemyMax = GetEnemyMax();
	for (int i = 0; i < enemyMax; i++) {
		switch (pEnemy[i].enemyKind) {

		case WALKINGENEMY_HAS_KEY_1:
			g_GateKeyNum[KEYNUM_WALKINGENEMY_HAS_KEY_1]++;
			break;

		case WALKINGENEMY_HAS_KEY_2:
			g_GateKeyNum[KEYNUM_WALKINGENEMY_HAS_KEY_2]++;
			break;

		case WALKINGENEMY_HAS_KEY_3:
			g_GateKeyNum[KEYNUM_WALKINGENEMY_HAS_KEY_3]++;
			break;

		case FLYINGENEMY_HAS_KEY1:
			g_GateKeyNum[KEYNUM_FLYINGENEMY_HAS_KEY1]++;
			break;

		case FLYINGENEMY_HAS_KEY2:
			g_GateKeyNum[KEYNUM_FLYINGENEMY_HAS_KEY2]++;
			break;

		case FLYINGENEMY_HAS_KEY3:
			g_GateKeyNum[KEYNUM_FLYINGENEMY_HAS_KEY3]++;
			break;
		}
	}
}

void StageGimmickManage() {

	Enemy* pEnemy = GetEnemyData();

	int gateKeyNumCnt[KEYNUM_KIND_MAX] = { 0 };

	//敵の死んでいる鍵持ちの数を数える
	int enemyMax = GetEnemyMax();
	for (int i = 0; i < enemyMax; i++) {

		if (pEnemy[i].beDead) {
			switch (pEnemy[i].enemyKind)
			{
			case WALKINGENEMY_HAS_KEY_1:
				gateKeyNumCnt[KEYNUM_WALKINGENEMY_HAS_KEY_1]++;
				break;

			case WALKINGENEMY_HAS_KEY_2:
				gateKeyNumCnt[KEYNUM_WALKINGENEMY_HAS_KEY_2]++;
				break;

			case WALKINGENEMY_HAS_KEY_3:
				gateKeyNumCnt[KEYNUM_WALKINGENEMY_HAS_KEY_3]++;
				break;

			case FLYINGENEMY_HAS_KEY1:
				gateKeyNumCnt[KEYNUM_FLYINGENEMY_HAS_KEY1]++;
				break;

			case FLYINGENEMY_HAS_KEY2:
				gateKeyNumCnt[KEYNUM_FLYINGENEMY_HAS_KEY2]++;
				break;

			case FLYINGENEMY_HAS_KEY3:
				gateKeyNumCnt[KEYNUM_FLYINGENEMY_HAS_KEY3]++;
				break;
			}
		}
	}

	int gateflg = 0;

	//それぞれの設定された鍵持ちの数と死んでいる鍵持ちの数が一緒か調べ、同じならフラグを立てる
	for (int i = 0; i < KEYNUM_KIND_MAX; i++) {

		if (gateKeyNumCnt[i] == g_GateKeyNum[i]) {
			switch (i) {
			case KEYNUM_WALKINGENEMY_HAS_KEY_1:
				gateflg = gateflg | KEYNUM_WALKINGENEMY_HAS_KEY_1_FLG;
				break;
			case KEYNUM_WALKINGENEMY_HAS_KEY_2:
				gateflg = gateflg | KEYNUM_WALKINGENEMY_HAS_KEY_2_FLG;
				break;
			case KEYNUM_WALKINGENEMY_HAS_KEY_3:
				gateflg = gateflg | KEYNUM_WALKINGENEMY_HAS_KEY_3_FLG;
				break;
			case KEYNUM_FLYINGENEMY_HAS_KEY1:
				gateflg = gateflg | KEYNUM_FLYINGENEMY_HAS_KEY1_FLG;
				break;
			case KEYNUM_FLYINGENEMY_HAS_KEY2:
				gateflg = gateflg | KEYNUM_FLYINGENEMY_HAS_KEY2_FLG;
				break;
			case KEYNUM_FLYINGENEMY_HAS_KEY3:
				gateflg = gateflg | KEYNUM_FLYINGENEMY_HAS_KEY3_FLG;
				break;
			}
		}
	}

	//シャッターとリンクさせる
	if (gateflg & KEYNUM_WALKINGENEMY_HAS_KEY_1_FLG) {
		OpenGate(SHUTTER_1);
		g_GateKeyNum[KEYNUM_WALKINGENEMY_HAS_KEY_1] = -1;
	}
	if ((gateflg & KEYNUM_WALKINGENEMY_HAS_KEY_2_FLG) && (gateflg & KEYNUM_FLYINGENEMY_HAS_KEY2_FLG)) {
		OpenGate(SHUTTER_2);
		g_GateKeyNum[KEYNUM_WALKINGENEMY_HAS_KEY_2] = -1;
		g_GateKeyNum[KEYNUM_FLYINGENEMY_HAS_KEY2] = -1;
	}

	bool bossActiveflg = CheckBossActiveBoss();

	if (bossActiveflg && !g_BossGateflg) {
		
		CloseGate(BOSS_SHUTTER, WALL);
		g_BossGateflg = true;
	}


}

void CloseGate(MapKind beforeMapKind, MapKind afterMapKind) {

	STAGE_ID stage_ID = GetStage_ID();
	STAGEXYMAX maxX = GetStageXYMAX(stage_ID, X);
	STAGEXYMAX maxY = GetStageXYMAX(stage_ID, Y);
	int* pGimmickData = (int*)malloc(sizeof(int)*maxX*maxY);

	StopBGM(stage_ID, false);
	PlayBGM(stage_ID, true);

	switch (stage_ID) {
	case STAGE_1:
		CSVLoad("CSV/mainscene/stage1_gimmick.csv", pGimmickData, maxY, maxX);
		break;

	case STAGE_2:
		CSVLoad("CSV/mainscene/stage2_gimmick.csv", pGimmickData, maxY, maxX);
		break;

	case STAGE_3:
		CSVLoad("CSV/mainscene/stage3_gimmick.csv", pGimmickData, maxY, maxX);
		break;

	case STAGE_4:
		CSVLoad("CSV/mainscene/stage4_gimmick.csv", pGimmickData, maxY, maxX);
		break;

	case STAGE_5:
		CSVLoad("CSV/mainscene/stage5_gimmick.csv", pGimmickData, maxY, maxX);
		break;
	}

	int* pMapData = GetMapData();

	for (int i = 0; i < maxY; i++) {
		for (int j = 0; j < maxX; j++) {
			if (*(pGimmickData + (i*maxX + j)) == beforeMapKind) {
				*(pMapData + (i*maxX + j)) = afterMapKind;
			}

		}
	}
}

void OpenGate(MapKind mapKind) {

	STAGE_ID stage_ID = GetStage_ID();
	STAGEXYMAX maxX = GetStageXYMAX(stage_ID, X);
	STAGEXYMAX maxY = GetStageXYMAX(stage_ID, Y);
	int* pGimmickData = (int*)malloc(sizeof(int)*maxX*maxY);

	switch (stage_ID) {
	case STAGE_1:
		CSVLoad("CSV/mainscene/stage1_gimmick.csv", pGimmickData, maxY, maxX);
		break;

	case STAGE_2:
		CSVLoad("CSV/mainscene/stage2_gimmick.csv", pGimmickData, maxY, maxX);
		break;

	case STAGE_3:
		CSVLoad("CSV/mainscene/stage3_gimmick.csv", pGimmickData, maxY, maxX);
		break;

	case STAGE_4:
		CSVLoad("CSV/mainscene/stage4_gimmick.csv", pGimmickData, maxY, maxX);
		break;

	case STAGE_5:
		CSVLoad("CSV/mainscene/stage5_gimmick.csv", pGimmickData, maxY, maxX);
		break;
	}

	int* pMapData = GetMapData();

	for (int i = 0; i < maxY; i++) {
		for (int j = 0; j < maxX; j++) {
			if (*(pGimmickData + (i*maxX + j)) == mapKind) {
				*(pMapData + (i*maxX + j)) = NOTHING;
			}

		}
	}
}

//チェックポイントは必ず二つ
void ComeBackCheckPoint() {

	STAGE_ID stage_ID = GetStage_ID();
	STAGEXYMAX maxX = GetStageXYMAX(stage_ID, X);
	STAGEXYMAX maxY = GetStageXYMAX(stage_ID, Y);
	int* pGimmickData = (int*)malloc(sizeof(int)*maxX*maxY);

	switch (stage_ID) {
	case STAGE_1:
		CSVLoad("CSV/mainscene/stage1_gimmick.csv", pGimmickData, maxY, maxX);
		break;

	case STAGE_2:
		CSVLoad("CSV/mainscene/stage2_gimmick.csv", pGimmickData, maxY, maxX);
		break;

	case STAGE_3:
		CSVLoad("CSV/mainscene/stage3_gimmick.csv", pGimmickData, maxY, maxX);
		break;

	case STAGE_4:
		CSVLoad("CSV/mainscene/stage4_gimmick.csv", pGimmickData, maxY, maxX);
		break;

	case STAGE_5:
		CSVLoad("CSV/mainscene/stage5_gimmick.csv", pGimmickData, maxY, maxX);
		break;
	}

	D3DXVECTOR2 pos[3];
	MapNumXY mapXY[3];
	for (int i = 0; i < maxY; i++) {
		for (int j = 0; j < maxX; j++) {
			
			if ((*(pGimmickData + (i*maxX + j)) == RESPAWN_POINT2)) {
				mapXY[2] = { j,i };
				PosSpecifyForMapchipNumber(&pos[2], &mapXY[2]);
			}
			if ((*(pGimmickData + (i*maxX + j)) == RESPAWN_POINT1)) {
				mapXY[1] = { j,i };
				PosSpecifyForMapchipNumber(&pos[1], &mapXY[1]);
			}
			if ((*(pGimmickData + (i*maxX + j)) == STARTLOCATION)) {
				mapXY[0] = { j,i };
				PosSpecifyForMapchipNumber(&pos[0], &mapXY[0]);
			}
		}
	}
	free(pGimmickData);

	Player* player = GetplayerData();
	D3DXVECTOR2* basePoint = GetBasePoint();
	if (pos[2].x < player->WorldPos.x) {
		player->WorldPos = pos[2];
		mapXY[2].NumY -= 5;
		PosSpecifyForMapchipNumber(basePoint, &mapXY[2]);
	}
	else if (pos[1].x < player->WorldPos.x) {
		player->WorldPos = pos[1];
		mapXY[1].NumY -= 5;
		PosSpecifyForMapchipNumber(basePoint, &mapXY[1]);
	}
	else {
		player->WorldPos = pos[0];
		mapXY[0].NumY -= 5;
		PosSpecifyForMapchipNumber(basePoint, &mapXY[0]);
		basePoint->y = 544;
	}
	
	D3DXVECTOR2 BasePoint0 = D3DXVECTOR2(DISPLAY_WIDTH / 2, DISPLAY_HEIGHT / 2);
	player->WindowPos.x = player->WorldPos.x - (basePoint->x - BasePoint0.x);
	player->WindowPos.y = player->WorldPos.y - (basePoint->y - BasePoint0.y);

	EnemyInit();
	BulletInit();
	Boss1Init();
	Boss3Init();
	StageGimmickInit();

	int* mapdata = GetMapData();
	switch (stage_ID) {
	case STAGE_1:
		CSVLoad("CSV/mainscene/stage1_map.csv", mapdata, maxY, maxX);
		break;

	case STAGE_2:
		CSVLoad("CSV/mainscene/stage2_map.csv", mapdata, maxY, maxX);
		break;

	case STAGE_3:
		CSVLoad("CSV/mainscene/stage3_map.csv", mapdata, maxY, maxX);
		break;

	case STAGE_4:
		CSVLoad("CSV/mainscene/stage4_map.csv", mapdata, maxY, maxX);
		break;

	case STAGE_5:
		CSVLoad("CSV/mainscene/stage5_map.csv", mapdata, maxY, maxX);
		break;
	}
}
bool CheckBossActiveBoss() {
	//boss用シャッターの処理
	Boss1Data* pBoss1 = GetBoss1Data();
	Boss3Data* pBoss3 = GetBoss3Data();
	Boss4Data* pBoss4 = GetBoss4Data();

	bool bossActiveflg = false;
	if (pBoss1->isExistence && pBoss1->isActive) {
		bossActiveflg = pBoss1->isActive;
	}
	if (pBoss3->isExistence && pBoss3->isActive) {
		bossActiveflg = pBoss3->isActive;
	}
	if (pBoss4->isExistence && pBoss4->isActive) {
		bossActiveflg = pBoss4->isActive;
	}

	return bossActiveflg;
}