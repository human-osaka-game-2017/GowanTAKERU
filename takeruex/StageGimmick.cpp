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
#include"Boss2Control.h"
#include"Boss4Control.h"
#include"MainScene.h"
#include"DirectXSound.h"

#define S_STOPENEMY_SHUTTER1_KEYNUMFLG		0x0001
#define G_MOVEENEMY_SHUTTER2_KEYNUMFLG		0x0002
#define S_MOVEENEMY_SHUTTER2_KEYNUMFLG		0x0004
#define DUMMY_______SHUTTER3_KEYNUMFLG		0x0008
#define DUMMY1______SHUTTER4_KEYNUMFLG		0x0010
#define DUMMY2______SHUTTER4_KEYNUMFLG		0x0020
#define G_MOVEENEMY_SHUTTER5_KEYNUMFLG		0x0040
#define G_MOVEENEMY_SHUTTER6_KEYNUMFLG		0x0080
#define DUMMY_______SHUTTER6_KEYNUMFLG		0x0100
#define BOSS1_SHUTTER7_KEYNUMFLG			0x0200
#define BOSS2_SHUTTER8_KEYNUMFLG			0x0400
#define DUMMY_______SHUTTER8_KEYNUMFLG		0x0800
#define KEYNUM_SWITCHA_FLG					0x1000
#define KEYNUM_SWITCHB_FLG					0x2000
#define KEYNUM_SWITCHC_FLG					0x4000

//何度も行わないためのフラグ
static bool g_BossGateflg = false;

//プロトタイプ宣言
void OpenGate(MapKind mapKind);
void CloseGate(MapKind beforeMapKind, MapKind afterMapKind);

enum KEYNUM_KIND {
	S_STOPENEMY_SHUTTER1_KEYNUM,
	G_MOVEENEMY_SHUTTER2_KEYNUM,
	S_MOVEENEMY_SHUTTER2_KEYNUM,
	DUMMY_______SHUTTER3_KEYNUM,//未確定
	DUMMY1______SHUTTER4_KEYNUM,//未確定
	DUMMY2______SHUTTER4_KEYNUM,//未確定
	G_MOVEENEMY_SHUTTER5_KEYNUM,
	G_MOVEENEMY_SHUTTER6_KEYNUM,
	DUMMY_______SHUTTER6_KEYNUM,//未確定
	BOSS1_SHUTTER7_KEYNUM,
	BOSS2_SHUTTER8_KEYNUM,
	DUMMY_______SHUTTER8_KEYNUM,//未確定
	SWITCH_A_KEYNUM,
	SWITCH_B_KEYNUM,
	SWITCH_C_KEYNUM,
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

		case S_STOPENEMY_SHUTTER1_KEY:
			g_GateKeyNum[S_STOPENEMY_SHUTTER1_KEYNUM]++;
			break;

		case G_MOVEENEMY_SHUTTER2_KEY:
			g_GateKeyNum[G_MOVEENEMY_SHUTTER2_KEYNUM]++;
			break;

		case S_MOVEENEMY_SHUTTER2_KEY:
			g_GateKeyNum[S_MOVEENEMY_SHUTTER2_KEYNUM]++;
			break;

		case DUMMY_______SHUTTER3_KEY:
			g_GateKeyNum[DUMMY_______SHUTTER3_KEYNUM]++;
			break;

		case DUMMY1______SHUTTER4_KEY:
			g_GateKeyNum[DUMMY1______SHUTTER4_KEYNUM]++;
			break;

		case DUMMY2______SHUTTER4_KEY:
			g_GateKeyNum[DUMMY2______SHUTTER4_KEYNUM]++;
			break;

		case G_MOVEENEMY_SHUTTER5_KEY:
			g_GateKeyNum[G_MOVEENEMY_SHUTTER5_KEYNUM]++;
			break;

		case G_MOVEENEMY_SHUTTER6_KEY:
			g_GateKeyNum[G_MOVEENEMY_SHUTTER6_KEYNUM]++;
			break;

		case DUMMY_______SHUTTER6_KEY:
			g_GateKeyNum[DUMMY_______SHUTTER6_KEYNUM]++;
			break;

		case BOSS1_SHUTTER7_KEY:
			g_GateKeyNum[BOSS1_SHUTTER7_KEYNUM]++;
			break;

