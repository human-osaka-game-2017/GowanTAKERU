#include"MainScene.h"
#include"StageSelect.h"
#include"FileManagement.h"
#include"MainRender.h"
#include"MainControl.h"
#include"MainSceneInit.h"
#include"DirectXSound.h"
#include"PlayerControl.h"


SCENE_ID RunMainScene(bool willbetrancefar) {

	SCENE_ID nextscene = MAINSCENE;
	Player* pPlayer = GetplayerData();

	static int step = 0;


	if (willbetrancefar) {
		step = 2;
	}

	switch (step) {

	case 0:
		//StageSelect();
		MainSceneLoad(GetStage_ID());
		MainSceneInit();
		PlayBackSound(SOUND02, true, 100);
		step++;
		break;

	case 1:
		MainControl();
		MainRender();

		if (pPlayer->LifeReduced == 0) {
			nextscene = GAMEOVERSCENE;
			StopSound(SOUND02);
			}
		//if(bossŽ€–S)step++
		//if(stage4bossŽ€–S)scene = GAMECLEAR
		break;

	case 2:
		FreeMapData();
		ReleaseTexture(MAINSCENE_TEXMAX);
		FreeTexture();
		//ReleaseBuffer();
		step=0;
		break;
	}

	return nextscene;
}