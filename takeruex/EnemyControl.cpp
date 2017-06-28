#include"EnemyControl.h"
#include"MapRender.h"
#include"MapControl.h"
#include"CommonRender.h"
#include"CommonControl.h"
#include"MainControl.h"
#include"PlayerRender.h"
#include"PlayerControl.h"
#include "BulletControl.h"

Enemy g_enemy[ENEMYNUMBER];

Enemy* GetenemyData() {
		return g_enemy;
	}

void EnemyBulettCreate(int enemyNum);

void EnemyInit() {
	for (int i = 0; i < ENEMYNUMBER; i++) {
		
		g_enemy[i].WindowPos.x = 0;
		g_enemy[i].WindowPos.y = 0;
		g_enemy[i].Atk = 2;//攻撃力
		g_enemy[i].Hp = 1;//体力
		g_enemy[i].beDead = false;//死んでいるか
		g_enemy[i].beActive = false;//活動中か
		g_enemy[i].beLeft = false;//左（右）どうっち向いてるか
	}
	g_enemy[0].WorldPos.x = 600;//ワールド座標
	g_enemy[0].WorldPos.y = 702;
	g_enemy[1].WorldPos.x = 900;//ワールド座標
	g_enemy[1].WorldPos.y = 702;
}

void EnemyMove(int enemyNum) {
	Player* player = GetplayerData();
	//エネミーがアクティブでデスでなかったら中に入る
		//エネミーのX座標がプレイヤーのX座標より小さかったら
		if (player->WindowPos.x < g_enemy[enemyNum].WindowPos.x) {
			//+方向にエネミーを動かす
			g_enemy[enemyNum].WorldPos.x -= MOVE_SUPEED;
		}
		//エネミーのX座標がプレイヤーのX座標より大きかったら
		else if (player->WindowPos.x > g_enemy[enemyNum].WindowPos.x) {
			//-方向にエネミーを動かす
			g_enemy[enemyNum].WorldPos.x += MOVE_SUPEED;
		}
	}



void EnemyControl() {

	D3DXVECTOR2* basepoint = GetBasePoint();
	for (int i = 1; i < ENEMYNUMBER; i++) {
		if (g_enemy[i].beDead == false) {
			//ウィンドウの外（左右）120ピクセルから描画開始のフラグを立てる
			if (g_enemy[i].WorldPos.x < basepoint->x + DISPLAY_WIDTH / 2 + 120 && g_enemy[i].WorldPos.x > basepoint->x - DISPLAY_WIDTH / 2 - 120) {
				g_enemy[i].beActive = true;
			}
			if (g_enemy[i].beActive == true && g_enemy[i].beDead == false) {

				//エネミーとベースポイントとのworld,X座標の距離を調べる
				float EnemyWorldDistanceX = g_enemy[i].WorldPos.x - basepoint->x;
				//エネミーとベースポイントとのworld,Y座標の距離を調べる
				float EnemyWorldDistanceY = g_enemy[i].WorldPos.y - basepoint->y;
				//エネミーのwindow,X座標を調べる
				g_enemy[i].WindowPos.x = DISPLAY_WIDTH / 2 + EnemyWorldDistanceX;
				//エネミーのwindow,Y座標を調べる
				g_enemy[i].WindowPos.y = DISPLAY_HEIGHT / 2 + EnemyWorldDistanceY;
				EnemyMove(i);

				EnemyBulettCreate(i);
			}
		}
	}
}


void EnemyBulettCreate(int enemyNum) {

	Bullet* bullt= GetBullet();

	static int FrameCount = 0;
	FrameCount++;

	if (FrameCount == 300) {//5秒（300フレーム）に一回入るはず

		static int bulletRest = 0;//弾の撃った数を図る変数
		BulletCreate(bulletRest, enemyNum,bullet01);
		bulletRest++;

		if (bulletRest == BULLETNUMBER) {//弾の装填数がなくなったら初期化
			bulletRest = 0;
		}

		FrameCount = 0;
	}
}
