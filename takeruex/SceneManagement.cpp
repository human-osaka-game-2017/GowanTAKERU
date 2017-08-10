#include "SceneManagement.h"
#include"MainScene.h"

void SceneManage() {
	static SCENE_ID scene_ID;

	switch (scene_ID) {
	case MAINSCENE:
		scene_ID = RunMainScene();
		break;
		
	}


}
