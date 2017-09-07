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

//プロトタイプ群
void SetEnemyData(int maxX, int maxY, int* pGimmickData);
void EnemyPursuit(int enemyNum);

EditableEnemyData g_EnemyInitialData[ENEMY_KIND_MAX];
Enemy* g_pEnemy;//

int g_EnemyMaxCount = 0;

void LeadEnemyData() {
	LoadEnemyDataCSV(g_EnemyInitialData);
}

Enemy* GetEnemyData() {
	return g_pEnemy;
}

void FreeEnemyData() {
	free(g_pEnemy);
}

int GetEnemyMax() {
	return g_EnemyMaxCount;
}

void CountEnemy(int maxX,int maxY,int* pGimmickData) {

	g_EnemyMaxCount = 0;

	for (int i = 0; i < maxY; i++) {
		for (int j = 0; j < maxX; j++) {

			switch (pGimmickData[j + i*maxX]) {
			case S_STOPENEMY_SHUTTER1_KEY:
			case G_MOVEENEMY_SHUTTER2_KEY:
			case S_MOVEENEMY_SHUTTER2_KEY:
			case DUMMY_______SHUTTER3_KEY://未確定
			case DUMMY1______SHUTTER4_KEY://未確定
			case DUMMY2______SHUTTER4_KEY://未確定
			case G_MOVEENEMY_SHUTTER5_KEY:
			case G_MOVEENEMY_SHUTTER6_KEY:
			case DUMMY_______SHUTTER6_KEY://未確定
			case BOSS1_SHUTTER7_KEY:
			case BOSS2_SHUTTER8_KEY:
			case DUMMY_______SHUTTER8_KEY://未確定
			case G_STOPENEMY01://地上敵
			case G_STOPENEMY02:
			case G_STOPENEMY03:
			case G_STOPENEMY04:
			case G_MOVEENEMY01:
			case G_MOVEENEMY02:
			case G_MOVEENEMY03:
			case G_MOVEENEMY04:
			case G_LARIATENEMY01:
			case G_LARIATENEMY02:
			case S_STOPENEMY01://空中敵
			case S_STOPENEMY02:
			case S_STOPENEMY03:
			case S_MOVEENEMY01:
			case S_MOVEENEMY02:
			case S_MOVEENEMY03:
			case S_MOVEENEMY04:
			case FIXEDBATTERY1://固定砲台
			case FIXEDBATTERY2:
			case SWITCH_A:
			case SWITCH_B:
			case SWITCH_C:
				g_EnemyMaxCount++;
				break;
			}
		}
	}
	g_EnemyMaxCount++;
}

void EnemyInit() {

	STAGE_ID stage_ID = GetStage_ID();
	int maxX = GetStageXYMAX(stage_ID, X);
	int maxY = GetStageXYMAX(stage_ID, Y);
	int* pGimmickData = (int*)malloc(sizeof(int)*maxX*maxY);
	switch (stage_ID) {
	case STAGE_1:
		CSVLoad("CSV/mainscene/stage1_gimmick.csv", pGimmickData, maxY, maxX);
		break;

	case STAGE_2:
		CSVLoad("CSV/mainscene/stage2_gimmick.csv", pGimmickData, maxY, maxX);
		break;

	case STAGE_3:
		CSVLoad("CSV/mainscene/stage3_gimmick.csv", pGimmickData, maxY, maxX);
		break;

	case STAGE_4:
		CSVLoad("CSV/mainscene/stage4_gimmick.csv", pGimmickData, maxY, maxX);
		break;

	case STAGE_5:
		CSVLoad("CSV/mainscene/stage5_gimmick.csv", pGimmickData, maxY, maxX);
		break;
	}

	CountEnemy(maxX, maxY, pGimmickData);

	//g_pEnemyを魔ロック
	g_pEnemy = (Enemy*)malloc(sizeof(Enemy)*g_EnemyMaxCount);

	SetEnemyData(maxX, maxY, pGimmickData);

	free(pGimmickData);
}

void EnemyControl() {

	int enemyMax = GetEnemyMax();
	for (int i = 0; i < enemyMax; i++) {
		D3DXVECTOR2* basepoint = GetBasePoint();
		D3DXVECTOR2 BasePoint0 = D3DXVECTOR2(DISPLAY_WIDTH / 2, DISPLAY_HEIGHT / 2);
		if (g_pEnemy[i].beDead == false) {

			g_pEnemy[i].WindowPos.x = g_pEnemy[i].WorldPos.x - (basepoint->x - BasePoint0.x);
			g_pEnemy[i].WindowPos.y = g_pEnemy[i].WorldPos.y - (basepoint->y - BasePoint0.y);

			//ウィンドウの外（左右）120ピクセルから描画開始のフラグを立てる
			if (-(2 * TIPSIZE) < g_pEnemy[i].WindowPos.x && g_pEnemy[i].WindowPos.x < DISPLAY_WIDTH + (2 * TIPSIZE) &&
				-(2 * TIPSIZE) < g_pEnemy[i].WindowPos.y && g_pEnemy[i].WindowPos.y < DISPLAY_HEIGHT + (2 * TIPSIZE)) {
				g_pEnemy[i].beActive = true;
			}
			else {
				g_pEnemy[i].beActive = false;
			}

			if (g_pEnemy[i].beActive == true && g_pEnemy[i].beDead == false) {
				EnemyPursuit(i);
				g_pEnemy[i].bulletFrameCount++;
				if (g_pEnemy[i].bulletFrameCount == g_pEnemy[i].firingInterval) {//エネミー毎に持っている発射感覚になったら入る

					if (g_pEnemy[i].BulletKind / 10 == 0 || g_pEnemy[i].BulletKind / 10 == 2) {
						if (g_pEnemy[i].beLeft) {
							BulletCreate(g_pEnemy[i].WorldPos, g_pEnemy[i].BulletKind, g_pEnemy[i].BulletDeg);
						}
						else {
							if (g_pEnemy[i].BulletDeg < 180) {
								BulletCreate(g_pEnemy[i].WorldPos, g_pEnemy[i].BulletKind, 180.0f - g_pEnemy[i].BulletDeg);
							}
							else {
								BulletCreate(g_pEnemy[i].WorldPos, g_pEnemy[i].BulletKind, 540.0f - g_pEnemy[i].BulletDeg);
							}
						}
					}
					else {
						BulletCreate(g_pEnemy[i].WorldPos, g_pEnemy[i].BulletKind, g_pEnemy[i].BulletDeg);
					}

					g_pEnemy[i].bulletFrameCount = 0;

				}
			}
		}
	}
}


