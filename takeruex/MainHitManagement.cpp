#include"MainHitManagement.h"
#include"MapControl.h"
#include"MainControl.h"
#include"PlayerControl.h"
#include"PlayerRender.h"
#include"BulletControl.h"
#include"BulletRender.h"
#include"EnemyControl.h"
#include"EnemyRender.h"
#include"DirectXSound.h"
#include"BlackOutRender.h"
#include"Boss1Control.h"

#include<Windows.h>
#include<tchar.h>
#include<stdio.h>

void PushOutMap(const D3DXVECTOR2& Pos, float* MovementX, float* MovementY, float width, float height);

void CollisionMapForBullet();

#include<Windows.h>
#include<tchar.h>
#include<stdio.h>

void HitManage() {

	Player* player = GetplayerData();
	Boss1Data* pBoss1 = GetBoss1Data();
	Bullet* pFirstBullet = GetFirstBulletAddress();
	Enemy* enemy = GetenemyData();
	

	//プレイヤーとマップの処理
	static int frcntInvincible;

	D3DXVECTOR2 playerRightBottom;
	D3DXVECTOR2 playerLeftBottom;
	playerRightBottom.x = player->WorldPos.x + PLAYERSIZEWIDTH / 2;
	playerLeftBottom.x = player->WorldPos.x - PLAYERSIZEWIDTH / 2;
	playerRightBottom.y= playerLeftBottom.y= player->WorldPos.y + PLAYERSIZEHEIGHT / 2 + 1;
	if (!player->beInvincible) {
		if (MapKindSpecifyForPos(&playerRightBottom) == UPNEEDLE || MapKindSpecifyForPos(&playerLeftBottom) == UPNEEDLE) {
			player->Hp -= 10;
			player->beInvincible = true;
		}
	}
	if (MapKindSpecifyForPos(&playerRightBottom) == HOLE || MapKindSpecifyForPos(&playerLeftBottom) == HOLE) {
		player->Hp = 0;
	}

	D3DXVECTOR2 tmp = player->WorldPos;
	tmp.y += 5;
	if (player->beLeft) {
		tmp.x -= 15;
	}
	else {
		tmp.x += 15;
	}
	PushOutMap(tmp, &player->MovementX, &player->MovementY, PLAYERSIZEWIDTH - 30, PLAYERSIZEHEIGHT - 10);

	//エネミーとマップの押し出し処理
	for (int i = 0; i < ENEMYNUMBER; i++) {
		PushOutMap(enemy[i].WorldPos, &enemy[i].MovementX, &enemy[i].MovementX, ENEMYRESIZEWIDTH, ENEMYRESIZEHEIGHT);
	}

	//boss1とマップの押し出し処理
	PushOutMap(pBoss1->WolrdPos, &pBoss1->MovementX, &pBoss1->MovementY, BOSS1WIDTH, BOSS1HEIGHT);
	
	//弾のマップとの反射処理
	CollisionMapForBullet();

	for (Bullet* pSearchBullet = pFirstBullet->next; pSearchBullet != NULL; pSearchBullet = pSearchBullet->next) {

		//ボス１と弾のダメージ計算
		if (pBoss1->isActive&&!(pBoss1->isDead)) {
			if (pSearchBullet->wasReflect) {
				if (SquareHit(&pSearchBullet->WindowPos, pSearchBullet->Size, pSearchBullet->Size, &pBoss1->WindowPos, BOSS1WIDTH, BOSS1HEIGHT)) {
					pBoss1->Hp -= pSearchBullet->Atk;
					DeleteBullet(&pSearchBullet);
					continue;
				}
			}
		}

		//プレイヤーと弾のダメージ計算と無敵時間の考慮
		D3DXVECTOR2 tmpPlayer = player->WindowPos;
		if (player->beLeft) {
			tmpPlayer.x += 15;
		}
		else {
			tmpPlayer.x += -15;
		}

		if (SquareHit(&tmpPlayer, PLAYERSIZEWIDTH - 30, PLAYERSIZEHEIGHT, &pSearchBullet->WindowPos, pSearchBullet->Size, pSearchBullet->Size)) {

			if (!player->beInvincible) {
				PlayBackSound(SOUND01, false, 100);

				player->Hp -= pSearchBullet->Atk;

				player->beInvincible = true;
			}

			DeleteBullet(&pSearchBullet);
			continue;
		}

		//敵とバレットのダメージ計算
		for (int j = 0; j < ENEMYNUMBER; j++) {
			if (enemy[j].beActive == true && enemy[j].beDead == false) {
				if (pSearchBullet->wasReflect) {
					if (SquareHit(&pSearchBullet->WindowPos, pFirstBullet->Size, pFirstBullet->Size, &enemy[j].WindowPos, ENEMYRESIZEWIDTH, ENEMYRESIZEHEIGHT)) {

						enemy[j].Hp -= pSearchBullet->Atk;

						if (enemy[j].Hp < 0) {
							enemy[j].beActive = false;
							enemy[j].beDead = true;
						}
						DeleteBullet(&pSearchBullet);
						break;
					}
				}
			}
		}
	}

	//エネミーとプレイヤーの直接のあたり判定
	for (int i = 0; i < ENEMYNUMBER; i++) {
		if (enemy[i].beActive && !enemy[i].beDead) {
			D3DXVECTOR2 tmpPlayer = player->WindowPos;
			if (player->beLeft) {
				tmpPlayer.x += 15;
			}
			else {
				tmpPlayer.x += -15;
			}
			if (SquareHit(&player->WindowPos, PLAYERSIZEWIDTH - 30, PLAYERSIZEHEIGHT, &enemy[i].WindowPos, ENEMYRESIZEWIDTH, ENEMYRESIZEHEIGHT)) {

				if (!player->beInvincible) {
					player->Hp -= enemy[i].Atk;
					player->beInvincible = true;
				}
			}
		}
	}

	//boss1とプレイヤーの直接のあたり判定
	if (pBoss1->isActive && !(pBoss1->isDead)) {
		D3DXVECTOR2 tmpPlayer = player->WindowPos;
		if (player->beLeft) {
			tmpPlayer.x += 15;
		}
		else {
			tmpPlayer.x += -15;
		}
		if (SquareHit(&player->WindowPos, PLAYERSIZEWIDTH - 30, PLAYERSIZEHEIGHT, &pBoss1->WindowPos, BOSS1WIDTH, BOSS1HEIGHT)) {

			if (!player->beInvincible) {
				player->Hp -= pBoss1->Atk;
				player->beInvincible = true;
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

void PushOutMap(const D3DXVECTOR2& Pos,float* MovementX,float* MovementY, float width, float height) {

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

			if (MapKindSpecify(&tmpNum) != NOTHING) {
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

			if (MapKindSpecify(&tmpNum) != NOTHING) {
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

void CollisionMapForBullet() {
	static int fr = 0;
	fr++;
	Bullet* pFirstBullet = GetFirstBulletAddress();
	for (Bullet* pSearchBullet = pFirstBullet->next; pSearchBullet != NULL; pSearchBullet = pSearchBullet->next) {
		D3DXVECTOR2 Left;
		D3DXVECTOR2 Right;
		D3DXVECTOR2 Bottom;
		D3DXVECTOR2 Top;

		Left.x = pSearchBullet->WorldPos.x - (pSearchBullet->Size / 2) + (pSearchBullet->MovementX);
		Right.x = pSearchBullet->WorldPos.x + (pSearchBullet->Size / 2) + (pSearchBullet->MovementX);
		Top.x = Bottom.x = pSearchBullet->WorldPos.x + (pSearchBullet->MovementX);
		Top.y = pSearchBullet->WorldPos.y - (pSearchBullet->Size / 2) + (pSearchBullet->MovementY);
		Bottom.y = pSearchBullet->WorldPos.y + (pSearchBullet->Size / 2) + (pSearchBullet->MovementY);
		Left.y = Right.y = pSearchBullet->WorldPos.y + (pSearchBullet->MovementY);

		D3DXVECTOR2 tmpPos;
		tmpPos.x = pSearchBullet->WorldPos.x + pSearchBullet->MovementX;
		tmpPos.y = pSearchBullet->WorldPos.y + pSearchBullet->MovementY;

		if (MapKindSpecifyForPos(&Left) != NOTHING) {

			pSearchBullet->ReflectCnt++;
			if (pSearchBullet->ReflectCnt == pSearchBullet->ReflectMax) {
				pSearchBullet->beActive = false;
				continue;
			}

			if (pSearchBullet->MovementY < 0) {
				pSearchBullet->Rad = 2 * D3DX_PI - Calculate_rad(tmpPos.x, tmpPos.y, pSearchBullet->WorldPos.x, pSearchBullet->WorldPos.y);
			}
			else if (pSearchBullet->MovementY > 0) {
				pSearchBullet->Rad = -1 * Calculate_rad(tmpPos.x, tmpPos.y, pSearchBullet->WorldPos.x, pSearchBullet->WorldPos.y);
			}
			else if (pSearchBullet->MovementY = 0) {
				pSearchBullet->Rad += D3DX_PI;
			}

		}

		else if (MapKindSpecifyForPos(&Right) != NOTHING) {
			pSearchBullet->ReflectCnt++;
			if (pSearchBullet->ReflectCnt == pSearchBullet->ReflectMax) {
				pSearchBullet->beActive = false;
				continue;
			}

			if (pSearchBullet->MovementY < 0) {
				pSearchBullet->Rad = D3DX_PI - (Calculate_rad(tmpPos.x, tmpPos.y, pSearchBullet->WorldPos.x, pSearchBullet->WorldPos.y) - D3DX_PI);
			}
			else if (pSearchBullet->MovementY > 0) {
				pSearchBullet->Rad = D3DX_PI + (D3DX_PI - Calculate_rad(tmpPos.x, tmpPos.y, pSearchBullet->WorldPos.x, pSearchBullet->WorldPos.y));
			}
			else if (pSearchBullet->MovementY = 0) {
				pSearchBullet->Rad += D3DX_PI;
			}

		}
		else if (MapKindSpecifyForPos(&Bottom) != NOTHING) {
			pSearchBullet->ReflectCnt++;
			if (pSearchBullet->ReflectCnt == pSearchBullet->ReflectMax) {
				pSearchBullet->beActive = false;
				continue;
			}

			if (pSearchBullet->MovementX < 0) {
				pSearchBullet->Rad = (0.5*D3DX_PI) + ((0.5*D3DX_PI) - Calculate_rad(tmpPos.x, tmpPos.y, pSearchBullet->WorldPos.x, pSearchBullet->WorldPos.y));
			}
			else if (pSearchBullet->MovementX > 0) {
				pSearchBullet->Rad = (0.5*D3DX_PI) - (Calculate_rad(tmpPos.x, tmpPos.y, pSearchBullet->WorldPos.x, pSearchBullet->WorldPos.y) - (0.5*D3DX_PI));
			}
			else if (pSearchBullet->MovementX = 0) {
				pSearchBullet->Rad += D3DX_PI;
			}

		}
		else if (MapKindSpecifyForPos(&Top) != NOTHING) {
			pSearchBullet->ReflectCnt++;
			if (pSearchBullet->ReflectCnt == pSearchBullet->ReflectMax) {
				pSearchBullet->beActive = false;
				continue;
			}

			if (pSearchBullet->MovementX < 0) {
				pSearchBullet->Rad = (1.5*D3DX_PI) - (Calculate_rad(tmpPos.x, tmpPos.y, pSearchBullet->WorldPos.x, pSearchBullet->WorldPos.y) - (1.5*D3DX_PI));
			}
			else if (pSearchBullet->MovementX > 0) {
				pSearchBullet->Rad = (1.5*D3DX_PI) + ((1.5*D3DX_PI) - Calculate_rad(tmpPos.x, tmpPos.y, pSearchBullet->WorldPos.x, pSearchBullet->WorldPos.y));
			}
			else if (pSearchBullet->MovementX = 0) {
				pSearchBullet->Rad += D3DX_PI;
			}

			SetBulletMovement(pSearchBullet);
		}
	}
		//Bullet* bullet = GetBullet();

		//for (int i = 0; i < BULLETNUMBER; i++) {

		//	if (bullet[i].beActive) {
		//		//for (float unitVectorCnt = 1 / bullet[i].Speed; unitVectorCnt <= 1; unitVectorCnt += 1 / bullet[i].Speed) 
		//		D3DXVECTOR2 Left;
		//		D3DXVECTOR2 Right;
		//		D3DXVECTOR2 Bottom;
		//		D3DXVECTOR2 Top;

		//		Left.x = bullet[i].WorldPos.x - (bullet[i].Size / 2) + (bullet[i].MovementX/*unitVectorCnt*/);
		//		Right.x = bullet[i].WorldPos.x + (bullet[i].Size / 2) + (bullet[i].MovementX/**unitVectorCnt*/);
		//		Top.x = Bottom.x = bullet[i].WorldPos.x + (bullet[i].MovementX/**unitVectorCnt*/);
		//		Top.y = bullet[i].WorldPos.y - (bullet[i].Size / 2) + (bullet[i].MovementY/**unitVectorCnt*/);
		//		Bottom.y = bullet[i].WorldPos.y + (bullet[i].Size / 2) + (bullet[i].MovementY/**unitVectorCnt*/);
		//		Left.y = Right.y = bullet[i].WorldPos.y + (bullet[i].MovementY/**unitVectorCnt*/);

		//		D3DXVECTOR2 tmpPos;
		//		tmpPos.x = bullet[i].WorldPos.x + bullet[i].MovementX;
		//		tmpPos.y = bullet[i].WorldPos.y + bullet[i].MovementY;

		//		if (MapKindSpecifyForPos(&Left) != NOTHING) {

		//			bullet[i].ReflectCnt++;
		//			if (bullet[i].ReflectCnt == bullet[i].ReflectMax) {
		//				bullet[i].beActive = false;
		//				continue;
		//			}

		//			if (bullet[i].MovementY < 0) {
		//				bullet[i].Rad = 2 * D3DX_PI - Calculate_rad(tmpPos.x, tmpPos.y, bullet[i].WorldPos.x, bullet[i].WorldPos.y);
		//			}
		//			else if (bullet[i].MovementY > 0) {
		//				bullet[i].Rad = -1 * Calculate_rad(tmpPos.x, tmpPos.y, bullet[i].WorldPos.x, bullet[i].WorldPos.y);
		//			}
		//			else if (bullet[i].MovementY = 0) {
		//				bullet[i].Rad += D3DX_PI;
		//			}

		//		}

		//		else if (MapKindSpecifyForPos(&Right) != NOTHING) {
		//			bullet[i].ReflectCnt++;
		//			if (bullet[i].ReflectCnt == bullet[i].ReflectMax) {
		//				bullet[i].beActive = false;
		//				continue;
		//			}

		//			if (bullet[i].MovementY < 0) {
		//				bullet[i].Rad = D3DX_PI - (Calculate_rad(tmpPos.x, tmpPos.y, bullet[i].WorldPos.x, bullet[i].WorldPos.y) - D3DX_PI);
		//			}
		//			else if (bullet[i].MovementY > 0) {
		//				bullet[i].Rad = D3DX_PI + (D3DX_PI - Calculate_rad(tmpPos.x, tmpPos.y, bullet[i].WorldPos.x, bullet[i].WorldPos.y));
		//			}
		//			else if (bullet[i].MovementY = 0) {
		//				bullet[i].Rad += D3DX_PI;
		//			}

		//		}
		//		else if (MapKindSpecifyForPos(&Bottom) != NOTHING) {
		//			bullet[i].ReflectCnt++;
		//			if (bullet[i].ReflectCnt == bullet[i].ReflectMax) {
		//				bullet[i].beActive = false;
		//				continue;
		//			}

		//			if (bullet[i].MovementX < 0) {
		//				bullet[i].Rad = (0.5*D3DX_PI) + ((0.5*D3DX_PI) - Calculate_rad(tmpPos.x, tmpPos.y, bullet[i].WorldPos.x, bullet[i].WorldPos.y));
		//			}
		//			else if (bullet[i].MovementX > 0) {
		//				bullet[i].Rad = (0.5*D3DX_PI) - (Calculate_rad(tmpPos.x, tmpPos.y, bullet[i].WorldPos.x, bullet[i].WorldPos.y) - (0.5*D3DX_PI));
		//			}
		//			else if (bullet[i].MovementX = 0) {
		//				bullet[i].Rad += D3DX_PI;
		//			}

		//		}
		//		else if (MapKindSpecifyForPos(&Top) != NOTHING) {
		//			bullet[i].ReflectCnt++;
		//			if (bullet[i].ReflectCnt == bullet[i].ReflectMax) {
		//				bullet[i].beActive = false;
		//				continue;
		//			}

		//			if (bullet[i].MovementX < 0) {
		//				bullet[i].Rad = (1.5*D3DX_PI) - (Calculate_rad(tmpPos.x, tmpPos.y, bullet[i].WorldPos.x, bullet[i].WorldPos.y) - (1.5*D3DX_PI));
		//			}
		//			else if (bullet[i].MovementX > 0) {
		//				bullet[i].Rad = (1.5*D3DX_PI) + ((1.5*D3DX_PI) - Calculate_rad(tmpPos.x, tmpPos.y, bullet[i].WorldPos.x, bullet[i].WorldPos.y));
		//			}
		//			else if (bullet[i].MovementX = 0) {
		//				bullet[i].Rad += D3DX_PI;
		//			}

		//			SetBulletMovement(i);
		//		}
		//	}
		//}
}