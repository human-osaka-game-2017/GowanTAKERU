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
#include"StageGimmick.h"
#include"CommonControl.h"

bool g_isScreenTransition = false;

SCENE_ID RunMainScene(bool willbetrancefar) {

	KEYSTATE* Key = GetKey();

	SCENE_ID nextscene = MAINSCENE;
	Player* pPlayer = GetplayerData();
	BlackOutData* blackOutData = GetBlackOutData();
	Boss1Data* pBoss1 = GetBoss1Data();
	Boss3Data* pBoss3 = GetBoss3Data();
	Boss4Data* pBoss4 = GetBoss4Data();

	static int step = 0;


	if (willbetrancefar&&g_isScreenTransition) {
		step = 2;
	}

	switch (step) {

	case 0:
		//デバッグ用
		KeyCheck(&Key[KEY_T], DIK_T);
		if (Key[KEY_T] == KEY_ON) {
			StageSelect(STAGE_5);
		}

		MainSceneLoad(GetStage_ID());
		//MainSceneSoundLoad(false);
		MainSceneInit();
		//PlayBackSound(MAINSCENE_BGM01, true, 10);
		step++;
		break;

	case 1:
		MainControl();
		MainRender();

		if (pPlayer->Hp <= 0) {

			g_isScreenTransition = false;
			blackOutData->BlackOutflg = true;


			if (blackOutData->BlackOutNextState == BLACKOUT/* && !g_isScreenTransition*/) {
				pPlayer->Hp = 100;
				pPlayer->LifeReduced--;
				ComeBackCheckPoint();
			}
		}

		if (pPlayer->LifeReduced == 0) {
			g_isScreenTransition = true;
			nextscene = GAMEOVERSCENE;
			//StopSound(SOUND02);
		}

		//stage遷移
		if (pBoss1->isExistence&&pBoss1->isDead) {
			g_isScreenTransition = false;
			blackOutData->BlackOutflg = true;
			if (blackOutData->BlackOutNextState == BLACKOUT) {
				step++;
				StageSelect(STAGE_2);
			}
		}

		if (pBoss3->isExistence&&pBoss3->isDead) {
			g_isScreenTransition = false;
			blackOutData->BlackOutflg = true;
			StageSelect(STAGE_4);
			if (blackOutData->BlackOutNextState == BLACKOUT) {
				step++;
			}
		}

		if (pBoss4->isDead&&pBoss4->isExistence) {
			g_isScreenTransition = true;
			blackOutData->BlackOutflg = true;
			nextscene = GAMECLEARSCENE;
		}
		break;

	case 2:
		FreeEnemyData();
		FreeMapData();
		ReleaseTexture(MAINSCENE_TEXMAX);
		FreeTexture();
		//StopSound(MAINSCENE_BGM01);
		FreeBuffer();
		//ReleaseBuffer();
		step=0;
		break;
	}

	return nextscene;
}