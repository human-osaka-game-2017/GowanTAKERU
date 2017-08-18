#include"EnemyControl.h"
#include"EnemyRender.h"
#include"MapRender.h"
#include"MapControl.h"
#include"CommonRender.h"
#include"CommonControl.h"
#include"MainControl.h"
#include"PlayerRender.h"
#include"PlayerControl.h"
#include "BulletControl.h"
#include "FileManagement.h"
#include "MapRender.h"

EnemyMapDate g_EnemyMapDate;

EnemyMapDate* GetEnemyMapDate() {
	return &g_EnemyMapDate;
}

Enemy g_enemy[ENEMYNUMBER];
struct EnemyMapNum {//CSVの座標と番号を入れる箱
	int NumX;
	int NumY;

};

Enemy* GetenemyData() {
		return g_enemy;
	}

//プロトタイプ群
void EnemyBulettCreate(int enemyNum);
void EnemyPursuit(int enemyNum);
void EnemyArrangement(EnemyMapNum enemyMapNum[]);

//void EnemyCSVInit() {
//	g_EnemyMapDate.MaxDate = 0;
//	g_EnemyMapDate.HasKeyDate = 0;
//
//	STAGE_ID stage_ID = GetStage_ID();
//	int MaxX = GetStageXYMAX(stage_ID, X);
//	int MaxY = GetStageXYMAX(stage_ID, Y);
//	int* enemyArrangement = (int*)malloc(sizeof(int)*MaxX*MaxY);
//
//	switch (stage_ID) {
//	case STAGE_1:
//		CSVLoad("CSV/mainscene/stage1_gimmick.csv", enemyArrangement, MaxY, MaxX);//CSV呼び出し
//		break;
//	}
//
//	int enemyMaxCount = 0;
//	int enemyHasKyeCount = 0;
//
//	for (int i = 0; i < MaxY; i++) {
//		for (int j = 0; j < MaxX; j++) {
//			switch (enemyArrangement[j + i*MaxX]) {
//			case WALKINGENEMY_HAS_KEY_2:
//			case FLYINGENEMY_HAS_KEY1:
//			case FLYINGENEMY_HAS_KEY2:
//				enemyHasKyeCount;
//			case WALKINGENEMY_1:
//			case FLYINGENEMY1:
//			case FIXEDBATTERY1:
//				enemyMaxCount++;
//				break;
//			default:
//				break;
//			}
//		}
//		g_EnemyMapDate.MaxDate = enemyMaxCount;
//		g_EnemyMapDate.HasKeyDate = enemyHasKyeCount;
//	}
//
//	free();
//
//
//}

void EnemyInit() {

	EnemyMapNum enemyMapNum[ENEMYNUMBER];
	EnemyArrangement(enemyMapNum);
	for (int i = 0; i < ENEMYNUMBER; i++) {
		g_enemy[i].WorldPos.x = enemyMapNum[i].NumX*TIPSIZE;//ワールド座標
		g_enemy[i].WorldPos.y = enemyMapNum[i].NumY*TIPSIZE;
		g_enemy[i].WindowPos.x = 0;
		g_enemy[i].WindowPos.y = 0;
		g_enemy[i].Atk = 20;//攻撃力
		g_enemy[i].Hp = 1;//体力
		g_enemy[i].bulletFrameCount = 0;
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
				//EnemyGravity(i);
				EnemyPursuit(i);
				g_enemy[i].bulletFrameCount++;
				if (g_enemy[i].bulletFrameCount == g_enemy[i].firingInterval) {//エネミー事に持っているは発射感覚になったら入る
					EnemyBulettCreate(i);
					g_enemy[i].bulletFrameCount = 0;
				}
			}
		}
	}
}


void EnemyPursuit(int enemyNum) {
	Player* player = GetplayerData();
		switch (g_enemy[enemyNum].enemyKind) {
		case WALKINGENEMY_1:
		case WALKINGENEMY_HAS_KEY_2:
			if (g_enemy[enemyNum].bulletFrameCount < g_enemy[enemyNum].firingInterval - 5) {//弾発射フレームより-5フレーム未満だったら中に入る
				//エネミーのX座標がプレイヤーのX座標より小さかったら
				if (player->WindowPos.x < g_enemy[enemyNum].WindowPos.x) {
					//+方向にエネミーを動かす
					g_enemy[enemyNum].MovementX -= g_enemy[enemyNum].Speed;
				}
				//エネミーのX座標がプレイヤーのX座標より大きかったら
				else if (player->WindowPos.x > g_enemy[enemyNum].WindowPos.x) {
					//-方向にエネミーを動かす
					g_enemy[enemyNum].MovementX += g_enemy[enemyNum].Speed;
				}
			}
			if (g_enemy[enemyNum].bulletFrameCount >= g_enemy[enemyNum].firingInterval - 5) {//発射フレームの-5フレーム以上あれば中に入る
				g_enemy[enemyNum].MovementX = 0;
				g_enemy[enemyNum].MovementY = 0;
			}
			g_enemy[enemyNum].MovementY += GRAVITY;

			break;
		case FLYINGENEMY1:
		case FLYINGENEMY_HAS_KEY1:
		case FLYINGENEMY_HAS_KEY2:
			if (g_enemy[enemyNum].bulletFrameCount < g_enemy[enemyNum].firingInterval - 5) {//弾発射フレームより-5フレーム未満だったら中に入る
				//エネミーのX座標がプレイヤーのX座標+200の位置より大きかったら
				if (player->WindowPos.x + 200 < g_enemy[enemyNum].WindowPos.x) {
					//+方向にエネミーを動かす
					g_enemy[enemyNum].MovementX -= g_enemy[enemyNum].Speed;
				}
				//エネミーのX座標がプレイヤーのX座標-200の位置より小さかったら
				else if (player->WindowPos.x - 200 > g_enemy[enemyNum].WindowPos.x) {
					//+方向にエネミーを動かす
					g_enemy[enemyNum].MovementX += g_enemy[enemyNum].Speed;
				}
			}
			
			if (g_enemy[enemyNum].bulletFrameCount >= g_enemy[enemyNum].firingInterval - 5) {//発射フレームの-5フレーム以上あれば中に入る
				g_enemy[enemyNum].MovementX = 0;
				g_enemy[enemyNum].MovementY = 0;
			}

			break;
		}

	
}




