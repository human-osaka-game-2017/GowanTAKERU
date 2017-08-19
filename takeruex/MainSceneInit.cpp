#include"MainSceneInit.h"
#include"PlayerControl.h"
#include"EnemyControl.h"
#include"BulletControl.h"
#include"Boss1Control.h"
#include"StageGimmick.h"
#include"Boss2Control.h"

void MainSceneInit() {
	PlayerInit();
	EnemyInit();
	BulletInit();
	Boss1Init();
	Boss2Init();
	StageGimmickInit();
}