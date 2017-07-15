#include"StageGimmick.h"
#include"StageSelect.h"
#include"PlayerControl.h"
#include"FileManagement.h"
#include"MapRender.h"

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