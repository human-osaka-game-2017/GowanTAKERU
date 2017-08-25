#include"MoveManagement.h"
#include"BulletControl.h"
#include"PlayerControl.h"
#include"EnemyControl.h"
#include"Boss1Control.h"
#include"Boss2Control.h"
#include"Boss3Control.h"
#include"Boss4Control.h"

void MoveManage() {
	MovePlayer();
	MoveBullet();
	MoveEnemy();
	MoveBoss1();
	//MoveBoss2();
	MoveBoss3();
	MoveBoss4();
}