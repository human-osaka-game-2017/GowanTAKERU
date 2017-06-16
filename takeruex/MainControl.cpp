#include"MainControl.h"
#include"PlayerControl.h"
#include"EnemyControl.h"
#include"BulletControl.h"
#include"MainHitManagement.h"

void MainControl() {

	PlayerControl();
	EnemyControl();
	BulletControl();
	HitManage();
}



double Calculate_rad(float x1, float y1, float x2, float y2) {

	double rad;

	double lengthX = x1 - x2;
	double lengthY = y1 - y2;

	if (lengthX < 0 && lengthY > 0) {
		rad = atan(lengthY / lengthX) + 180;
	}
	else if (lengthX < 0 && lengthY < 0) {
		rad = atan(lengthY / lengthX) + 180;
	}
	else {
		rad = atan(lengthY / lengthX);
	}

	return rad;
}