void EnemyPursuit(int enemyNum) {
	Player* player = GetplayerData();
	//床判定のための足元位置
	//                                           敵世界座標X－幅/2－１　　　　　　　　　　　　　　　　　　　　　　敵世界座標Y＋高さ/２＋１
	D3DXVECTOR2 EnemyLeftFoot = D3DXVECTOR2(g_pEnemy[enemyNum].WorldPos.x -g_pEnemy[enemyNum].Width/2 - 1, g_pEnemy[enemyNum].WorldPos.y + g_pEnemy[enemyNum].Height/2+1);//左足
	//                                            敵世界座標X＋幅/２＋１　　　　　　　　　　　　　　　　　　　　　　敵世界座標Y＋高さ/２＋１
	D3DXVECTOR2 EnemyRightFoot = D3DXVECTOR2(g_pEnemy[enemyNum].WorldPos.x + g_pEnemy[enemyNum].Width/2 + 1, g_pEnemy[enemyNum].WorldPos.y + g_pEnemy[enemyNum].Height/2+1);//右足

	switch (g_pEnemy[enemyNum].enemyKind) {
	case G_MOVEENEMY_SHUTTER2_KEY:
	case G_MOVEENEMY_SHUTTER5_KEY:
	case G_MOVEENEMY_SHUTTER6_KEY:
	case G_MOVEENEMY01:
	case G_MOVEENEMY02:
	case G_MOVEENEMY03:
	case G_MOVEENEMY04:
		if (g_pEnemy[enemyNum].WindowPos.x > player->WindowPos.x) {
			g_pEnemy[enemyNum].beLeft = true;
		}
		else {
			g_pEnemy[enemyNum].beLeft = false;
		}

		if (g_pEnemy[enemyNum].bulletFrameCount < g_pEnemy[enemyNum].firingInterval - 30) {//弾発射フレームより-5フレーム未満だったら中に入る																			  //エネミーのX座標がプレイヤーのX座標より小さかったら
			if (player->WindowPos.x < g_pEnemy[enemyNum].WindowPos.x) {
				if (MapKindSpecifyForPos(&EnemyLeftFoot) != NOTHING) {
					//+方向にエネミーを動かす
					g_pEnemy[enemyNum].MovementX = -g_pEnemy[enemyNum].Speed;
				}
			}
			//エネミーのX座標がプレイヤーのX座標より大きかったら
			if (player->WindowPos.x > g_pEnemy[enemyNum].WindowPos.x) {
				if (MapKindSpecifyForPos(&EnemyRightFoot) != NOTHING) {
					//-方向にエネミーを動かす
					g_pEnemy[enemyNum].MovementX = g_pEnemy[enemyNum].Speed;
				}
			}
		}
		if (g_pEnemy[enemyNum].bulletFrameCount <= 30 || g_pEnemy[enemyNum].bulletFrameCount >= g_pEnemy[enemyNum].firingInterval - 30) {//発射フレームの-5フレーム以上あれば中に入る
			g_pEnemy[enemyNum].MovementX = 0;
			g_pEnemy[enemyNum].MovementY = 0;
	
		}
		g_pEnemy[enemyNum].MovementY = ENEMYGRAVITY;

		break;

	case G_STOPENEMY01:
	case G_STOPENEMY02:
	case G_STOPENEMY03:
	case G_STOPENEMY04:
		if (g_pEnemy[enemyNum].WindowPos.x > player->WindowPos.x) {
			g_pEnemy[enemyNum].beLeft = true;
		}
		else {
			g_pEnemy[enemyNum].beLeft = false;
		}
		g_pEnemy[enemyNum].MovementX = 0;
		g_pEnemy[enemyNum].MovementY = ENEMYGRAVITY;
		break;

	case G_LARIATENEMY01://体当たり
	case G_LARIATENEMY02:
		 //敵POP地点よりー突進距離以上にプレイヤーがいる　かつ　エネミーのPOP地点より手前ににプレイヤーがいる　かつ　敵の世界座標がPOP座標より-突進距離以内の範囲　
		//かつ　前フレームの世界座標以下の場合にいるとき
		if (g_pEnemy[enemyNum].EnemyBasePoint.x - RushDistance <= player->WorldPos.x && g_pEnemy[enemyNum].EnemyBasePoint.x>player->WorldPos.x 
			&& g_pEnemy[enemyNum].WorldPos.x>= g_pEnemy[enemyNum].EnemyBasePoint.x - RushDistance&&g_pEnemy[enemyNum].FrontWorldPos.x>=g_pEnemy[enemyNum].WorldPos.x) {//PCが範囲内に入ったらフラグおｎ
			g_pEnemy[enemyNum].Rush = true;
		}
		//自機の世界座標が突進距離範囲外　またわ　敵の世界座標が突進範囲に達している　またわ　前フレームの世界座標より大きい場合　場合フラグOFF
		if (g_pEnemy[enemyNum].EnemyBasePoint.x - RushDistance >= player->WorldPos.x|| g_pEnemy[enemyNum].EnemyBasePoint.x- RushDistance >= g_pEnemy[enemyNum].WorldPos.x || g_pEnemy[enemyNum].FrontWorldPos.x<g_pEnemy[enemyNum].WorldPos.x) {//突進範囲に入るまでOFF　
			g_pEnemy[enemyNum].Rush = false;
		}
		if (g_pEnemy[enemyNum].Rush == true) {//フラグONならSPEED---
			g_pEnemy[enemyNum].MovementX -= g_pEnemy[enemyNum].Speed;
			g_pEnemy[enemyNum].MovementY = ENEMYGRAVITY;
	
		}
		//フラグOFFかつPOP位置じゃない場合
		if (g_pEnemy[enemyNum].Rush == false && g_pEnemy[enemyNum].WorldPos.x <= g_pEnemy[enemyNum].EnemyBasePoint.x) {
			g_pEnemy[enemyNum].MovementX = 2;
			g_pEnemy[enemyNum].MovementY = ENEMYGRAVITY;
	
		}
		//フラグがOFFで敵の世界座標がPOP地点と同じ場合動かない
		if (g_pEnemy[enemyNum].Rush == false && g_pEnemy[enemyNum].WorldPos.x==g_pEnemy[enemyNum].EnemyBasePoint.x) {
			g_pEnemy[enemyNum].MovementX = 0;
			g_pEnemy[enemyNum].MovementY = ENEMYGRAVITY;
			
		}
		if (g_pEnemy[enemyNum].Rush == true) {
			g_pEnemy[enemyNum].beLeft = true;
		}
		if (g_pEnemy[enemyNum].Rush != true) {
			if (g_pEnemy[enemyNum].WorldPos.x > player->WorldPos.x) {
				g_pEnemy[enemyNum].beLeft = true;
			}
			else {
				g_pEnemy[enemyNum].beLeft = false;
			}
		}
		break;

	case S_MOVEENEMY_SHUTTER2_KEY:
	case S_MOVEENEMY01:
	case S_MOVEENEMY02:
	case S_MOVEENEMY03:
	case S_MOVEENEMY04:
	
		if (g_pEnemy[enemyNum].WindowPos.x > player->WindowPos.x) {
			g_pEnemy[enemyNum].beLeft = true;
		}
		else {
			g_pEnemy[enemyNum].beLeft = false;
		}
		if (g_pEnemy[enemyNum].bulletFrameCount < g_pEnemy[enemyNum].firingInterval - 30) {//弾発射フレームより-30フレーム未満だったら中に入る
																						  //エネミーのX座標がプレイヤーのX座標+200の位置より大きかったら
			if (player->WindowPos.x + 200 < g_pEnemy[enemyNum].WindowPos.x) {
				//-方向にエネミーを動かす
				g_pEnemy[enemyNum].MovementX -= g_pEnemy[enemyNum].Speed;
			}
			//エネミーのX座標がプレイヤーのX座標-200の位置より小さかったら
			if (player->WindowPos.x - 200 > g_pEnemy[enemyNum].WindowPos.x) {
				//+方向にエネミーを動かす
				g_pEnemy[enemyNum].MovementX += g_pEnemy[enemyNum].Speed;
			}
		}

		if (g_pEnemy[enemyNum].bulletFrameCount <= 30 || g_pEnemy[enemyNum].bulletFrameCount >= g_pEnemy[enemyNum].firingInterval - 30) {//発射フレームの-5フレーム以上あれば中に入る
			g_pEnemy[enemyNum].MovementX = 0;
			g_pEnemy[enemyNum].MovementY = 0;

		}
		break;

	case S_STOPENEMY_SHUTTER1_KEY:
	case S_STOPENEMY01:
	case S_STOPENEMY02:
	case S_STOPENEMY03:

		if (g_pEnemy[enemyNum].WindowPos.x > player->WindowPos.x) {
			g_pEnemy[enemyNum].beLeft = true;
		}
		else {
			g_pEnemy[enemyNum].beLeft = false;
		}
		g_pEnemy[enemyNum].MovementX = 0;
		g_pEnemy[enemyNum].MovementY = 0;
		break;


	case FIXEDBATTERY1:
	case FIXEDBATTERY2:
		g_pEnemy[enemyNum].beLeft = false;
		g_pEnemy[enemyNum].MovementX = 0;
		g_pEnemy[enemyNum].MovementY = 0;
		break;

	case SWITCH_A:
	case SWITCH_B:
	case SWITCH_C:
		g_pEnemy[enemyNum].beLeft = false;
		g_pEnemy[enemyNum].MovementX = 0;
		g_pEnemy[enemyNum].MovementY = 0;
		break;
		}

		g_pEnemy[enemyNum].FrontWorldPos.x = g_pEnemy[enemyNum].WorldPos.x;
		g_pEnemy[enemyNum].FrontWorldPos.y = g_pEnemy[enemyNum].WorldPos.y;

}
	

