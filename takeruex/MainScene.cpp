#include"MainScene.h"
#include"StageSelect.h"
#include"FileManagement.h"
#include"MainRender.h"
#include"MainControl.h"
#include"MainSceneInit.h"
#include"DirectXSound.h"
#include"PlayerControl.h"
#include"EnemyControl.h"
#include"BlackOutRender.h"
#include"Boss1Control.h"
#include"Boss2Control.h"
#include"Boss3Control.h"
#include"Boss4Control.h"


SCENE_ID RunMainScene(bool willbetrancefar) {

	SCENE_ID nextscene = MAINSCENE;
	Player* pPlayer = GetplayerData();
	BlackOutData* blackOutData = GetBlackOutData();
	Boss1Data* pBoss1 = GetBoss1Data();
	Boss3Data* pBoss3 = GetBoss3Data();
	Boss4Data* pBoss4 = GetBoss4Data();


	static int step = 0;


	if (willbetrancefar) {
		step = 2;
	}

	switch (step) {

	case 0:
		//StageSelect();
		MainSceneLoad(GetStage_ID());
		MainSceneInit();
		//PlayBackSound(SOUND02, true, 10);
		step++;
		break;

	case 1:
		MainControl();
		MainRender();

		if (pPlayer->LifeReduced == 0) {
			nextscene = GAMEOVERSCENE;
			//StopSound(SOUND02);
		}

		if (pBoss1->isExistence&&pBoss1->isDead) {
			blackOutData->BlackOutflg = true;
			if (blackOutData->BlackOutNextState == BLACKOUT) {
				StageSelect(STAGE_2);
				step++;
			}
		}

		if (pBoss3->isExistence&&pBoss3->isDead) {
			blackOutData->BlackOutflg = true;
			StageSelect(STAGE_4);
			if (blackOutData->BlackOutNextState == BLACKOUT) {
				step++;
			}
		}

		if (pBoss4->isDead&&pBoss4->isExistence) {
			blackOutData->BlackOutflg = true;
			nextscene = GAMECLEARSCENE;
		}
		break;

	case 2:
		FreeEnemyData();
		FreeMapData();
		ReleaseTexture(MAINSCENE_TEXMAX);
		FreeTexture();
		//ReleaseBuffer();
		step=0;
		break;
	}

	return nextscene;
}