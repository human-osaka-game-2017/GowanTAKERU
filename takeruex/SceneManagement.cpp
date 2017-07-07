#include "SceneManagement.h"
#include"StageSelect.h"
#include"FileManagement.h"
#include"MainRender.h"
#include"MainControl.h"
#include"MainSceneInit.h"

void MainScene();

void SceneManage() {
	static SCENE_ID scene_ID;

	switch (scene_ID) {
	case MAINSCENE:
		MainScene();
		//if ƒV[ƒ“Ø‚è‘Ö‚¦ğŒ
		break;
		
	}


}

void MainScene() {
	static int step = 0;
	switch (step) {

	case 0:
		StageSelect();
		MainSceneLoad(GetStage());
		MainSceneInit();
		step++;
		break;

	case 1:
		MainControl();
		MainRender();

		//if  step++;
		break;

	case 2:
		TextureFree();
		//step=0;
		//break;
	}
}