		case BOSS2_SHUTTER8_KEY:
			g_GateKeyNum[BOSS2_SHUTTER8_KEYNUM]++;
			break;

		case DUMMY_______SHUTTER8_KEY:
			g_GateKeyNum[DUMMY_______SHUTTER8_KEYNUM]++;
			break;

		case SWITCH_A:
			g_GateKeyNum[SWITCH_A_KEYNUM]++;
			break;

		case SWITCH_B:
			g_GateKeyNum[SWITCH_B_KEYNUM]++;
			break;

		case SWITCH_C:
			g_GateKeyNum[SWITCH_C_KEYNUM]++;
			break;
		}
	}
}

void StageGimmickManage() {

	Enemy* pEnemy = GetEnemyData();

	int gateKeyNumCnt[KEYNUM_KIND_MAX] = { 0 };

	int gateflg = 0;

	//敵の死んでいる鍵持ちの数を数える
	int enemyMax = GetEnemyMax();
	for (int i = 0; i < enemyMax; i++) {

		switch (pEnemy[i].enemyKind) {
		case SWITCH_A:
			if (pEnemy[i].Hp != INTMAX) {
				gateKeyNumCnt[SWITCH_A_KEYNUM]++;
				pEnemy[i].tu = 256.0f;
			}
			break;
		case SWITCH_B:
			if (pEnemy[i].Hp != INTMAX) {
				gateKeyNumCnt[SWITCH_B_KEYNUM]++;
				pEnemy[i].tu = 256.0f;
			}
			break;
		case SWITCH_C:
			if (pEnemy[i].Hp != INTMAX) {
				gateKeyNumCnt[SWITCH_C_KEYNUM]++;
				pEnemy[i].tu = 256.0f;
			}
			break;
		}

		if (pEnemy[i].beDead) {
			switch (pEnemy[i].enemyKind)
			{
			case S_STOPENEMY_SHUTTER1_KEY:
				gateKeyNumCnt[S_STOPENEMY_SHUTTER1_KEYNUM]++;
				break;

			case G_MOVEENEMY_SHUTTER2_KEY:
				gateKeyNumCnt[G_MOVEENEMY_SHUTTER2_KEYNUM]++;
				break;

			case S_MOVEENEMY_SHUTTER2_KEY:
				gateKeyNumCnt[S_MOVEENEMY_SHUTTER2_KEYNUM]++;
				break;

			case DUMMY_______SHUTTER3_KEY:
				gateKeyNumCnt[DUMMY_______SHUTTER3_KEYNUM]++;
				break;

			case DUMMY1______SHUTTER4_KEY:
				gateKeyNumCnt[DUMMY1______SHUTTER4_KEYNUM]++;
				break;

			case DUMMY2______SHUTTER4_KEY:
				gateKeyNumCnt[DUMMY2______SHUTTER4_KEYNUM]++;
				break;

			case G_MOVEENEMY_SHUTTER5_KEY:
				gateKeyNumCnt[G_MOVEENEMY_SHUTTER5_KEYNUM]++;
				break;

			case G_MOVEENEMY_SHUTTER6_KEY:
				gateKeyNumCnt[G_MOVEENEMY_SHUTTER6_KEYNUM]++;
				break;

			case DUMMY_______SHUTTER6_KEY:
				gateKeyNumCnt[DUMMY_______SHUTTER6_KEYNUM]++;
				break;

			case BOSS1_SHUTTER7_KEY:
				gateKeyNumCnt[BOSS1_SHUTTER7_KEYNUM]++;
				break;

			case BOSS2_SHUTTER8_KEY:
				gateKeyNumCnt[BOSS2_SHUTTER8_KEYNUM]++;
				break;

			case DUMMY_______SHUTTER8_KEY:
				gateKeyNumCnt[DUMMY_______SHUTTER8_KEYNUM]++;
				break;

			}
		}
	}

	//それぞれの設定された鍵持ちの数と死んでいる鍵持ちの数が一緒か調べ、同じならフラグを立てる
	//スイッチのフラグチェック
	for (int i = 0; i < KEYNUM_KIND_MAX; i++) {

		//if (!(g_GateKeyNum[i] == 0)) {

			if (gateKeyNumCnt[i] == g_GateKeyNum[i]) {
				switch (i) {
				case S_STOPENEMY_SHUTTER1_KEYNUM:
					gateflg = gateflg | S_STOPENEMY_SHUTTER1_KEYNUMFLG;
					break;

				case G_MOVEENEMY_SHUTTER2_KEYNUM:
					gateflg = gateflg | G_MOVEENEMY_SHUTTER2_KEYNUMFLG;
					break;

				case S_MOVEENEMY_SHUTTER2_KEYNUM:
					gateflg = gateflg | S_MOVEENEMY_SHUTTER2_KEYNUMFLG;
					break;

				case DUMMY_______SHUTTER3_KEYNUM:
					gateflg = gateflg | DUMMY_______SHUTTER3_KEYNUMFLG;
					break;

				case DUMMY1______SHUTTER4_KEYNUM:
					gateflg = gateflg | DUMMY1______SHUTTER4_KEYNUMFLG;
					break;

				case DUMMY2______SHUTTER4_KEYNUM:
					gateflg = gateflg | DUMMY2______SHUTTER4_KEYNUMFLG;
					break;

				case G_MOVEENEMY_SHUTTER5_KEYNUM:
					gateflg = gateflg | G_MOVEENEMY_SHUTTER5_KEYNUMFLG;
					break;

				case G_MOVEENEMY_SHUTTER6_KEYNUM:
					gateflg = gateflg | G_MOVEENEMY_SHUTTER6_KEYNUMFLG;
					break;

				case DUMMY_______SHUTTER6_KEYNUM:
					gateflg = gateflg | DUMMY_______SHUTTER6_KEYNUMFLG;
					break;

				case BOSS1_SHUTTER7_KEYNUM:
					gateflg = gateflg | BOSS1_SHUTTER7_KEYNUMFLG;
					break;

				case BOSS2_SHUTTER8_KEYNUM:
						gateflg = gateflg | BOSS2_SHUTTER8_KEYNUMFLG;
					break;

				case DUMMY_______SHUTTER8_KEYNUM:
						gateflg = gateflg | DUMMY_______SHUTTER8_KEYNUMFLG;
					break;

				case SWITCH_A_KEYNUM:
						gateflg = gateflg | KEYNUM_SWITCHA_FLG;
					break;

				case SWITCH_B_KEYNUM:
					gateflg = gateflg | KEYNUM_SWITCHB_FLG;
					break;

				case SWITCH_C_KEYNUM:
					gateflg = gateflg | KEYNUM_SWITCHC_FLG;
					break;
				}
			//}
		}
	}

	//シャッターとリンクさせる

	if (gateflg & S_STOPENEMY_SHUTTER1_KEYNUMFLG) {
		OpenGate(SHUTTER1);
		PlayBackSound(MAINSCENE_SE_SHUTTER, false, 100);
		g_GateKeyNum[S_STOPENEMY_SHUTTER1_KEYNUM] = -1;
	}
	if ((gateflg & G_MOVEENEMY_SHUTTER2_KEYNUMFLG) && (gateflg & S_MOVEENEMY_SHUTTER2_KEYNUMFLG)) {
		OpenGate(SHUTTER2);
		PlayBackSound(MAINSCENE_SE_SHUTTER, false, 100);
		g_GateKeyNum[G_MOVEENEMY_SHUTTER2_KEYNUM] = -1;
		g_GateKeyNum[S_MOVEENEMY_SHUTTER2_KEYNUM] = -1;
	}
	if (gateflg & DUMMY_______SHUTTER3_KEYNUMFLG) {
		OpenGate(SHUTTER3);
		PlayBackSound(MAINSCENE_SE_SHUTTER, false, 100);
		g_GateKeyNum[DUMMY_______SHUTTER3_KEYNUM] = -1;
	}
	if ((gateflg & DUMMY1______SHUTTER4_KEYNUMFLG) && (gateflg & DUMMY2______SHUTTER4_KEYNUMFLG)) {
		OpenGate(SHUTTER4);
		PlayBackSound(MAINSCENE_SE_SHUTTER, false, 100);
		g_GateKeyNum[DUMMY1______SHUTTER4_KEYNUM] = -1;
		g_GateKeyNum[DUMMY2______SHUTTER4_KEYNUM] = -1;
	}

	if (gateflg & G_MOVEENEMY_SHUTTER5_KEYNUMFLG) {
		OpenGate(SHUTTER5);
		PlayBackSound(MAINSCENE_SE_SHUTTER, false, 100);
		g_GateKeyNum[G_MOVEENEMY_SHUTTER5_KEYNUM] = -1;
	}
	if ((gateflg & G_MOVEENEMY_SHUTTER6_KEYNUMFLG) && (gateflg & DUMMY_______SHUTTER6_KEYNUMFLG)) {
		OpenGate(SHUTTER6);
		PlayBackSound(MAINSCENE_SE_SHUTTER, false, 100);
		g_GateKeyNum[G_MOVEENEMY_SHUTTER6_KEYNUM] = -1;
		g_GateKeyNum[DUMMY_______SHUTTER6_KEYNUM] = -1;
	}
	if (gateflg & BOSS1_SHUTTER7_KEYNUMFLG) {
		OpenGate(SHUTTER7);
		PlayBackSound(MAINSCENE_SE_SHUTTER, false, 100);
		g_GateKeyNum[BOSS1_SHUTTER7_KEYNUM] = -1;
	}
	if ((gateflg & BOSS2_SHUTTER8_KEYNUMFLG) && (gateflg & DUMMY_______SHUTTER8_KEYNUMFLG)) {
		OpenGate(SHUTTER8);
		PlayBackSound(MAINSCENE_SE_SHUTTER, false, 100);
		g_GateKeyNum[BOSS2_SHUTTER8_KEYNUM] = -1;
		g_GateKeyNum[DUMMY_______SHUTTER8_KEYNUM] = -1;
	}
	if (gateflg & KEYNUM_SWITCHA_FLG) {
		OpenGate(SHUTTER_A);
		PlayBackSound(MAINSCENE_SE_SHUTTERSWITCH, false, 100);
		g_GateKeyNum[SWITCH_A_KEYNUM] = -1;

	};
	if (gateflg & KEYNUM_SWITCHB_FLG) {
		OpenGate(SHUTTER_B);
		PlayBackSound(MAINSCENE_SE_SHUTTERSWITCH, false, 100);
		g_GateKeyNum[SWITCH_B_KEYNUM] = -1;

	};
	if (gateflg & KEYNUM_SWITCHC_FLG) {
		OpenGate(SHUTTER_C);
		PlayBackSound(MAINSCENE_SE_SHUTTERSWITCH, false, 100);
		g_GateKeyNum[SWITCH_C_KEYNUM] = -1;

	};

	bool bossActiveflg = CheckBossActiveBoss();

	if (bossActiveflg && !g_BossGateflg) {
		
		CloseGate(BOSS_SHUTTER, BOSS_SHUTTER);
		PlayBackSound(MAINSCENE_SE_BOSSSPAWN1, false, 100);
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
	free(pGimmickData);
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
	free(pGimmickData);
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

	basePoint->y = 544;
	
	D3DXVECTOR2 BasePoint0 = D3DXVECTOR2(DISPLAY_WIDTH / 2, DISPLAY_HEIGHT / 2);
	player->WindowPos.x = player->WorldPos.x - (basePoint->x - BasePoint0.x);
	player->WindowPos.y = player->WorldPos.y - (basePoint->y - BasePoint0.y);

	player->beActive = true;
	SetStopScroll(false);
	EnemyInit();
	BulletInit();
	Boss1Init();
	Boss2Init();
	Boss3Init();
	Boss4Init();
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
	Boss2Data* pBoss2 = GetBoss2Data();
	Boss3Data* pBoss3 = GetBoss3Data();
	Boss4Data* pBoss4 = GetBoss4Data();

	bool bossActiveflg = false;
	if (pBoss1->isExistence && pBoss1->isActive) {
		bossActiveflg = pBoss1->isActive;
	}
	if (pBoss3->isExistence && pBoss3->isActive) {
		bossActiveflg = pBoss3->isActive;
	}
	if (pBoss2->isExistence && pBoss2->isActive) {
		bossActiveflg = pBoss2->isActive;
	}
	if (pBoss4->isExistence && pBoss4->isActive) {
		bossActiveflg = pBoss4->isActive;
	}

	return bossActiveflg;
}