#include"MainScene.h"
#include"StageSelect.h"
#include"FileManagement.h"
#include"MainRender.h"
#include"MainControl.h"
#include"MainSceneInit.h"
#include"DirectXSound.h"
#include"PlayerControl.h"


SCENE_ID RunMainScene() {

	SCENE_ID scene = MAINSCENE;
	Player* pPlayer = GetplayerData();

	static int step = 0;
	switch (step) {

	case 0:
		StageSelect();
		MainSceneLoad(GetStage_ID());
		MainSceneInit();
		PlayBackSound(SOUND02, true, 100);
		step++;
		break;

	case 1:
		MainControl();
		MainRender();

		/*if (pPlayer->LifeReduced == 0) {
				scene = GAMEOVER;
				step++;
			}*/
		//if(bossŽ€–S)step++
		//if(stage4bossŽ€–S)scene = GAMECLEAR
		break;

	case 2:
		ReleseMapData();
		TextureFree();
		ReleaseBuffer();
		step=0;
		break;
	}

	return scene;
}