void EnemyBulettCreate(int enemyNum) {
	Bullet* bullt= GetBullet();

		static int bulletRest = 0;//弾の撃った数を図る変数
		BulletCreate(bulletRest, enemyNum, BULLET01);
		bulletRest++;

		if (bulletRest == BULLETNUMBER ) {//弾の装填数がなくなったら初期化
			bulletRest = 0;
		}

}



void EnemyArrangement(EnemyMapNum enemyMapNum[]) {//CSVからエネミーの座標と種類をもらう

	STAGE_ID stage_ID = GetStage_ID();
	int MaxX = GetStageXYMAX(stage_ID, X);
	int MaxY = GetStageXYMAX(stage_ID, Y);
	int* enemyArrangement = (int*)malloc(sizeof(int)*MaxX*MaxY);

	switch (stage_ID) {
	case STAGE_1:
		CSVLoad("CSV/mainscene/stage1_gimmick.csv", enemyArrangement, MaxY, MaxX);//CSV呼び出し
		break;
	}

	int enemycount = 0;

	for (int i = 0; i < MaxY;i++) {
		for (int j = 0; j < MaxX; j++) {
			switch (enemyArrangement[j + i*MaxX]) {//もらった敵のデータを入れ込む
			case WALKINGENEMY_HAS_KEY_2:
				g_enemy[enemycount].enemyKind = WALKINGENEMY_HAS_KEY_2;
			case WALKINGENEMY_1:
				enemyMapNum[enemycount].NumX = j;
				enemyMapNum[enemycount].NumY = i;
				if (g_enemy[enemycount].enemyKind != WALKINGENEMY_HAS_KEY_2) {
					g_enemy[enemycount].enemyKind = WALKINGENEMY_1;
				}
				g_enemy[enemycount].Speed = 1;
				g_enemy[enemycount].firingInterval = 200;
				g_enemy[enemycount].size = 32;
				enemycount++;
				break;

			case FLYINGENEMY_HAS_KEY1:
				g_enemy[enemycount].enemyKind = FLYINGENEMY_HAS_KEY1;
			case FLYINGENEMY_HAS_KEY2:
				if (g_enemy[enemycount].enemyKind != FLYINGENEMY_HAS_KEY1){
					g_enemy[enemycount].enemyKind = FLYINGENEMY_HAS_KEY2;
				}
			case FLYINGENEMY1:
				enemyMapNum[enemycount].NumX = j;
				enemyMapNum[enemycount].NumY = i;
				if (g_enemy[enemycount].enemyKind != FLYINGENEMY_HAS_KEY1 || g_enemy[enemycount].enemyKind != FLYINGENEMY_HAS_KEY2) {
					g_enemy[enemycount].enemyKind = FLYINGENEMY1;
				}
				g_enemy[enemycount].Speed = 2;
				g_enemy[enemycount].firingInterval = 250;
				g_enemy[enemycount].size = 32;
				enemycount++;
				break;
			case FIXEDBATTERY1:
				enemyMapNum[enemycount].NumX = j;
				enemyMapNum[enemycount].NumY = i;
				g_enemy[enemycount].Speed = 0;
				g_enemy[enemycount].enemyKind = FIXEDBATTERY1;
				g_enemy[enemycount].firingInterval = 150;
				g_enemy[enemycount].size = 32 ;

			default:
				break;
			}
		}

	}

	free(enemyArrangement);

}


//}

void MoveEnemy() {
	for (int i = 0; i < g_EnemyMapDate.MaxDate; i++) {
		if (g_enemy[i].beActive == true && g_enemy[i].beDead == false) {//デス、アクティブチェック
			//各エネミーの座標に動く値を足す
			g_enemy[i].WorldPos.x += g_enemy[i].MovementX;
			g_enemy[i].WorldPos.y += g_enemy[i].MovementY;
			g_enemy[i].WindowPos.x += g_enemy[i].MovementX;
			g_enemy[i].WindowPos.y += g_enemy[i].MovementY;

			//Movementの初期化
			g_enemy[i].MovementX = 0;
			g_enemy[i].MovementY = 0;
		}
		
	 }
}