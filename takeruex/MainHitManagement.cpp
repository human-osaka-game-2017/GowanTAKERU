#include"MainHitManagement.h"
#include"MapControl.h"
#include"PlayerControl.h"
#include"PlayerRender.h"
#include"BulletControl.h"
#include"BulletRender.h"
#include"EnemyControl.h"
#include"EnemyRender.h"
#include"DirectXSound.h"

#include<Windows.h>
#include<tchar.h>
#include<stdio.h>

void CollisionMap(const D3DXVECTOR2& Pos, float* MovementX, float* MovementY, float width, float height);

void HitManage() {

	Player* player = GetplayerData();
	Bullet* bullet = GetBullet();
	Enemy* enemy = GetenemyData();

	//プレイヤーとマップのあたり判定
	D3DXVECTOR2 tmp = player->WorldPos;
	tmp.y += 5;
	CollisionMap(tmp, &player->MovementX, &player->MovementY, PLAYERSIZEWIDHE, PLAYERSIZEHEIGHT - 10);

	//エネミーとマップのあたり判定
	for (int i = 0; i < ENEMYNUMBER; i++) {
		CollisionMap(enemy[i].WorldPos, &enemy[i].MovementX, &enemy[i].MovementX, ENEMYRESIZEWIDTH, ENEMYRESIZEHEIGHT);
	}
	
	static int frcntInvincible;

	for (int i = 0; i < BULLETNUMBER; i++) {
		for (int j = 0; j < ENEMYNUMBER; j++) {

			//敵とバレットのダメージ計算
			if(bullet[i].beActive){
				if (enemy[j].beActive == true && enemy[j].beDead == false) {
					if (bullet[i].wasReflect) {
						if (SquareHit(&bullet[i].WindowPos, bullet->Size, bullet->Size, &enemy[j].WindowPos, ENEMYRESIZEWIDTH, ENEMYRESIZEHEIGHT)) {

							DeactivateBullet(i);
							enemy[j].Hp -= bullet[i].Atk;

							if (enemy[j].Hp < 0) {
								enemy[j].beActive = false;
								enemy[j].beDead = true;
							}
						}
					}
				}
			}

			//プレイヤーのダメージ計算と無敵時間の考慮
			if (bullet[i].beActive) {
				D3DXVECTOR2 tmpPlayer = player->WindowPos;
				if (player->beLeft) {
					tmpPlayer.x += 15;
				}
				else {
					tmpPlayer.x += -15;
				}
				
				if (SquareHit(&tmpPlayer, PLAYERSIZEWIDHE - 30, PLAYERSIZEHEIGHT, &bullet[i].WindowPos, bullet->Size, bullet->Size)) {

					DeactivateBullet(i);

					if (!player->beInvincible) {
						//StopSound(SOUND02);
						PlayBackSound(SOUND01, false);
						player->Hp -= bullet->Atk;

						player->beInvincible = true;
					}
				}
			}
		}

		//エネミーとプレイヤーの直接のあたり判定
		for (int i = 0; i < ENEMYNUMBER; i++) {
			if(enemy[i].beActive && !enemy[i].beDead){
				D3DXVECTOR2 tmpPlayer = player->WindowPos;
				if (player->beLeft) {
					tmpPlayer.x += 15;
				}
				else {
					tmpPlayer.x += -15;
				}
				if (SquareHit(&player->WindowPos, PLAYERSIZEWIDHE - 30, PLAYERSIZEHEIGHT, &enemy[i].WindowPos, ENEMYRESIZEWIDTH, ENEMYRESIZEHEIGHT)) {
				
					if (!player->beInvincible) {
						player->Hp -= enemy[i].Atk;
						player->beInvincible = true;
					}
				}
			}
		}
	}
	if (player->beInvincible) {
		frcntInvincible++;
	}

	if (frcntInvincible >= 90) {
		frcntInvincible = 0;
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

void CollisionMap(const D3DXVECTOR2& Pos,float* MovementX,float* MovementY, float width, float height) {

	D3DXVECTOR2 tmpPos;
	MapNumXY tmpNum;
	float tmpMovementX = *MovementX;
	float tmpMovementY = *MovementY;

	int MaxNumX = CalculateNumInRange(width);
	int MaxNumY = CalculateNumInRange(height);

	for (int i = 0; i <MaxNumY + 1; i++) {
		for (int j = 0; j <MaxNumX + 1; j++) {

			if (i == MaxNumY && j != MaxNumX) {
				tmpPos.x = (Pos.x - (width / 2)) + *MovementX + j * TIPSIZE;
				tmpPos.y = (Pos.y + (height / 2));
			}
			else if (i != MaxNumY && j == MaxNumX) {
				tmpPos.x = (Pos.x + (width / 2)) + *MovementX;
				tmpPos.y = (Pos.y - (height / 2)) + i * TIPSIZE;
			}
			else if (i == MaxNumY && j == MaxNumX) {
				tmpPos.x = (Pos.x + (width / 2)) + *MovementX;
				tmpPos.y = (Pos.y + (height / 2));
			}
			else {
				tmpPos.x = (Pos.x - (width / 2)) + *MovementX + j * TIPSIZE;
				tmpPos.y = (Pos.y - (height / 2)) + i * TIPSIZE;
			}
			MapchipNumberSpecify(&tmpNum, &tmpPos);

			if (MapKindSpecify(&tmpNum) == FLOOR) {
				if (*MovementX < 0) {
					*MovementX += ((tmpNum.NumX + 1)* TIPSIZE) - tmpPos.x;
				}
				else if (*MovementX > 0) {
					*MovementX += ((tmpNum.NumX)* TIPSIZE) - tmpPos.x - 1;
				}

				if (*MovementX != 0) {
					*MovementY *=*MovementX / tmpMovementX;
				}
			}

			if (i == MaxNumY && j != MaxNumX) {
				tmpPos.x = (Pos.x - (width / 2)) + j * TIPSIZE;
				tmpPos.y = (Pos.y + (height / 2)) + *MovementY;
			}
			else if (i != MaxNumY && j == MaxNumX) {
				tmpPos.x = (Pos.x + (width / 2));
				tmpPos.y = (Pos.y - (height / 2)) + *MovementY + i * TIPSIZE;
			}
			else if (i == MaxNumY && j == MaxNumX) {
				tmpPos.x = (Pos.x + (width / 2));
				tmpPos.y = (Pos.y + (height / 2)) + *MovementY;
			}
			else {
				tmpPos.x = (Pos.x - (width / 2)) + j * TIPSIZE;
				tmpPos.y = (Pos.y - (height / 2)) + *MovementY + i * TIPSIZE;
			}

			MapchipNumberSpecify(&tmpNum, &tmpPos);

			if (MapKindSpecify(&tmpNum) == FLOOR) {
				if (*MovementY < 0) {
					*MovementY += ((tmpNum.NumY + 1)* TIPSIZE) - tmpPos.y;
				}
				else if (*MovementY > 0) {
					*MovementY += ((tmpNum.NumY)* TIPSIZE) - tmpPos.y - 1;
				}

				if (*MovementY != 0) {
					*MovementX *= *MovementY / tmpMovementY;
				}

			}
		}
	}
}