#include"MainScene.h"
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
#include"BulletControl.h"

bool g_isScreenTransition = false;

SCENE_ID RunMainScene(bool willbetrancefar) {

	KEYSTATE* Key = GetKey();

	SCENE_ID nextscene = MAINSCENE;
	Player* pPlayer = GetplayerData();
	BlackOutData* blackOutData = GetBlackOutData();
	Boss1Data* pBoss1 = GetBoss1Data();
	Boss2Data* pBoss2 = GetBoss2Data();
	Boss3Data* pBoss3 = GetBoss3Data();
	Boss4Data* pBoss4 = GetBoss4Data();

	static int step = 0;


	if (willbetrancefar&&g_isScreenTransition) {
		step = 2;
	}

	STAGE_ID stage_ID = GetStage_ID();

	RENDER_STATE rederState = WAITING;
	static int frcnt = 0;

	switch (step) {

	case 0:
		//デバッグ用
		KeyCheck(&Key[KEY_T], DIK_T);
		if (Key[KEY_T] == KEY_ON) {
			StageSelect(STAGE_5);
		}
		frcnt = 0;
		stage_ID = GetStage_ID();
		MainSceneLoad(stage_ID);
		PlayBGM(stage_ID, false);
		//MainSceneSoundLoad(false);
		MainSceneInit();
		step++;
		break;

	case 1:
		if (stage_ID != STAGE_1) {
			if (frcnt == 0) {
				rederState = STARTUP;
				frcnt++;
			}
			else if (frcnt < 300) {
				rederState = RUNNING;
				frcnt++;
			}
		}
		

		MainControl(/*rederState*/);
		MainRender(rederState);

		if (pPlayer->Hp <= 0) {

			pPlayer->beActive = false;
			g_isScreenTransition = false;
			blackOutData->BlackOutflg = true;


			if (blackOutData->BlackOutNextState == BLACKOUT/* && !g_isScreenTransition*/) {
				pPlayer->Hp = 100;
				pPlayer->LifeReduced--;
				ComeBackCheckPoint();
			}
		}

		if (pPlayer->LifeReduced == 0) {
			StageSelect(STAGE_1);
			g_isScreenTransition = true;
			nextscene = GAMEOVERSCENE;
			StopBGM(stage_ID, true);
			StopBGM(stage_ID, false);
		}

		//stage遷移
		if (pBoss1->isExistence&&pBoss1->goNextStage) {
			g_isScreenTransition = false;
			blackOutData->BlackOutflg = true;
			if (blackOutData->BlackOutNextState == BLACKOUT) {
				step++;
				StageSelect(STAGE_2);
			}
		}

		if (pBoss2->isExistence&&pBoss2->goNextStage) {
			g_isScreenTransition = false;
			blackOutData->BlackOutflg = true;
			if (blackOutData->BlackOutNextState == BLACKOUT) {
				step++;
				StageSelect(STAGE_3);
			}
		}

		if (pBoss3->isExistence&&pBoss3->goNextStage) {
			g_isScreenTransition = false;
			blackOutData->BlackOutflg = true;
			if (blackOutData->BlackOutNextState == BLACKOUT) {
				step++;
				StageSelect(STAGE_4);
			}
		}

		if (pBoss4->goNextStage&&pBoss4->isExistence) {
			StopBGM(stage_ID, true);
			g_isScreenTransition = true;
			blackOutData->BlackOutflg = true;
			nextscene = GAMECLEARSCENE;
			StageSelect(STAGE_1);
		}
		break;

	case 2:
		DeleteALLBullet();
		FreeEnemyData();
		FreeMapData();
		ReleaseTexture(MAINSCENE_TEXMAX);
		FreeTexture();
		
		//FreeBuffer();
		//ReleaseBuffer();
		step=0;
		break;
	}

	return nextscene;
}

void PlayBGM(STAGE_ID stage_ID, bool boss) {
	if (!boss) {
		switch (stage_ID) {
		case STAGE_1:
			PlayBackSound(MAINSCENE_STAGEBGM01, true, 10);
			break;
		case STAGE_2:
			PlayBackSound(MAINSCENE_STAGEBGM02, true, 10);
			break;
		case STAGE_3:
			PlayBackSound(MAINSCENE_STAGEBGM03, true, 10);
			break;
		case STAGE_4:
			PlayBackSound(MAINSCENE_STAGEBGM04, true, 10);
			break;
		case STAGE_5:
			PlayBackSound(MAINSCENE_STAGEBGM03, true, 10);
			break;
		}
	}
	else {
		switch (stage_ID) {
		case STAGE_1:
		case STAGE_3:
		case STAGE_5:
			PlayBackSound(MAINSCENE_BOSSBGM01, true, 10);
			break;
		case STAGE_2:
			PlayBackSound(MAINSCENE_BOSSBGM02, true, 10);
			break;
		case STAGE_4:
			PlayBackSound(MAINSCENE_BOSSBGM03, true, 10);
			break;
		}
	}
}

void StopBGM(STAGE_ID stage_ID, bool boss) {
	if (!boss) {
		switch (stage_ID) {
		case STAGE_1:
			StopSound(MAINSCENE_STAGEBGM01);
			break;
		case STAGE_2:
			StopSound(MAINSCENE_STAGEBGM02);
			break;
		case STAGE_3:
			StopSound(MAINSCENE_STAGEBGM03);
			break;
		case STAGE_4:
			StopSound(MAINSCENE_STAGEBGM04);
			break;
		case STAGE_5:
			StopSound(MAINSCENE_STAGEBGM03);
			break;
		}
	}
	else {
		switch (stage_ID) {
		case STAGE_1:
		case STAGE_3:
		case STAGE_5:
			StopSound(MAINSCENE_BOSSBGM01);
			break;
		case STAGE_2:
			StopSound(MAINSCENE_BOSSBGM02);
			break;
		case STAGE_4:
			StopSound(MAINSCENE_BOSSBGM03);
			break;
		}
	}
}