void MoveEnemy() {
	int enemyMax = GetEnemyMax();
	for (int i = 0; i < enemyMax; i++) {
		if (g_pEnemy[i].beActive == true && g_pEnemy[i].beDead == false) {//デス、アクティブチェック
			//各エネミーの座標に動く値を足す
			g_pEnemy[i].WorldPos.x += g_pEnemy[i].MovementX;
			g_pEnemy[i].WorldPos.y += g_pEnemy[i].MovementY;
			g_pEnemy[i].WindowPos.x += g_pEnemy[i].MovementX;
			g_pEnemy[i].WindowPos.y += g_pEnemy[i].MovementY;

			//Movementの初期化
			g_pEnemy[i].MovementX = 0;
			g_pEnemy[i].MovementY = 0;
		}
		
	 }
}

void SetEnemyData(int maxX,int maxY, int* pGimmickData) {

	int enemyCount = 0;

	for (int i = 0; i < maxY; i++) {
		for (int j = 0; j < maxX; j++) {

			g_pEnemy[enemyCount].enemyKind = NOTHING;

			switch (pGimmickData[j + i*maxX]) {//もらった敵のデータを入れ込む

			case S_STOPENEMY_SHUTTER1_KEY:
				g_pEnemy[enemyCount].enemyKind = S_STOPENEMY_SHUTTER1_KEY;
				g_pEnemy[enemyCount].Hp = g_EnemyInitialData[0].Hp;
				g_pEnemy[enemyCount].Speed = g_EnemyInitialData[0].Speed;
				g_pEnemy[enemyCount].firingInterval = (int)(g_EnemyInitialData[0].ShotInterval * 60.0f);
				g_pEnemy[enemyCount].Height = g_EnemyInitialData[0].Height;
				g_pEnemy[enemyCount].Width = g_EnemyInitialData[0].Width;
				g_pEnemy[enemyCount].Atk = g_EnemyInitialData[0].Atk;
				g_pEnemy[enemyCount].tu = g_EnemyInitialData[0].tu;
				g_pEnemy[enemyCount].tv = g_EnemyInitialData[0].tv;
				g_pEnemy[enemyCount].BulletKind = g_EnemyInitialData[0].BulletKind;
				g_pEnemy[enemyCount].BulletDeg = g_EnemyInitialData[0].BulletDeg;
				break;

			case G_MOVEENEMY_SHUTTER2_KEY:
				g_pEnemy[enemyCount].enemyKind = G_MOVEENEMY_SHUTTER2_KEY;
				g_pEnemy[enemyCount].Hp = g_EnemyInitialData[1].Hp;
				g_pEnemy[enemyCount].Speed = g_EnemyInitialData[1].Speed;
				g_pEnemy[enemyCount].firingInterval = (int)(g_EnemyInitialData[1].ShotInterval * 60.0f);
				g_pEnemy[enemyCount].Height = g_EnemyInitialData[1].Height;
				g_pEnemy[enemyCount].Width = g_EnemyInitialData[1].Width;
				g_pEnemy[enemyCount].Atk = g_EnemyInitialData[1].Atk;
				g_pEnemy[enemyCount].tu = g_EnemyInitialData[1].tu;
				g_pEnemy[enemyCount].tv = g_EnemyInitialData[1].tv;
				g_pEnemy[enemyCount].BulletKind = g_EnemyInitialData[1].BulletKind;
				g_pEnemy[enemyCount].BulletDeg = g_EnemyInitialData[1].BulletDeg;
				break;

			case S_MOVEENEMY_SHUTTER2_KEY://未定
				g_pEnemy[enemyCount].enemyKind = S_MOVEENEMY_SHUTTER2_KEY;
				g_pEnemy[enemyCount].Hp = g_EnemyInitialData[2].Hp;
				g_pEnemy[enemyCount].Speed = g_EnemyInitialData[2].Speed;
				g_pEnemy[enemyCount].firingInterval = (int)(g_EnemyInitialData[2].ShotInterval * 60.0f);
				g_pEnemy[enemyCount].Height = g_EnemyInitialData[2].Height;
				g_pEnemy[enemyCount].Width = g_EnemyInitialData[2].Width;
				g_pEnemy[enemyCount].Atk = g_EnemyInitialData[2].Atk;
				g_pEnemy[enemyCount].tu = g_EnemyInitialData[2].tu;
				g_pEnemy[enemyCount].tv = g_EnemyInitialData[2].tv;
				g_pEnemy[enemyCount].BulletKind = g_EnemyInitialData[2].BulletKind;
				g_pEnemy[enemyCount].BulletDeg = g_EnemyInitialData[2].BulletDeg;
				break;

			case DUMMY_______SHUTTER3_KEY:
				g_pEnemy[enemyCount].enemyKind = DUMMY_______SHUTTER3_KEY;
				g_pEnemy[enemyCount].Hp = g_EnemyInitialData[3].Hp;
				g_pEnemy[enemyCount].Speed = g_EnemyInitialData[3].Speed;
				g_pEnemy[enemyCount].firingInterval = (int)(g_EnemyInitialData[3].ShotInterval * 60.0f);
				g_pEnemy[enemyCount].Height = g_EnemyInitialData[3].Height;
				g_pEnemy[enemyCount].Width = g_EnemyInitialData[3].Width;
				g_pEnemy[enemyCount].Atk = g_EnemyInitialData[3].Atk;
				g_pEnemy[enemyCount].tu = g_EnemyInitialData[3].tu;
				g_pEnemy[enemyCount].tv = g_EnemyInitialData[3].tv;
				g_pEnemy[enemyCount].BulletKind = g_EnemyInitialData[3].BulletKind;
				g_pEnemy[enemyCount].BulletDeg = g_EnemyInitialData[3].BulletDeg;
				break;

			case DUMMY1______SHUTTER4_KEY:
				g_pEnemy[enemyCount].enemyKind = DUMMY1______SHUTTER4_KEY;
				g_pEnemy[enemyCount].Hp = g_EnemyInitialData[4].Hp;
				g_pEnemy[enemyCount].Speed = g_EnemyInitialData[4].Speed;
				g_pEnemy[enemyCount].firingInterval = (int)(g_EnemyInitialData[4].ShotInterval * 60.0f);
				g_pEnemy[enemyCount].Height = g_EnemyInitialData[4].Height;
				g_pEnemy[enemyCount].Width = g_EnemyInitialData[4].Width;
				g_pEnemy[enemyCount].Atk = g_EnemyInitialData[4].Atk;
				g_pEnemy[enemyCount].tu = g_EnemyInitialData[4].tu;
				g_pEnemy[enemyCount].tv = g_EnemyInitialData[4].tv;
				g_pEnemy[enemyCount].BulletKind = g_EnemyInitialData[4].BulletKind;
				g_pEnemy[enemyCount].BulletDeg = g_EnemyInitialData[4].BulletDeg;
				break;

			case DUMMY2______SHUTTER4_KEY:
				g_pEnemy[enemyCount].enemyKind = DUMMY2______SHUTTER4_KEY;
				g_pEnemy[enemyCount].Hp = g_EnemyInitialData[5].Hp;
				g_pEnemy[enemyCount].Speed = g_EnemyInitialData[5].Speed;
				g_pEnemy[enemyCount].firingInterval = (int)(g_EnemyInitialData[5].ShotInterval * 60.0f);
				g_pEnemy[enemyCount].Height = g_EnemyInitialData[5].Height;
				g_pEnemy[enemyCount].Width = g_EnemyInitialData[5].Width;
				g_pEnemy[enemyCount].Atk = g_EnemyInitialData[5].Atk;
				g_pEnemy[enemyCount].tu = g_EnemyInitialData[5].tu;
				g_pEnemy[enemyCount].tv = g_EnemyInitialData[5].tv;
				g_pEnemy[enemyCount].BulletKind = g_EnemyInitialData[5].BulletKind;
				g_pEnemy[enemyCount].BulletDeg = g_EnemyInitialData[5].BulletDeg;
				break;

			case G_MOVEENEMY_SHUTTER5_KEY:
				g_pEnemy[enemyCount].enemyKind = G_MOVEENEMY_SHUTTER5_KEY;
				g_pEnemy[enemyCount].Hp = g_EnemyInitialData[6].Hp;
				g_pEnemy[enemyCount].Speed = g_EnemyInitialData[6].Speed;
				g_pEnemy[enemyCount].firingInterval = (int)(g_EnemyInitialData[6].ShotInterval * 60.0f);
				g_pEnemy[enemyCount].Height = g_EnemyInitialData[6].Height;
				g_pEnemy[enemyCount].Width = g_EnemyInitialData[6].Width;
				g_pEnemy[enemyCount].Atk = g_EnemyInitialData[6].Atk;
				g_pEnemy[enemyCount].tu = g_EnemyInitialData[6].tu;
				g_pEnemy[enemyCount].tv = g_EnemyInitialData[6].tv;
				g_pEnemy[enemyCount].BulletKind = g_EnemyInitialData[6].BulletKind;
				g_pEnemy[enemyCount].BulletDeg = g_EnemyInitialData[6].BulletDeg;
				break;

			case G_MOVEENEMY_SHUTTER6_KEY:
				g_pEnemy[enemyCount].enemyKind = G_MOVEENEMY_SHUTTER6_KEY;
				g_pEnemy[enemyCount].Hp = g_EnemyInitialData[7].Hp;
				g_pEnemy[enemyCount].Speed = g_EnemyInitialData[7].Speed;
				g_pEnemy[enemyCount].firingInterval = (int)(g_EnemyInitialData[7].ShotInterval * 60.0f);
				g_pEnemy[enemyCount].Height = g_EnemyInitialData[7].Height;
				g_pEnemy[enemyCount].Width = g_EnemyInitialData[7].Width;
				g_pEnemy[enemyCount].Atk = g_EnemyInitialData[7].Atk;
				g_pEnemy[enemyCount].tu = g_EnemyInitialData[7].tu;
				g_pEnemy[enemyCount].tv = g_EnemyInitialData[7].tv;
				g_pEnemy[enemyCount].BulletKind = g_EnemyInitialData[7].BulletKind;
				g_pEnemy[enemyCount].BulletDeg = g_EnemyInitialData[7].BulletDeg;
				break;

			case DUMMY_______SHUTTER6_KEY:
				g_pEnemy[enemyCount].enemyKind = DUMMY_______SHUTTER6_KEY;
				g_pEnemy[enemyCount].Hp = g_EnemyInitialData[8].Hp;
				g_pEnemy[enemyCount].Speed = g_EnemyInitialData[8].Speed;
				g_pEnemy[enemyCount].firingInterval = (int)(g_EnemyInitialData[8].ShotInterval * 60.0f);
				g_pEnemy[enemyCount].Height = g_EnemyInitialData[8].Height;
				g_pEnemy[enemyCount].Width = g_EnemyInitialData[8].Width;
				g_pEnemy[enemyCount].Atk = g_EnemyInitialData[8].Atk;
				g_pEnemy[enemyCount].tu = g_EnemyInitialData[8].tu;
				g_pEnemy[enemyCount].tv = g_EnemyInitialData[8].tv;
				g_pEnemy[enemyCount].BulletKind = g_EnemyInitialData[8].BulletKind;
				g_pEnemy[enemyCount].BulletDeg = g_EnemyInitialData[8].BulletDeg;
				break;

			case BOSS1_SHUTTER7_KEY:
				g_pEnemy[enemyCount].enemyKind = BOSS1_SHUTTER7_KEY;
				g_pEnemy[enemyCount].Hp = g_EnemyInitialData[9].Hp;
				g_pEnemy[enemyCount].Speed = g_EnemyInitialData[9].Speed;
				g_pEnemy[enemyCount].firingInterval = (int)(g_EnemyInitialData[9].ShotInterval * 60.0f);
				g_pEnemy[enemyCount].Height = g_EnemyInitialData[9].Height;
				g_pEnemy[enemyCount].Width = g_EnemyInitialData[9].Width;
				g_pEnemy[enemyCount].Atk = g_EnemyInitialData[9].Atk;
				g_pEnemy[enemyCount].tu = g_EnemyInitialData[9].tu;
				g_pEnemy[enemyCount].tv = g_EnemyInitialData[9].tv;
				g_pEnemy[enemyCount].BulletKind = g_EnemyInitialData[9].BulletKind;
				g_pEnemy[enemyCount].BulletDeg = g_EnemyInitialData[9].BulletDeg;
				break;

			case BOSS2_SHUTTER8_KEY:
				g_pEnemy[enemyCount].enemyKind = BOSS2_SHUTTER8_KEY;
				g_pEnemy[enemyCount].Hp = g_EnemyInitialData[10].Hp;
				g_pEnemy[enemyCount].Speed = g_EnemyInitialData[10].Speed;
				g_pEnemy[enemyCount].firingInterval = (int)(g_EnemyInitialData[10].ShotInterval * 60.0f);
				g_pEnemy[enemyCount].Height = g_EnemyInitialData[10].Height;
				g_pEnemy[enemyCount].Width = g_EnemyInitialData[10].Width;
				g_pEnemy[enemyCount].Atk = g_EnemyInitialData[10].Atk;
				g_pEnemy[enemyCount].tu = g_EnemyInitialData[10].tu;
				g_pEnemy[enemyCount].tv = g_EnemyInitialData[10].tv;
				g_pEnemy[enemyCount].BulletKind = g_EnemyInitialData[10].BulletKind;
				g_pEnemy[enemyCount].BulletDeg = g_EnemyInitialData[10].BulletDeg;
				break;

			case DUMMY_______SHUTTER8_KEY:
				g_pEnemy[enemyCount].enemyKind = DUMMY_______SHUTTER8_KEY;
				g_pEnemy[enemyCount].Hp = g_EnemyInitialData[11].Hp;
				g_pEnemy[enemyCount].Speed = g_EnemyInitialData[11].Speed;
				g_pEnemy[enemyCount].firingInterval = (int)(g_EnemyInitialData[11].ShotInterval * 60.0f);
				g_pEnemy[enemyCount].Height = g_EnemyInitialData[11].Height;
				g_pEnemy[enemyCount].Width = g_EnemyInitialData[11].Width;
				g_pEnemy[enemyCount].Atk = g_EnemyInitialData[11].Atk;
				g_pEnemy[enemyCount].tu = g_EnemyInitialData[11].tu;
				g_pEnemy[enemyCount].tv = g_EnemyInitialData[11].tv;
				g_pEnemy[enemyCount].BulletKind = g_EnemyInitialData[11].BulletKind;
				g_pEnemy[enemyCount].BulletDeg = g_EnemyInitialData[11].BulletDeg;
				break;

			case G_STOPENEMY01://未定
				g_pEnemy[enemyCount].enemyKind = G_STOPENEMY01;
				g_pEnemy[enemyCount].Hp = g_EnemyInitialData[12].Hp;
				g_pEnemy[enemyCount].Speed = g_EnemyInitialData[12].Speed;
				g_pEnemy[enemyCount].firingInterval = (int)(g_EnemyInitialData[12].ShotInterval * 60.0f);
				g_pEnemy[enemyCount].Height = g_EnemyInitialData[12].Height;
				g_pEnemy[enemyCount].Width = g_EnemyInitialData[12].Width;
				g_pEnemy[enemyCount].Atk = g_EnemyInitialData[12].Atk;
				g_pEnemy[enemyCount].tu = g_EnemyInitialData[12].tu;
				g_pEnemy[enemyCount].tv = g_EnemyInitialData[12].tv;
				g_pEnemy[enemyCount].BulletKind = g_EnemyInitialData[12].BulletKind;
				g_pEnemy[enemyCount].BulletDeg = g_EnemyInitialData[12].BulletDeg;
				break;

			case G_STOPENEMY02:
				g_pEnemy[enemyCount].enemyKind = G_STOPENEMY02;
				g_pEnemy[enemyCount].Hp = g_EnemyInitialData[13].Hp;
				g_pEnemy[enemyCount].Speed = g_EnemyInitialData[13].Speed;
				g_pEnemy[enemyCount].firingInterval = (int)(g_EnemyInitialData[13].ShotInterval * 60.0f);
				g_pEnemy[enemyCount].Height = g_EnemyInitialData[13].Height;
				g_pEnemy[enemyCount].Width = g_EnemyInitialData[13].Width;
				g_pEnemy[enemyCount].Atk = g_EnemyInitialData[13].Atk;
				g_pEnemy[enemyCount].tu = g_EnemyInitialData[13].tu;
				g_pEnemy[enemyCount].tv = g_EnemyInitialData[13].tv;
				g_pEnemy[enemyCount].BulletKind = g_EnemyInitialData[13].BulletKind;
				g_pEnemy[enemyCount].BulletDeg = g_EnemyInitialData[13].BulletDeg;
				break;

			case G_STOPENEMY03://未定
				g_pEnemy[enemyCount].enemyKind = G_STOPENEMY03;
				g_pEnemy[enemyCount].Hp = g_EnemyInitialData[14].Hp;
				g_pEnemy[enemyCount].Speed = g_EnemyInitialData[14].Speed;
				g_pEnemy[enemyCount].firingInterval = (int)(g_EnemyInitialData[14].ShotInterval * 60.0f);
				g_pEnemy[enemyCount].Height = g_EnemyInitialData[14].Height;
				g_pEnemy[enemyCount].Width = g_EnemyInitialData[14].Width;
				g_pEnemy[enemyCount].Atk = g_EnemyInitialData[14].Atk;
				g_pEnemy[enemyCount].tu = g_EnemyInitialData[14].tu;
				g_pEnemy[enemyCount].tv = g_EnemyInitialData[14].tv;
				g_pEnemy[enemyCount].BulletKind = g_EnemyInitialData[14].BulletKind;
				g_pEnemy[enemyCount].BulletDeg = g_EnemyInitialData[14].BulletDeg;
				break;

			case G_STOPENEMY04:
				g_pEnemy[enemyCount].enemyKind = G_STOPENEMY04;
				g_pEnemy[enemyCount].Hp = g_EnemyInitialData[15].Hp;
				g_pEnemy[enemyCount].Speed = g_EnemyInitialData[15].Speed;
				g_pEnemy[enemyCount].firingInterval = (int)(g_EnemyInitialData[15].ShotInterval * 60.0f);
				g_pEnemy[enemyCount].Height = g_EnemyInitialData[15].Height;
				g_pEnemy[enemyCount].Width = g_EnemyInitialData[15].Width;
				g_pEnemy[enemyCount].Atk = g_EnemyInitialData[15].Atk;
				g_pEnemy[enemyCount].tu = g_EnemyInitialData[15].tu;
				g_pEnemy[enemyCount].tv = g_EnemyInitialData[15].tv;
				g_pEnemy[enemyCount].BulletKind = g_EnemyInitialData[15].BulletKind;
				g_pEnemy[enemyCount].BulletDeg = g_EnemyInitialData[15].BulletDeg;
				break;

			case G_MOVEENEMY01:
				g_pEnemy[enemyCount].enemyKind = G_MOVEENEMY01;
				g_pEnemy[enemyCount].Hp = g_EnemyInitialData[16].Hp;
				g_pEnemy[enemyCount].Speed = g_EnemyInitialData[16].Speed;
				g_pEnemy[enemyCount].firingInterval = (int)(g_EnemyInitialData[16].ShotInterval * 60.0f);
				g_pEnemy[enemyCount].Height = g_EnemyInitialData[16].Height;
				g_pEnemy[enemyCount].Width = g_EnemyInitialData[16].Width;
				g_pEnemy[enemyCount].Atk = g_EnemyInitialData[16].Atk;
				g_pEnemy[enemyCount].tu = g_EnemyInitialData[16].tu;
				g_pEnemy[enemyCount].tv = g_EnemyInitialData[16].tv;
				g_pEnemy[enemyCount].BulletKind = g_EnemyInitialData[16].BulletKind;
				g_pEnemy[enemyCount].BulletDeg = g_EnemyInitialData[16].BulletDeg;
				break;

			case G_MOVEENEMY02://未定
				g_pEnemy[enemyCount].enemyKind = G_MOVEENEMY02;
				g_pEnemy[enemyCount].Hp = g_EnemyInitialData[17].Hp;
				g_pEnemy[enemyCount].Speed = g_EnemyInitialData[17].Speed;
				g_pEnemy[enemyCount].firingInterval = (int)(g_EnemyInitialData[17].ShotInterval * 60.0f);
				g_pEnemy[enemyCount].Height = g_EnemyInitialData[17].Height;
				g_pEnemy[enemyCount].Width = g_EnemyInitialData[17].Width;
				g_pEnemy[enemyCount].Atk = g_EnemyInitialData[17].Atk;
				g_pEnemy[enemyCount].tu = g_EnemyInitialData[17].tu;
				g_pEnemy[enemyCount].tv = g_EnemyInitialData[17].tv;
				g_pEnemy[enemyCount].BulletKind = g_EnemyInitialData[17].BulletKind;
				g_pEnemy[enemyCount].BulletDeg = g_EnemyInitialData[17].BulletDeg;
				break;

			case G_MOVEENEMY03:
				g_pEnemy[enemyCount].enemyKind = G_MOVEENEMY03;
				g_pEnemy[enemyCount].Hp = g_EnemyInitialData[18].Hp;
				g_pEnemy[enemyCount].Speed = g_EnemyInitialData[18].Speed;
				g_pEnemy[enemyCount].firingInterval = (int)(g_EnemyInitialData[18].ShotInterval * 60.0f);
				g_pEnemy[enemyCount].Height = g_EnemyInitialData[18].Height;
				g_pEnemy[enemyCount].Width = g_EnemyInitialData[18].Width;
				g_pEnemy[enemyCount].Atk = g_EnemyInitialData[18].Atk;
				g_pEnemy[enemyCount].tu = g_EnemyInitialData[18].tu;
				g_pEnemy[enemyCount].tv = g_EnemyInitialData[18].tv;
				g_pEnemy[enemyCount].BulletKind = g_EnemyInitialData[18].BulletKind;
				g_pEnemy[enemyCount].BulletDeg = g_EnemyInitialData[18].BulletDeg;
				break;

			case G_MOVEENEMY04://未定
				g_pEnemy[enemyCount].enemyKind = G_MOVEENEMY04;
				g_pEnemy[enemyCount].Hp = g_EnemyInitialData[19].Hp;
				g_pEnemy[enemyCount].Speed = g_EnemyInitialData[19].Speed;
				g_pEnemy[enemyCount].firingInterval = (int)(g_EnemyInitialData[19].ShotInterval * 60.0f);
				g_pEnemy[enemyCount].Height = g_EnemyInitialData[19].Height;
				g_pEnemy[enemyCount].Width = g_EnemyInitialData[19].Width;
				g_pEnemy[enemyCount].Atk = g_EnemyInitialData[19].Atk;
				g_pEnemy[enemyCount].tu = g_EnemyInitialData[19].tu;
				g_pEnemy[enemyCount].tv = g_EnemyInitialData[19].tv;
				g_pEnemy[enemyCount].BulletKind = g_EnemyInitialData[19].BulletKind;
				g_pEnemy[enemyCount].BulletDeg = g_EnemyInitialData[19].BulletDeg;
				break;

			case G_LARIATENEMY01:
				g_pEnemy[enemyCount].enemyKind = G_LARIATENEMY01;
				g_pEnemy[enemyCount].Hp = g_EnemyInitialData[20].Hp;
				g_pEnemy[enemyCount].Speed = g_EnemyInitialData[20].Speed;
				g_pEnemy[enemyCount].firingInterval = (int)(g_EnemyInitialData[20].ShotInterval * 60.0f);
				g_pEnemy[enemyCount].Height = g_EnemyInitialData[20].Height;
				g_pEnemy[enemyCount].Width = g_EnemyInitialData[20].Width;
				g_pEnemy[enemyCount].Atk = g_EnemyInitialData[20].Atk;
				g_pEnemy[enemyCount].tu = g_EnemyInitialData[20].tu;
				g_pEnemy[enemyCount].tv = g_EnemyInitialData[20].tv;
				g_pEnemy[enemyCount].BulletKind = g_EnemyInitialData[20].BulletKind;
				g_pEnemy[enemyCount].BulletDeg = g_EnemyInitialData[20].BulletDeg;
				break;

			case G_LARIATENEMY02:
				g_pEnemy[enemyCount].enemyKind = G_LARIATENEMY02;
				g_pEnemy[enemyCount].Hp = g_EnemyInitialData[21].Hp;
				g_pEnemy[enemyCount].Speed = g_EnemyInitialData[21].Speed;
				g_pEnemy[enemyCount].firingInterval = (int)(g_EnemyInitialData[21].ShotInterval * 60.0f);
				g_pEnemy[enemyCount].Height = g_EnemyInitialData[21].Height;
				g_pEnemy[enemyCount].Width = g_EnemyInitialData[21].Width;
				g_pEnemy[enemyCount].Atk = g_EnemyInitialData[21].Atk;
				g_pEnemy[enemyCount].tu = g_EnemyInitialData[21].tu;
				g_pEnemy[enemyCount].tv = g_EnemyInitialData[21].tv;
				g_pEnemy[enemyCount].BulletKind = g_EnemyInitialData[21].BulletKind;
				g_pEnemy[enemyCount].BulletDeg = g_EnemyInitialData[21].BulletDeg;
				break;

			case S_STOPENEMY01:
				g_pEnemy[enemyCount].enemyKind = S_STOPENEMY01;
				g_pEnemy[enemyCount].Hp = g_EnemyInitialData[22].Hp;
				g_pEnemy[enemyCount].Speed = g_EnemyInitialData[22].Speed;
				g_pEnemy[enemyCount].firingInterval = (int)(g_EnemyInitialData[22].ShotInterval * 60.0f);
				g_pEnemy[enemyCount].Height = g_EnemyInitialData[22].Height;
				g_pEnemy[enemyCount].Width = g_EnemyInitialData[22].Width;
				g_pEnemy[enemyCount].Atk = g_EnemyInitialData[22].Atk;
				g_pEnemy[enemyCount].tu = g_EnemyInitialData[22].tu;
				g_pEnemy[enemyCount].tv = g_EnemyInitialData[22].tv;
				g_pEnemy[enemyCount].BulletKind = g_EnemyInitialData[22].BulletKind;
				g_pEnemy[enemyCount].BulletDeg = g_EnemyInitialData[22].BulletDeg;
				break;

			case S_STOPENEMY02:
				g_pEnemy[enemyCount].enemyKind = S_STOPENEMY02;
				g_pEnemy[enemyCount].Hp = g_EnemyInitialData[23].Hp;
				g_pEnemy[enemyCount].Speed = g_EnemyInitialData[23].Speed;
				g_pEnemy[enemyCount].firingInterval = (int)(g_EnemyInitialData[23].ShotInterval * 60.0f);
				g_pEnemy[enemyCount].Height = g_EnemyInitialData[23].Height;
				g_pEnemy[enemyCount].Width = g_EnemyInitialData[23].Width;
				g_pEnemy[enemyCount].Atk = g_EnemyInitialData[23].Atk;
				g_pEnemy[enemyCount].tu = g_EnemyInitialData[23].tu;
				g_pEnemy[enemyCount].tv = g_EnemyInitialData[23].tv;
				g_pEnemy[enemyCount].BulletKind = g_EnemyInitialData[23].BulletKind;
				g_pEnemy[enemyCount].BulletDeg = g_EnemyInitialData[23].BulletDeg;
				break;

			case S_STOPENEMY03:
				g_pEnemy[enemyCount].enemyKind = S_STOPENEMY03;
				g_pEnemy[enemyCount].Hp = g_EnemyInitialData[24].Hp;
				g_pEnemy[enemyCount].Speed = g_EnemyInitialData[24].Speed;
				g_pEnemy[enemyCount].firingInterval = (int)(g_EnemyInitialData[24].ShotInterval * 60.0f);
				g_pEnemy[enemyCount].Height = g_EnemyInitialData[24].Height;
				g_pEnemy[enemyCount].Width = g_EnemyInitialData[24].Width;
				g_pEnemy[enemyCount].Atk = g_EnemyInitialData[24].Atk;
				g_pEnemy[enemyCount].tu = g_EnemyInitialData[24].tu;
				g_pEnemy[enemyCount].tv = g_EnemyInitialData[24].tv;
				g_pEnemy[enemyCount].BulletKind = g_EnemyInitialData[24].BulletKind;
				g_pEnemy[enemyCount].BulletDeg = g_EnemyInitialData[24].BulletDeg;
				break;

			case S_MOVEENEMY01:
				g_pEnemy[enemyCount].enemyKind = S_MOVEENEMY01;
				g_pEnemy[enemyCount].Hp = g_EnemyInitialData[25].Hp;
				g_pEnemy[enemyCount].Speed = g_EnemyInitialData[25].Speed;
				g_pEnemy[enemyCount].firingInterval = (int)(g_EnemyInitialData[25].ShotInterval * 60.0f);
				g_pEnemy[enemyCount].Height = g_EnemyInitialData[25].Height;
				g_pEnemy[enemyCount].Width = g_EnemyInitialData[25].Width;
				g_pEnemy[enemyCount].Atk = g_EnemyInitialData[25].Atk;
				g_pEnemy[enemyCount].tu = g_EnemyInitialData[25].tu;
				g_pEnemy[enemyCount].tv = g_EnemyInitialData[25].tv;
				g_pEnemy[enemyCount].BulletKind = g_EnemyInitialData[25].BulletKind;
				g_pEnemy[enemyCount].BulletDeg = g_EnemyInitialData[25].BulletDeg;
				break;

			case S_MOVEENEMY02:
				g_pEnemy[enemyCount].enemyKind = S_MOVEENEMY02;
				g_pEnemy[enemyCount].Hp = g_EnemyInitialData[26].Hp;
				g_pEnemy[enemyCount].Speed = g_EnemyInitialData[26].Speed;
				g_pEnemy[enemyCount].firingInterval = (int)(g_EnemyInitialData[26].ShotInterval * 60.0f);
				g_pEnemy[enemyCount].Height = g_EnemyInitialData[26].Height;
				g_pEnemy[enemyCount].Width = g_EnemyInitialData[26].Width;
				g_pEnemy[enemyCount].Atk = g_EnemyInitialData[26].Atk;
				g_pEnemy[enemyCount].tu = g_EnemyInitialData[26].tu;
				g_pEnemy[enemyCount].tv = g_EnemyInitialData[26].tv;
				g_pEnemy[enemyCount].BulletKind = g_EnemyInitialData[26].BulletKind;
				g_pEnemy[enemyCount].BulletDeg = g_EnemyInitialData[26].BulletDeg;
				break;

			case S_MOVEENEMY03:
				g_pEnemy[enemyCount].enemyKind = S_MOVEENEMY03;
				g_pEnemy[enemyCount].Hp = g_EnemyInitialData[27].Hp;
				g_pEnemy[enemyCount].Speed = g_EnemyInitialData[27].Speed;
				g_pEnemy[enemyCount].firingInterval = (int)(g_EnemyInitialData[27].ShotInterval * 60.0f);
				g_pEnemy[enemyCount].Height = g_EnemyInitialData[27].Height;
				g_pEnemy[enemyCount].Width = g_EnemyInitialData[27].Width;
				g_pEnemy[enemyCount].Atk = g_EnemyInitialData[27].Atk;
				g_pEnemy[enemyCount].tu = g_EnemyInitialData[27].tu;
				g_pEnemy[enemyCount].tv = g_EnemyInitialData[27].tv;
				g_pEnemy[enemyCount].BulletKind = g_EnemyInitialData[27].BulletKind;
				g_pEnemy[enemyCount].BulletDeg = g_EnemyInitialData[27].BulletDeg;
				break;

			case S_MOVEENEMY04:
				g_pEnemy[enemyCount].enemyKind = S_MOVEENEMY04;
				g_pEnemy[enemyCount].Hp = g_EnemyInitialData[28].Hp;
				g_pEnemy[enemyCount].Speed = g_EnemyInitialData[28].Speed;
				g_pEnemy[enemyCount].firingInterval = (int)(g_EnemyInitialData[28].ShotInterval * 60.0f);
				g_pEnemy[enemyCount].Height = g_EnemyInitialData[28].Height;
				g_pEnemy[enemyCount].Width = g_EnemyInitialData[28].Width;
				g_pEnemy[enemyCount].Atk = g_EnemyInitialData[28].Atk;
				g_pEnemy[enemyCount].tu = g_EnemyInitialData[28].tu;
				g_pEnemy[enemyCount].tv = g_EnemyInitialData[28].tv;
				g_pEnemy[enemyCount].BulletKind = g_EnemyInitialData[28].BulletKind;
				g_pEnemy[enemyCount].BulletDeg = g_EnemyInitialData[28].BulletDeg;
				break;

			case FIXEDBATTERY1:
				g_pEnemy[enemyCount].enemyKind = FIXEDBATTERY1;
				g_pEnemy[enemyCount].Hp = INTMAX;
				g_pEnemy[enemyCount].Speed = g_EnemyInitialData[29].Speed;
				g_pEnemy[enemyCount].firingInterval = (int)(g_EnemyInitialData[29].ShotInterval * 60.0f);
				g_pEnemy[enemyCount].Height = g_EnemyInitialData[29].Height;
				g_pEnemy[enemyCount].Width = g_EnemyInitialData[29].Width;
				g_pEnemy[enemyCount].Atk = g_EnemyInitialData[29].Atk;
				g_pEnemy[enemyCount].tu = g_EnemyInitialData[29].tu;
				g_pEnemy[enemyCount].tv = g_EnemyInitialData[29].tv;
				g_pEnemy[enemyCount].BulletKind = g_EnemyInitialData[29].BulletKind;
				g_pEnemy[enemyCount].BulletDeg = g_EnemyInitialData[29].BulletDeg;
				break;

			case FIXEDBATTERY2:
				g_pEnemy[enemyCount].enemyKind = FIXEDBATTERY2;
				g_pEnemy[enemyCount].Hp = INTMAX;
				g_pEnemy[enemyCount].Speed = g_EnemyInitialData[30].Speed;
				g_pEnemy[enemyCount].firingInterval = (int)(g_EnemyInitialData[30].ShotInterval * 60.0f);
				g_pEnemy[enemyCount].Height = g_EnemyInitialData[30].Height;
				g_pEnemy[enemyCount].Width = g_EnemyInitialData[30].Width;
				g_pEnemy[enemyCount].Atk = g_EnemyInitialData[30].Atk;
				g_pEnemy[enemyCount].tu = g_EnemyInitialData[30].tu;
				g_pEnemy[enemyCount].tv = g_EnemyInitialData[30].tv;
				g_pEnemy[enemyCount].BulletKind = g_EnemyInitialData[30].BulletKind;
				g_pEnemy[enemyCount].BulletDeg = g_EnemyInitialData[30].BulletDeg;
				break;

			case SWITCH_A:
				g_pEnemy[enemyCount].enemyKind = SWITCH_A;
				g_pEnemy[enemyCount].Hp = INTMAX;
				g_pEnemy[enemyCount].Speed =0.0f;
				g_pEnemy[enemyCount].firingInterval = INTMAX;
				g_pEnemy[enemyCount].Height = 70.0f;
				g_pEnemy[enemyCount].Width = 38.0f;
				g_pEnemy[enemyCount].Atk = 5.0f;
				g_pEnemy[enemyCount].tu = 128.0f;
				g_pEnemy[enemyCount].tv = 320.0f;
				g_pEnemy[enemyCount].BulletKind = BULLETNORMAL1;
				g_pEnemy[enemyCount].BulletDeg = 0.0f;
				break;

			case SWITCH_B:
				g_pEnemy[enemyCount].enemyKind = SWITCH_B;
				g_pEnemy[enemyCount].Hp = INTMAX;
				g_pEnemy[enemyCount].Speed = 0.0f;
				g_pEnemy[enemyCount].firingInterval = INTMAX;
				g_pEnemy[enemyCount].Height = 70.0f;
				g_pEnemy[enemyCount].Width = 38.0f;
				g_pEnemy[enemyCount].Atk = 5.0f;
				g_pEnemy[enemyCount].tu = 128.0f;
				g_pEnemy[enemyCount].tv = 320.0f;
				g_pEnemy[enemyCount].BulletKind = BULLETNORMAL1;
				g_pEnemy[enemyCount].BulletDeg = 0.0f;
				break;

			case SWITCH_C:
				g_pEnemy[enemyCount].enemyKind = SWITCH_C;
				g_pEnemy[enemyCount].Hp = INTMAX;
				g_pEnemy[enemyCount].Speed = 0.0f;
				g_pEnemy[enemyCount].firingInterval = INTMAX;
				g_pEnemy[enemyCount].Height = 70.0f;
				g_pEnemy[enemyCount].Width = 38.0f;
				g_pEnemy[enemyCount].Atk = 5.0f;
				g_pEnemy[enemyCount].tu = 128.0f;
				g_pEnemy[enemyCount].tv = 320.0f;
				g_pEnemy[enemyCount].BulletKind = BULLETNORMAL1;
				g_pEnemy[enemyCount].BulletDeg = 0.0f;
				break;

			
			}

			if (g_pEnemy[enemyCount].enemyKind != NOTHING) {
				g_pEnemy[enemyCount].WorldPos.x = j*TIPSIZE;
				g_pEnemy[enemyCount].WorldPos.y = i*TIPSIZE;
				g_pEnemy[enemyCount].WindowPos.x = 0;
				g_pEnemy[enemyCount].WindowPos.y = 0;
				g_pEnemy[enemyCount].MovementX = g_pEnemy[enemyCount].MovementY = 0;
				g_pEnemy[enemyCount].bulletFrameCount = 0;
				g_pEnemy[enemyCount].beDead = false;//死んでいるか
				g_pEnemy[enemyCount].beActive = false;//活動中か
				g_pEnemy[enemyCount].beLeft = false;//左（右）どうっち向いてるか
				g_pEnemy[enemyCount].EnemyBasePoint.x = g_pEnemy[enemyCount].WorldPos.x;//エネミーの初期座標
				g_pEnemy[enemyCount].EnemyBasePoint.y = g_pEnemy[enemyCount].WorldPos.y;
				g_pEnemy[enemyCount].FrontWorldPos.x = g_pEnemy[enemyCount].WorldPos.x;//1フレーム前の敵の座標
				g_pEnemy[enemyCount].FrontWorldPos.y = g_pEnemy[enemyCount].WorldPos.y;
				g_pEnemy[enemyCount].Rush = false;
				enemyCount++;

			}
		}
	}
}