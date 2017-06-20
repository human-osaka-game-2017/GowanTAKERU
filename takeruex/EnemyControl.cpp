#include"EnemyControl.h"
#include"MapRender.h"
#include"MapControl.h"
#include"CommonRender.h"
#include"CommonControl.h"
#include"MainControl.h"
#include"PlayerRender.h"
#include"PlayerControl.h"

Enemy g_enemy[ENEMYNUMBER];

Enemy* GetenemyData() {
		return g_enemy;
	}

void EnemyInit() {

}

////Player* GetplayerData()
////D3DXVECTOR2* GetBasePoint()
//void EnemyMove() {
//	Player* player = GetplayerData();
//
//
//	//エネミーのX座標がプレイヤーのX座標より小さかったら
//	if (player->WindowPos.x < g_enemy.WindowPos.x) {
//		//+方向にエネミーを動かす
//		g_enemy.WindowPos.x += MOVE_SUPEED;
//	}
//	//エネミーのX座標がプレイヤーのX座標より大きかったら
//	else if (player->WindowPos.x > g_enemy.WindowPos.x) {
//		//-方向にエネミーを動かす
//		g_enemy.WindowPos.x -= MOVE_SUPEED;
//	}
//}
//
void EnemyControl() {

}
