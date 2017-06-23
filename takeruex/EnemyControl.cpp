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

void EnemyBulettCreate();

void EnemyInit() {
	g_enemy[0].WorldPos.x=600;//ワールド座標
	g_enemy[0].WorldPos.y = 702;
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
	for (int i = 0; i < ENEMYNUMBER ; i++) {
		//ウィンドウの外（左右）120ピクセルから描画開始のフラグを立てる
		if (g_enemy[i].WorldPos.x < basepoint->x + DISPLAY_WIDTH / 2 + 120 && g_enemy[i].WorldPos.x > basepoint->x - DISPLAY_WIDTH / 2 - 120) {
			g_enemy[i].beActive = true;
		}

		EnemyMove(i);

		//エネミーとベースポイントとのworld,X座標の距離を調べる
		float EnemyWorldDistanceX = g_enemy[i].WorldPos.x - basepoint->x;
		//エネミーとベースポイントとのworld,Y座標の距離を調べる
		float EnemyWorldDistanceY = g_enemy[i].WorldPos.y - basepoint->y;
		//エネミーのwindow,X座標を調べる
		g_enemy[i].WindowPos.x = DISPLAY_WIDTH / 2 + EnemyWorldDistanceX;
		//エネミーのwindow,Y座標を調べる
		g_enemy[i].WindowPos.y = DISPLAY_HEIGHT / 2 + EnemyWorldDistanceY;

		EnemyBulettCreate();

	}
}

void EnemyBulettCreate() {

	Bullet* bullt= GetBullet();

	static int FrameCount = 0;
	FrameCount++;

	if (FrameCount == 300) {//5秒（300フレーム）に一回入るはず

		static int bulletRest = 0;//弾の撃った数を図る変数
		BulletCreate(bulletRest,bullet01);
		bulletRest++;

		if (bulletRest == BULLETNUMBER) {//弾の装填数がなくなったら初期化
			bulletRest = 0;
		}

		FrameCount = 0;
	}
}
