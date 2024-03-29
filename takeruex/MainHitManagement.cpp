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
#include"CommonControl.h"
#include"Boss2Control.h"
#include"Boss3Control.h"
#include"Boss4Control.h"

#include<Windows.h>
#include<tchar.h>
#include<stdio.h>

void PushOutMapFourPoint(const D3DXVECTOR2& Pos, float* MovementX, float* MovementY, float width, float height);
void PushOutMap(const D3DXVECTOR2& Pos, float* MovementX, float* MovementY, float width, float height);

void CollisionMapForBullet();

void HitManage() {

	Player* player = GetplayerData();
	Boss1Data* pBoss1 = GetBoss1Data();
	Boss2Data* pBoss2 = GetBoss2Data();
	Boss3Data* pBoss3 = GetBoss3Data();
	Boss4Data* pBoss4 = GetBoss4Data();
	Bullet* pFirstBullet = GetFirstBulletAddress();
	EditableBulletData* BulletInitializeData = GetEditableBulletData();
	Enemy* enemy = GetEnemyData();

	//プレイヤーとマップの処理
	static int frcntInvincible;

	D3DXVECTOR2 playerLeftTop;
	D3DXVECTOR2 playerRightTop;
	D3DXVECTOR2 playerRightBottom;
	D3DXVECTOR2 playerLeftBottom;
	D3DXVECTOR2 playerCenterTop;
	D3DXVECTOR2 playerCenterBottom;
	D3DXVECTOR2 playerCenterRight;
	D3DXVECTOR2 playerCenterLeft;

	if (!player->beInvincible) {

		//上向き針の判定
		playerRightBottom.x = player->WorldPos.x + PLAYERSIZEWIDTH / 2.0f;
		playerLeftBottom.x = player->WorldPos.x - PLAYERSIZEWIDTH / 2.0f;
		playerCenterBottom.x = player->WorldPos.x;
		playerRightBottom.y = playerLeftBottom.y = playerCenterBottom.y = player->WorldPos.y + PLAYERSIZEHEIGHT / 2.0f + 1.0f;
		if (MapKindSpecifyForPos(&playerRightBottom) == UPNEEDLE
			|| MapKindSpecifyForPos(&playerLeftBottom) == UPNEEDLE
			|| MapKindSpecifyForPos(&playerCenterBottom) == UPNEEDLE) {
			player->Hp -= 10;
			player->beInvincible = true;
		}

		//落とし穴の判定
		if (MapKindSpecifyForPos(&playerRightBottom) == HOLE
			|| MapKindSpecifyForPos(&playerLeftBottom) == HOLE
			|| MapKindSpecifyForPos(&playerCenterBottom) == HOLE) {
			player->Hp = 0;
		}

		//左向き針の判定
		playerRightTop.y = player->WorldPos.y - PLAYERSIZEHEIGHT / 2.0f;
		playerRightBottom.y = player->WorldPos.y + PLAYERSIZEHEIGHT / 2.0f;
		playerCenterRight.y = player->WorldPos.y;
		playerRightTop.x = playerRightBottom.x = playerCenterRight.x = player->WorldPos.x + PLAYERSIZEWIDTH / 2.0f + 1.0f;
		if (MapKindSpecifyForPos(&playerRightBottom) == LEFTNEEDLE
			|| MapKindSpecifyForPos(&playerRightTop) == LEFTNEEDLE
			|| MapKindSpecifyForPos(&playerCenterRight) == LEFTNEEDLE) {
			player->Hp -= 10;
			player->beInvincible = true;
		}

		//右向き針の判定
		playerLeftTop.y = player->WorldPos.y - PLAYERSIZEHEIGHT / 2.0f;
		playerLeftBottom.y = player->WorldPos.y + PLAYERSIZEHEIGHT / 2.0f;
		playerCenterLeft.y = player->WorldPos.y;
		playerLeftTop.x = playerLeftBottom.x = playerCenterLeft.x = player->WorldPos.x - PLAYERSIZEWIDTH / 2.0f - 1.0f;
		if (MapKindSpecifyForPos(&playerLeftTop) == RIGHTNEEDLE
			|| MapKindSpecifyForPos(&playerLeftBottom) == RIGHTNEEDLE
			|| MapKindSpecifyForPos(&playerCenterLeft) == RIGHTNEEDLE) {
			player->Hp -= 10;
			player->beInvincible = true;
		}
	}

	//playerとマップの押し出し処理
	PushOutMap(player->WorldPos, &player->MovementX, &player->MovementY, PLAYERSIZEWIDTH, PLAYERSIZEHEIGHT);

	//エネミーとマップの押し出し処理
	int enemyMax = GetEnemyMax();
	for (int i = 0; i < enemyMax; i++) {
		if (enemy[i].beActive && !enemy[i].beDead) {
			PushOutMapFourPoint(enemy[i].WorldPos, &enemy[i].MovementX, &enemy[i].MovementY, enemy[i].Width, enemy[i].Height);
		}
	}

	//デバッグ用
	KEYSTATE* Key = GetKey();
	KeyCheck(&Key[KEY_P], DIK_P);
	KeyCheck(&Key[KEY_I], DIK_I);
	if (Key[KEY_I] == KEY_PUSH) {
		pBoss4->isActive = true;
		pBoss4->isDead = true;
	}

	if (Key[KEY_P] == KEY_PUSH) {
		for (int i = 0; i < enemyMax; i++) {
			enemy[i].beDead = true;
		}
	}

	//boss1とマップの押し出し処理
	PushOutMapFourPoint(pBoss1->WolrdPos, &pBoss1->MovementX, &pBoss1->MovementY, BOSS1WIDTH, BOSS1HEIGHT);
	//boss2とマップの押し出し処理
	PushOutMapFourPoint(pBoss2->WorldPos, &pBoss2->MovementX, &pBoss2->MovementY, BOSS2WIDTH, BOSS2HEIGHT);
	//boss3とマップの押し出し処理
	PushOutMapFourPoint(pBoss3->WolrdPos, &pBoss3->MovementX, &pBoss3->MovementY, BOSS3OBJWIDTH, BOSS3OBJHEIGHT);
	//boss4とマップの押し出し処理
	PushOutMapFourPoint(pBoss4->WolrdPos, &pBoss4->MovementX, &pBoss4->MovementY, BOSS4WIDTH, BOSS4HEIGHT);

	//弾のマップとの反射処理
	CollisionMapForBullet();

	for (Bullet* pSearchBullet = pFirstBullet->next; pSearchBullet != NULL; pSearchBullet = pSearchBullet->next) {

		int bulletKind = pFirstBullet->BulletKind;

		//ボス１と弾のダメージ計算
		if (pBoss1->isActive&&!(pBoss1->isDead)) {
			if (pSearchBullet->wasReflect) {
				if (SquareHit(&pSearchBullet->WindowPos, BulletInitializeData[bulletKind].Size, BulletInitializeData[bulletKind].Size, &pBoss1->WindowPos, BOSS1WIDTH, BOSS1HEIGHT)) {
					pBoss1->Hp -= pSearchBullet->Atk;
					if (pBoss1->Hp <= 0) {
						pBoss1->isDead = true;
						pBoss1->isActive = false;
						PlayBackSound(MAINSCENE_SE_BOSSDEAD, false, 100);
					}
					else {
						PlayBackSound(MAINSCENE_SE_ENEMYDAMAGE, false, 100);
					}
					DeleteBullet(&pSearchBullet);
					continue;
				}
			}
		}
		//ボス２と球のダメージ計算
		if (pBoss2->isActive && !(pBoss2->isDead)) {
			if (pSearchBullet->wasReflect) {
				D3DXVECTOR2 Boss2Pos = pBoss2->WorldPos;
				Boss2Pos.y -= 27.0f;
				if (SquareHit(&pSearchBullet->WorldPos, BulletInitializeData[bulletKind].Size, BulletInitializeData[bulletKind].Size, &Boss2Pos, BOSS2WIDTH - 76.0f, BOSS2HEIGHT - 54.0f)) {
					pBoss2->Hp -= pSearchBullet->Atk;
					pBoss2->hasDamage = true;
					if (pBoss2->Hp <= 0) {
						pBoss2->isDead = true;
						pBoss2->isActive = false;
						PlayBackSound(MAINSCENE_SE_BOSSDEAD, false, 100);
					}
					else {
						PlayBackSound(MAINSCENE_SE_ENEMYDAMAGE, false, 100);
					}
					DeleteBullet(&pSearchBullet);
					continue;
				}
			}
		}


		//ボス3と弾のダメージ計算とシールドの判定
		if (pBoss3->isActive && !(pBoss3->isDead)) {
			if (pSearchBullet->wasReflect) {
				D3DXVECTOR2 boss3, shield1, shield2, shield3;
				if (pBoss3->isLeft) {
					boss3.x = pBoss3->WolrdPos.x + 5;
					boss3.y = pBoss3->WolrdPos.y + 12;
					shield1.x = pBoss3->WolrdPos.x - 61;
					shield1.y = pBoss3->WolrdPos.y - 23;
					shield2.x = pBoss3->WolrdPos.x - 84;
					shield2.y = pBoss3->WolrdPos.y - 23;
					shield3.x = pBoss3->WolrdPos.x - 108;
					shield3.y = pBoss3->WolrdPos.y - 23;
				}
				else {
					boss3.x = pBoss3->WolrdPos.x - 5;
					boss3.y = pBoss3->WolrdPos.y + 12;
					shield1.x = pBoss3->WolrdPos.x + 61;
					shield1.y = pBoss3->WolrdPos.y - 23;
					shield2.x = pBoss3->WolrdPos.x + 84;
					shield2.y = pBoss3->WolrdPos.y - 23;
					shield3.x = pBoss3->WolrdPos.x + 108;
					shield3.y = pBoss3->WolrdPos.y - 23;
				}
				if (SquareHit(&pSearchBullet->WorldPos, BulletInitializeData[bulletKind].Size, BulletInitializeData[bulletKind].Size, &shield1, BOSS3SHIELD1WIDTH, BOSS3SHIELD1HEIGHT)||
					SquareHit(&pSearchBullet->WorldPos, BulletInitializeData[bulletKind].Size, BulletInitializeData[bulletKind].Size, &shield2, BOSS3SHIELD2WIDTH, BOSS3SHIELD2HEIGHT)||
					SquareHit(&pSearchBullet->WorldPos, BulletInitializeData[bulletKind].Size, BulletInitializeData[bulletKind].Size, &shield3, BOSS3SHIELD3WIDTH, BOSS3SHIELD3HEIGHT)
					) {
					PlayBackSound(MAINSCENE_SE_BARRIER, false, 100);
					DeleteBullet(&pSearchBullet);
					continue;
				}
				if (SquareHit(&pSearchBullet->WorldPos, BulletInitializeData[bulletKind].Size, BulletInitializeData[bulletKind].Size, &boss3, BOSS3WIDTH, BOSS3HEIGHT)) {
					pBoss3->Hp -= pSearchBullet->Atk;
					if (pBoss3->Hp <= 0) {
						pBoss3->isDead = true;
						pBoss3->isActive = false;
						PlayBackSound(MAINSCENE_SE_BOSSDEAD, false, 100);
					}
					else {
						PlayBackSound(MAINSCENE_SE_ENEMYDAMAGE, false, 100);
					}
					DeleteBullet(&pSearchBullet);
					continue;
				}
			}
		}

		//ボス4と弾のダメージ計算
		if (pBoss4->isActive && !(pBoss4->isDead)) {
			if (pSearchBullet->wasReflect) {
				if (SquareHit(&pSearchBullet->WindowPos, BulletInitializeData[bulletKind].Size, BulletInitializeData[bulletKind].Size, &pBoss4->WindowPos, BOSS4WIDTH, BOSS4HEIGHT)) {
					pBoss4->Hp -= pSearchBullet->Atk;
					if (pBoss4->Hp <= 0) {
						pBoss4->isDead = true;
						pBoss4->isActive = false;
						PlayBackSound(MAINSCENE_SE_BOSSDEAD, false, 100);
					}
					else {
						PlayBackSound(MAINSCENE_SE_ENEMYDAMAGE, false, 100);
					}
					DeleteBullet(&pSearchBullet);
					continue;
				}
			}
		}

		//プレイヤーと弾のダメージ計算と無敵時間の考慮
		
		if(!pSearchBullet->wasReflect){
				if (SquareHit(&player->WorldPos, PLAYERSIZEWIDTH, PLAYERSIZEHEIGHT - 16.0f, &pSearchBullet->WorldPos, BulletInitializeData[bulletKind].Size, BulletInitializeData[bulletKind].Size)) {

					if (!player->beInvincible) {
						PlayBackSound(MAINSCENE_SE_PLAYYERDAMAGE, false, 100);

						player->Hp -= pSearchBullet->Atk;

						player->beInvincible = true;
					}

					DeleteBullet(&pSearchBullet);
					continue;
				}
		}

		//敵とバレットのダメージ計算
		int enemyMax = GetEnemyMax();
		for (int j = 0; j < enemyMax; j++) {
			if (enemy[j].beActive == true && enemy[j].beDead == false) {
				if (pSearchBullet->wasReflect) {
					if (SquareHit(&pSearchBullet->WorldPos, BulletInitializeData[bulletKind].Size, BulletInitializeData[bulletKind].Size, &enemy[j].WorldPos, enemy[j].Width - 10, enemy[j].Height)) {

						enemy[j].Hp -= pSearchBullet->Atk;

						if (enemy[j].Hp <= 0) {
							enemy[j].beActive = false;
							enemy[j].beDead = true;
							PlayBackSound(MAINSCENE_SE_ENEMYDEAD, false, 100);
						}
						else {
							PlayBackSound(MAINSCENE_SE_ENEMYDAMAGE, false, 100);
						}
						DeleteBullet(&pSearchBullet);
						break;
					}
				}
			}
		}
	}

	//エネミーとプレイヤーの直接のあたり判定
	for (int i = 0; i < enemyMax; i++) {
		if (enemy[i].beActive && !enemy[i].beDead) {
			if (SquareHit(&player->WorldPos, PLAYERSIZEWIDTH, PLAYERSIZEHEIGHT - 10.0f, &enemy[i].WorldPos, enemy[i].Width, enemy[i].Height)) {

				if (!player->beInvincible) {
					player->Hp -= enemy[i].Atk;
					player->beInvincible = true;
					PlayBackSound(MAINSCENE_SE_PLAYYERDAMAGE, false, 100);
				}
			}
		}
	}

	//boss1とプレイヤーの直接のあたり判定
	if (pBoss1->isActive && !(pBoss1->isDead)) {
		D3DXVECTOR2 tmpBoss1 = pBoss1->WolrdPos;
		if (player->beLeft) {
			tmpBoss1.y += 40;
		}

		if (SquareHit(&player->WorldPos, PLAYERSIZEWIDTH, PLAYERSIZEHEIGHT, &pBoss1->WolrdPos, BOSS1WIDTH-20, BOSS1HEIGHT - 20)) {

			if (!player->beInvincible) {
				player->Hp -= pBoss1->Atk;
				player->beInvincible = true;
				PlayBackSound(MAINSCENE_SE_PLAYYERDAMAGE, false, 100);
			}
		}
	}
	if (pBoss2->isActive && !(pBoss2->isDead)) {
		if (SquareHit(&player->WorldPos, PLAYERSIZEWIDTH, PLAYERSIZEHEIGHT, &pBoss2->WorldPos, BOSS2WIDTH, BOSS2HEIGHT)) {

			if (!player->beInvincible) {
				player->Hp -= pBoss2->Atk;
				player->beInvincible = true;
				PlayBackSound(MAINSCENE_SE_PLAYYERDAMAGE, false, 100);
			}
		}
	}

	//boss3とプレイヤーの直接のあたり判定
	if (pBoss3->isActive && !(pBoss3->isDead)) {
		if (SquareHit(&player->WorldPos, PLAYERSIZEWIDTH, PLAYERSIZEHEIGHT, &pBoss3->WolrdPos, BOSS3OBJWIDTH, BOSS3OBJHEIGHT)) {

			if (!player->beInvincible) {
				player->Hp -= pBoss3->Atk;
				player->beInvincible = true;
				PlayBackSound(MAINSCENE_SE_PLAYYERDAMAGE, false, 100);
			}
		}
	}

	//boss4とプレイヤーの直接のあたり判定
	if (pBoss4->isActive && !(pBoss4->isDead)) {
		if (SquareHit(&player->WorldPos, PLAYERSIZEWIDTH, PLAYERSIZEHEIGHT, &pBoss4->WolrdPos, BOSS4WIDTH, BOSS4HEIGHT)) {

			if (!player->beInvincible) {
				player->Hp -= pBoss1->Atk;
				player->beInvincible = true;
				PlayBackSound(MAINSCENE_SE_PLAYYERDAMAGE, false, 100);
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

void PushOutMapFourPoint(const D3DXVECTOR2& Pos, float* MovementX, float* MovementY, float width, float height) {
	D3DXVECTOR2 Left;
	D3DXVECTOR2 Right;
	D3DXVECTOR2 Bottom;
	D3DXVECTOR2 Top;

	Left.x = Pos.x - (width / 2) + *MovementX - 1;
	Right.x = Pos.x + (width / 2) + *MovementX;
	Top.x = Bottom.x = Pos.x + *MovementX;
	Top.y = Pos.y - (height / 2) + *MovementY - 1;
	Bottom.y = Pos.y + (height / 2) + *MovementY + 1;
	Left.y = Right.y = Pos.y + *MovementY;

	MapNumXY tmpNum;
	MapchipNumberSpecify(&tmpNum, &Left);

	if (MapKindSpecify(&tmpNum) != NOTHING) {
		*MovementX -= Left.x - ((tmpNum.NumX + 1) * TIPSIZE);
	}

	MapchipNumberSpecify(&tmpNum, &Right);
	if (MapKindSpecify(&tmpNum) != NOTHING) {
		*MovementX -= Right.x - ((tmpNum.NumX) * TIPSIZE);
	}

	MapchipNumberSpecify(&tmpNum, &Top);
	if (MapKindSpecify(&tmpNum) != NOTHING) {
		*MovementY += Top.y - ((tmpNum.NumY + 1) * TIPSIZE) + 1;

	}

	MapchipNumberSpecify(&tmpNum, &Bottom);
	if (MapKindSpecify(&tmpNum) != NOTHING) {
		*MovementY -= (Bottom.y) - ((tmpNum.NumY) * TIPSIZE);
	}
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
	Bullet* pFirstBullet = GetFirstBulletAddress();
	EditableBulletData* BulletInitializeData = GetEditableBulletData();
	for (Bullet* pSearchBullet = pFirstBullet->next; pSearchBullet != NULL; pSearchBullet = pSearchBullet->next) {

		int bulletKind = pSearchBullet->BulletKind;

		D3DXVECTOR2 Left;
		D3DXVECTOR2 Right;
		D3DXVECTOR2 Bottom;
		D3DXVECTOR2 Top;

		Left.x = pSearchBullet->WorldPos.x - (BulletInitializeData[bulletKind].Size / 2) + (pSearchBullet->MovementX) -1;
		Right.x = pSearchBullet->WorldPos.x + (BulletInitializeData[bulletKind].Size / 2) + (pSearchBullet->MovementX);
		Top.x = Bottom.x = pSearchBullet->WorldPos.x + (pSearchBullet->MovementX);
		Top.y = pSearchBullet->WorldPos.y - (BulletInitializeData[bulletKind].Size / 2) + (pSearchBullet->MovementY);
		Bottom.y = pSearchBullet->WorldPos.y + (BulletInitializeData[bulletKind].Size / 2) + (pSearchBullet->MovementY) + 1;
		Left.y = Right.y = pSearchBullet->WorldPos.y + (pSearchBullet->MovementY);

		D3DXVECTOR2 tmpPos;
		tmpPos.x = pSearchBullet->WorldPos.x + pSearchBullet->MovementX;
		tmpPos.y = pSearchBullet->WorldPos.y + pSearchBullet->MovementY;

		if (MapKindSpecifyForPos(&Left) != NOTHING) {

			if (MapKindSpecifyForPos(&Left) == RIGHTNEEDLE) {
				DeleteBullet(&pSearchBullet);
				continue;
			}

			pSearchBullet->ReflectCnt++;
			if (pSearchBullet->ReflectCnt == pSearchBullet->ReflectMax) {
				DeleteBullet(&pSearchBullet);
				continue;
			}

			if (pSearchBullet->MovementY < 0) {
				pSearchBullet->Rad = 2 * D3DX_PI - Calculate_rad(tmpPos.x, tmpPos.y, pSearchBullet->WorldPos.x, pSearchBullet->WorldPos.y);
			}
			else if (pSearchBullet->MovementY > 0) {
				pSearchBullet->Rad = -1 * Calculate_rad(tmpPos.x, tmpPos.y, pSearchBullet->WorldPos.x, pSearchBullet->WorldPos.y);
			}
			else if (pSearchBullet->MovementY == 0) {
				pSearchBullet->Rad += D3DX_PI;
			}

		}

		else if (MapKindSpecifyForPos(&Right) != NOTHING) {

			if (MapKindSpecifyForPos(&Right) == LEFTNEEDLE) {
				DeleteBullet(&pSearchBullet);
				continue;
			}

			pSearchBullet->ReflectCnt++;
			if (pSearchBullet->ReflectCnt == pSearchBullet->ReflectMax) {
				DeleteBullet(&pSearchBullet);
				continue;
			}

			if (pSearchBullet->MovementY < 0) {
				pSearchBullet->Rad = D3DX_PI - (Calculate_rad(tmpPos.x, tmpPos.y, pSearchBullet->WorldPos.x, pSearchBullet->WorldPos.y) - D3DX_PI);
			}
			else if (pSearchBullet->MovementY > 0) {
				pSearchBullet->Rad = D3DX_PI + (D3DX_PI - Calculate_rad(tmpPos.x, tmpPos.y, pSearchBullet->WorldPos.x, pSearchBullet->WorldPos.y));
			}
			else if (pSearchBullet->MovementY == 0) {
				pSearchBullet->Rad += D3DX_PI;
			}

		}
		else if (MapKindSpecifyForPos(&Bottom) != NOTHING) {

			if (MapKindSpecifyForPos(&Bottom) == UPNEEDLE) {
				DeleteBullet(&pSearchBullet);
				continue;
			}

			pSearchBullet->ReflectCnt++;
			if (pSearchBullet->ReflectCnt == pSearchBullet->ReflectMax) {
				DeleteBullet(&pSearchBullet);
				continue;
			}

			if (pSearchBullet->MovementX < 0) {
				pSearchBullet->Rad = (0.5*D3DX_PI) + ((0.5*D3DX_PI) - Calculate_rad(tmpPos.x, tmpPos.y, pSearchBullet->WorldPos.x, pSearchBullet->WorldPos.y));
			}
			else if (pSearchBullet->MovementX > 0) {
				pSearchBullet->Rad = (0.5*D3DX_PI) - (Calculate_rad(tmpPos.x, tmpPos.y, pSearchBullet->WorldPos.x, pSearchBullet->WorldPos.y) - (0.5*D3DX_PI));
			}
			else if (pSearchBullet->MovementX == 0) {
				pSearchBullet->Rad += D3DX_PI;
			}

		}
		else if (MapKindSpecifyForPos(&Top) != NOTHING) {
			pSearchBullet->ReflectCnt++;
			if (pSearchBullet->ReflectCnt == pSearchBullet->ReflectMax) {
				DeleteBullet(&pSearchBullet);
				continue;
			}

			if (pSearchBullet->MovementX < 0) {
				pSearchBullet->Rad = (1.5*D3DX_PI) - (Calculate_rad(tmpPos.x, tmpPos.y, pSearchBullet->WorldPos.x, pSearchBullet->WorldPos.y) - (1.5*D3DX_PI));
			}
			else if (pSearchBullet->MovementX > 0) {
				pSearchBullet->Rad = (1.5*D3DX_PI) + ((1.5*D3DX_PI) - Calculate_rad(tmpPos.x, tmpPos.y, pSearchBullet->WorldPos.x, pSearchBullet->WorldPos.y));
			}
			else if (pSearchBullet->MovementX == 0) {
				pSearchBullet->Rad += D3DX_PI;
			}

			SetBulletMovement(pSearchBullet);
		}
	}
}