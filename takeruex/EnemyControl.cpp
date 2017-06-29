#include"EnemyControl.h"
#include"MapRender.h"
#include"MapControl.h"
#include"CommonRender.h"
#include"CommonControl.h"
#include"MainControl.h"
#include"PlayerRender.h"
#include"PlayerControl.h"
#include "BulletControl.h"
#include "FileManagement.h"

Enemy g_enemy[ENEMYNUMBER];
struct EnemyMapNum {
	int NumX;
	int NumY;
	EnemyKind enemyKind;

};



Enemy* GetenemyData() {
		return g_enemy;
	}
//プロトタイプ群
void EnemyBulettCreate(int enemyNum);
void EnemyMove(int enemyNuintq);
void EnemyArrangement(EnemyMapNum enemyMapNum[]);

void EnemyInit() {
	EnemyMapNum enemyMapNum[ENEMYNUMBER];
	EnemyArrangement(enemyMapNum);
	for (int i = 0; i < ENEMYNUMBER; i++) {
		g_enemy[i].WorldPos.x = enemyMapNum[i].NumX*TIPSIZE;//ワールド座標
		g_enemy[i].WorldPos.y = enemyMapNum[i].NumY*TIPSIZE;
		g_enemy[i].enemyKind = enemyMapNum[i].enemyKind;
		g_enemy[i].WindowPos.x = 0;
		g_enemy[i].WindowPos.y = 0;
		g_enemy[i].Atk = 2;//攻撃力
		g_enemy[i].Hp = 1;//体力
		g_enemy[i].bulletFreamCount = 0;
		g_enemy[i].beDead = false;//死んでいるか
		g_enemy[i].beActive = false;//活動中か
		g_enemy[i].beLeft = false;//左（右）どうっち向いてるか

	}

}

void EnemyControl() {

	D3DXVECTOR2* basepoint = GetBasePoint();
	for (int i = 0; i < ENEMYNUMBER; i++) {
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
				g_enemy[i].bulletFreamCount++;
				if (g_enemy[i].bulletFreamCount == 300) {//300フレームに1回入るはず
					EnemyBulettCreate(i);
					g_enemy[i].bulletFreamCount = 0;
				}
			}
		}
	}
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




void EnemyBulettCreate(int enemyNum) {
	Bullet* bullt= GetBullet();

		static int bulletRest = 0;//弾の撃った数を図る変数
		BulletCreate(bulletRest, enemyNum, bullet01);
		bulletRest++;

		if (bulletRest == BULLETNUMBER) {//弾の装填数がなくなったら初期化
			bulletRest = 0;
		}

}

void EnemyMovingRange() {//エネミーと壁の判定

} 


void EnemyArrangement(EnemyMapNum enemyMapNum[]) {//エネミーのCSVからの表示

	int enemyArrangement[MAPCHIPNUM_HEIGHT*MAPCHIPNUM_WIDTH];
	CSVLoad("CSV/mainscene/enemyArrangement.csv", enemyArrangement, MAPCHIPNUM_HEIGHT, MAPCHIPNUM_WIDTH);//マップチップ呼び出し

	int count = 0;

	for (int i = 0; i < MAPCHIPNUM_HEIGHT;i++) {
		for (int j = 0; j < MAPCHIPNUM_WIDTH; j++) {
			switch (enemyArrangement[j+i*MAPCHIPNUM_WIDTH]) {
			case enemyKind01:
				enemyMapNum[count].NumX = j;
				enemyMapNum[count].NumY = i;
				enemyMapNum[count].enemyKind = enemyKind01;
				count++;
				break;

			default:
				break;
			}
		}

	}


}

////第一引数ーーーーMapNumXYの構造体でXのマップチップ番号、Yのマップチップ番号を返してくれます
////第二引数ーーーーワールド座標を入れてください
//void MapchipNumberSpecify(MapNumXY* pMapNumXY, D3DXVECTOR2* pWorldPos);
//
////第一引数ーーーーマップチップの番号(MapNumXY)をいれて
////第二引数ーーーーワールド座標を返してくれます
//void PosSpecifyForMapchipNumber(D3DXVECTOR2* pWorldPos, MapNumXY* pMapNumXY);
//
////マップチップの番号(MapNumXY)をいれて、そのマップチップの種類を返してくれます
//int MapKindSpecify(MapNumXY* pMapNumXY);
//
////第一引数ーーーーマップチップの番号(MapNumXY)をいれて
////第二引数ーーーー方向（Direction）を指定するとその方向のマップチップの種類を返してくれます
//int MapKindSpecify_Plus1(MapNumXY* pMapNumXY, Direction direction);
