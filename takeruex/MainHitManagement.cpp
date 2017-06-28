#include"MainHitManagement.h"
#include"PlayerControl.h"
#include"PlayerRender.h"
#include"BulletControl.h"
#include"BulletRender.h"
#include"EnemyControl.h"
#include"EnemyRender.h"

void HitManage() {

	Player* player = GetplayerData();
	Bullet* bullet = GetBullet();
	Enemy* enemy = GetenemyData();


	static int frcnt;

	for (int i = 0; i < BULLETNUMBER; i++) {
		for (int j = 0; j < ENEMYNUMBER; j++) {

			//敵とバレットのダメージ計算
			if (enemy[j].beActive == true && enemy[j].beDead == false) {
				if (bullet[i].wasReflect) {
					if (SquareHit(&bullet[i].WindowPos, bullet->Size, bullet->Size, &enemy[j].WindowPos, ENEMYRESIZEWIDHE, ENEMYRESIZEHEIGHT)) {

						bullet[i].beActive = false;
						enemy[j].Hp -= bullet[i].Atk;

						if (enemy[j].Hp < 0) {
							enemy[j].beActive = false;
							enemy[j].beDead = true;
						}
					}
				}
			}

			//プレイヤーのダメージ計算と無敵時間の考慮
			if (bullet[i].beActive) {
				if (SquareHit(&player->WindowPos, PLAYERSIZEWIDHE, PLAYERSIZEHEIGHT, &bullet[i].WindowPos, bullet->Size, bullet->Size)) {

					bullet[i].beActive = false;

					if (!player->beInvincible) {
						player->Hp -= bullet->Atk;

						player->beInvincible = true;
					}
				}
			}
		}

		//エネミーとプレイヤーの直接のあたり判定
		for (int i = 0; i < ENEMYNUMBER; i++) {
			if(enemy[i].beActive && !enemy[i].beDead){
				if (SquareHit(&player->WindowPos, PLAYERSIZEWIDHE, PLAYERSIZEHEIGHT, &enemy[i].WindowPos, ENEMYRESIZEWIDHE, ENEMYRESIZEHEIGHT)) {
					if (!player->beInvincible) {
						player->Hp -= enemy[i].Atk;
						player->beInvincible = true;
					}
				}
			}
		}
	}
	if (player->beInvincible) {
		frcnt++;
	}

	if (frcnt >= 90) {
		frcnt = 0;
		player->beInvincible = false;
	}

	//playerの死亡判定のフラグ
	if (player->Hp <= 0) {
		player->beActive = false;
	}
}

bool CircleHit(float cx1, float cy1, float r1, float cx2, float cy2, float r2) {

	float sumR = r1 + r2;//2つの円の半径の合計求める
						 // 三平方の定理で中心点の長さを求めている
	float L = pow(cx1 - cx2, 2) + pow(cy1 - cy2, 2);
	L = sqrt(L);

	if (sumR >= L) {//中心点の長さが半径の合計より短かったら真
		return TRUE;
	}
	else if (sumR < L) {//中心点の長さが半径の合計より長かったら偽
		return FALSE;
	}
}

bool SquareHit(D3DXVECTOR2* pPosA, float widthA, float heightA, D3DXVECTOR2* pPosB, float widthB, float heightB)
{
	D3DXVECTOR2 LeftTopA;
	D3DXVECTOR2 RightTopA;
	D3DXVECTOR2 RightBottomA;
	D3DXVECTOR2 LeftBottomA;
	D3DXVECTOR2 LeftTopB;
	D3DXVECTOR2 RightTopB;
	D3DXVECTOR2 RightBottomB;
	D3DXVECTOR2 LeftBottomB;

	LeftTopA.x = LeftBottomA.x = pPosA->x - widthA / 2;
	RightTopA.x = RightBottomA.x = pPosA->x + widthA / 2;
	LeftTopA.y = RightTopA.y = pPosA->y - heightA / 2;
	RightBottomA.y = LeftBottomA.y = pPosA->y + heightA / 2;
	LeftTopB.x = LeftBottomB.x = pPosB->x - widthB / 2;
	RightTopB.x = RightBottomB.x = pPosB->x + widthB / 2;
	LeftTopB.y = RightTopB.y = pPosB->y - heightB / 2;
	RightBottomB.y = LeftBottomB.y = pPosB->y + heightB / 2;

	if (LeftTopA.x < RightTopB.x) {
		if (LeftTopB.x < RightTopA.x) {
			if (LeftTopA.y < LeftBottomB.y) {
				if (LeftTopB.y < LeftBottomA.y) {
					return true;
				}
			}
		}
	}
	return false;
}