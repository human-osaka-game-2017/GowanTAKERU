#include"MoveManagement.h"
#include"BulletControl.h"
#include"PlayerControl.h"
#include"EnemyControl.h"

void MoveManage() {
	MovePlayer();
	MoveBullet();
	MoveEnemy();
}