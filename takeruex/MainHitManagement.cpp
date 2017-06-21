#include"MainHitManagement.h"
#include"PlayerControl.h"
#include"PlayerRender.h"
#include"BulletControl.h"
#include"BulletRender.h"
#include"EnemyControl.h"


void HitManage() {

	Player* player = GetplayerData();
	Bullet* bullet=GetBullet();

	//プレイヤーのダメージ計算と無敵時間の考慮
	static int frcnt;
	if (!player->beInvincible) {
		if (SquareHit(&player->WindowPos, PLAYERSIZEWIDHE, PLAYERSIZEHEIGHT, &bullet->WindowPos, bullet->Size, bullet->Size)) {

			player->Hp -= bullet->Atk;

			player->beInvincible = true;
		}
	}
	if (player->beInvincible) {
		frcnt++;
	}
	if (frcnt >= 90) {
		frcnt = 0;
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

bool SquareHit(D3DXVECTOR2* pPos1, float width1, float height1, D3DXVECTOR2* pPos2, float width2, float height2)
{
	D3DXVECTOR2 LeftTop1;
	D3DXVECTOR2 RightTop1;
	D3DXVECTOR2 RightBottom1;
	D3DXVECTOR2 LeftBottom1;
	D3DXVECTOR2 LeftTop2;
	D3DXVECTOR2 RightTop2;
	D3DXVECTOR2 RightBottom2;
	D3DXVECTOR2 LeftBottom2;

	LeftTop1.x = LeftBottom1.x = pPos1->x - width1 / 2;
	RightTop1.x = RightBottom1.x = pPos1->x + width1 / 2;
	LeftTop1.y = RightTop1.y = pPos1->y - height1 / 2;
	RightBottom1.y = LeftBottom1.y = pPos1->y + height1 / 2;
	LeftTop2.x = LeftBottom2.x = pPos2->x - width2 / 2;
	RightTop2.x = RightBottom2.x = pPos2->x + width2 / 2;
	LeftTop2.y = RightTop2.y = pPos2->y - height2 / 2;
	RightBottom2.y = LeftBottom2.y = pPos2->y + height2 / 2;

	if(LeftTop1.x < RightTop2.x){
		if (LeftTop2.x < RightTop1.x) {
			if (LeftTop1 < RightBottom2) {
				if (LeftTop2 < RightBottom1) {
					return true;
				}
			}
		}
	}
	else {
		return false;
	}
}