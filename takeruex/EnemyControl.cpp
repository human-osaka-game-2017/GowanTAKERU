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
//void EnemyGravity(int enemyNum);

void EnemyInit() {
	EnemyMapNum enemyMapNum[ENEMYNUMBER];
	EnemyArrangement(enemyMapNum);
	for (int i = 0; i < ENEMYNUMBER; i++) {
		g_enemy[i].WorldPos.x = enemyMapNum[i].NumX*TIPSIZE;//ワールド座標
		g_enemy[i].WorldPos.y = enemyMapNum[i].NumY*TIPSIZE;
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
				//EnemyGravity(i);
				EnemyPursuit(i);
				g_enemy[i].bulletFreamCount++;
				if (g_enemy[i].bulletFreamCount == g_enemy[i].firingInterval) {//エネミー事に持っているは発射感覚になったら入る
					EnemyBulettCreate(i);
					g_enemy[i].bulletFreamCount = 0;
				}
			}
		}
	}
}


void EnemyPursuit(int enemyNum) {
	Player* player = GetplayerData();
		switch (g_enemy[enemyNum].enemyKind) {
		case Walkingenemy:
		case Walkingenemy_Kye02:
			if (g_enemy[enemyNum].bulletFreamCount < g_enemy[enemyNum].firingInterval - 5) {//弾発射フレームより-5フレーム未満だったら中に入る
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
			if (g_enemy[enemyNum].bulletFreamCount >= g_enemy[enemyNum].firingInterval - 5) {//発射フレームの-5フレーム以上あれば中に入る
				g_enemy[enemyNum].MovementX = 0;
				g_enemy[enemyNum].MovementY = 0;
			}


			break;
		case Flyingenemy:
		case Flyingenemy_Kye01:
		case Flyingenemy_Kye02:
			if (g_enemy[enemyNum].bulletFreamCount < g_enemy[enemyNum].firingInterval - 5) {//弾発射フレームより-5フレーム未満だったら中に入る
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
			D3DXVECTOR2 tmpPos = g_enemy[enemyNum].WorldPos;
			if (g_enemy[enemyNum].MovementX < 0) {
				tmpPos.x += -ENEMYRESIZEWIDTH / 2;
				if (MapKindSpecifyForPos(&tmpPos) != NOTHING)
				{
					g_enemy[enemyNum].MovementX = 0;
				}
			}
			if (g_enemy[enemyNum].MovementX > 0) {
				tmpPos.x += ENEMYRESIZEWIDTH / 2;
				if (MapKindSpecifyForPos(&tmpPos) != NOTHING)
				{
					g_enemy[enemyNum].MovementX = 0;
				}
			}
			if (g_enemy[enemyNum].bulletFreamCount >= g_enemy[enemyNum].firingInterval - 5) {//発射フレームの-5フレーム以上あれば中に入る
				g_enemy[enemyNum].MovementX = 0;
				g_enemy[enemyNum].MovementY = 0;
			}

			break;
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



void EnemyArrangement(EnemyMapNum enemyMapNum[]) {//CSVからエネミーの座標と種類をもらう

	int enemyArrangement[MAPCHIPNUM_HEIGHT*MAPCHIPNUM_WIDTH];
	CSVLoad("CSV/mainscene/stage1_gimmick.csv", enemyArrangement, MAPCHIPNUM_HEIGHT, MAPCHIPNUM_WIDTH);//CSV呼び出し

	int count = 0;

	for (int i = 0; i < MAPCHIPNUM_HEIGHT;i++) {
		for (int j = 0; j < MAPCHIPNUM_WIDTH; j++) {
			switch (enemyArrangement[j + i*MAPCHIPNUM_WIDTH]) {//もらった敵のデータを入れ込む
			case Walkingenemy_Kye02:
				g_enemy[count].enemyKind = Walkingenemy_Kye02;
			case Walkingenemy:
				enemyMapNum[count].NumX = j;
				enemyMapNum[count].NumY = i;
				if (g_enemy[count].enemyKind != Walkingenemy_Kye02) {
					g_enemy[count].enemyKind = Walkingenemy;
				}
				g_enemy[count].Speed = 1;
				g_enemy[count].firingInterval = 200;
				count++;
				break;

			case Flyingenemy_Kye01:
				g_enemy[count].enemyKind = Flyingenemy_Kye01;
			case Flyingenemy_Kye02:
				if (g_enemy[count].enemyKind != Flyingenemy_Kye01){
					g_enemy[count].enemyKind = Flyingenemy_Kye02;
				}
			case Flyingenemy:
				enemyMapNum[count].NumX = j;
				enemyMapNum[count].NumY = i;
				if (g_enemy[count].enemyKind != Flyingenemy_Kye01|| g_enemy[count].enemyKind != Flyingenemy_Kye02) {
					g_enemy[count].enemyKind = Flyingenemy;
				}
				g_enemy[count].Speed = 2;
				g_enemy[count].firingInterval = 250;
				count++;
				break;

			default:
				break;
			}
		}

	}


}

//void EnemyGravity(int enemyNum) {//地上ﾀｲﾌﾟのエネミーに重力をかける
//		switch (g_enemy[enemyNum].enemyKind) {
//		case enemyKind01:
//			g_enemy[enemyNum].MovementY += GRAVITY;
//			break;
//		
//		default:
//			break;
//
//		}
//	
//}

void MoveEnemy() {
	for (int i = 0; i < ENEMYNUMBER; i++) {
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