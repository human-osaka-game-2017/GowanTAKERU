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
	g_enemy[0].WorldPos.x=600;//ワールド座標
	g_enemy[0].WorldPos.y = 400;
	g_enemy[0].WindowPos.x = 0;
	g_enemy[0].WindowPos.y = 0;
	g_enemy[0].Atk=2;//攻撃力
	g_enemy[0].Hp=1;//体力
	g_enemy[0].beDead=false;//死んでいるか
	g_enemy[0].beActive=false;//活動中か
	g_enemy[0].beLeft = false;//左（右）どうっち向いてるか



}

void EnemyMove(int enemyNum) {
	Player* player = GetplayerData();

	//エネミーのX座標がプレイヤーのX座標より小さかったら
	if (player->WindowPos.x < g_enemy[enemyNum].WindowPos.x) {
		//+方向にエネミーを動かす
		g_enemy[enemyNum].WindowPos.x -= MOVE_SUPEED;
	}
	//エネミーのX座標がプレイヤーのX座標より大きかったら
	else if (player->WindowPos.x > g_enemy[enemyNum].WindowPos.x) {
		//-方向にエネミーを動かす
		g_enemy[enemyNum].WindowPos.x += MOVE_SUPEED;
	}
}
void EnemyControl() {  


		
	D3DXVECTOR2* basepoint = GetBasePoint();
	for (int i = 0; i < ENEMYNUMBER ; i++) {
		//ウィンドウの外（左右）120ピクセルから描画開始のフラグを立てる
		if (g_enemy[i].WorldPos.x < basepoint->x + DISPLAY_WIDTH / 2 + 120 && g_enemy[i].WorldPos.x > basepoint->x - DISPLAY_WIDTH / 2 - 120) {
			g_enemy[i].beActive = true;
		}

		EnemyMove(i);

		//エネミーとベースポイントとのworld,X座標の距離を調べる
		float EnemyWorldDistance = g_enemy[i].WorldPos.x - basepoint->x;
		//エネミーのwindow,X座標を調べる
		g_enemy[i].WindowPos.x = basepoint->x - DISPLAY_WIDTH / 2 + EnemyWorldDistance;



	}
}
