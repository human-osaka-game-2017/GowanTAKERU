#include"StageGimmick.h"
#include"StageSelect.h"
#include"PlayerControl.h"
#include"FileManagement.h"
#include"MapRender.h"
#include"CommonRender.h"
#include"MapControl.h"
#include"BulletControl.h"
#include"EnemyControl.h"

void StageGimmickManage() {

	STAGE_ID stage_ID = GetStage_ID();
	Player* player = GetplayerData();
	int* map = GetMapData();
	STAGEXYMAX maxX = GetStageXYMAX(stage_ID, X);
	STAGEXYMAX maxY = GetStageXYMAX(stage_ID, Y);

	switch (stage_ID) {

	case STAGE_1:
		if (player->WorldPos.x >= 9342) {
			for (int i = 19; i <= 25; i++) {
				for (int j = 288; j <= 290; j++) {
					map[i*maxX + j] = FLOOR;
				}
			}
		}
	}

}

void ComeBackCheckPoint() {

	STAGE_ID stage_ID = GetStage_ID();
	STAGEXYMAX maxX = GetStageXYMAX(stage_ID, X);
	STAGEXYMAX maxY = GetStageXYMAX(stage_ID, Y);
	int* gimmickData = (int*)malloc(sizeof(int)*maxX*maxY);

	CSVLoad("CSV/mainscene/stage1_gimmick.csv", gimmickData, maxY, maxX);

	D3DXVECTOR2 pos[3];
	MapNumXY mapXY[3];
	for (int i = 0; i < maxY; i++) {
		for (int j = 0; j < maxX; j++) {
			if ((*(gimmickData + (i*maxX + j)) == CHECKPOINT_2)) {
				mapXY[2] = { j,i };
				PosSpecifyForMapchipNumber(&pos[2], &mapXY[2]);
			}
			if ((*(gimmickData + (i*maxX + j)) == CHECKPOINT_1)) {
				mapXY[1] = { j,i };
				PosSpecifyForMapchipNumber(&pos[1], &mapXY[1]);
			}
			if ((*(gimmickData + (i*maxX + j)) == START)) {
				mapXY[0] = { j,i };
				PosSpecifyForMapchipNumber(&pos[0], &mapXY[0]);
			}
		}
	}

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
	}
	
	D3DXVECTOR2 BasePoint0 = D3DXVECTOR2(DISPLAY_WIDTH / 2, DISPLAY_HEIGHT / 2);
	player->WindowPos.x = player->WorldPos.x - (basePoint->x - BasePoint0.x);
	player->WindowPos.y = player->WorldPos.y - (basePoint->y - BasePoint0.y);

	EnemyInit();
	BulletInit();
}