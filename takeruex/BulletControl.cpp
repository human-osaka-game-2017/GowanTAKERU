#include "BulletControl.h"
#include"PlayerControl.h"
#include"EnemyControl.h"
#include"MainControl.h"
#include"MapControl.h"
#include"CommonRender.h"
#include"FileManagement.h"

#define FIREWORKSMAXFR 180

static Bullet g_firstBullet;
static EditableBulletData g_BulletInitialData[BULLET_MAX];

Bullet* GetFirstBulletAddress() {
	return &g_firstBullet;
}

void LoadBulletData() {
	LoadBulletDataCSV(g_BulletInitialData);
}

EditableBulletData* GetEditableBulletData() {
	return g_BulletInitialData;
}

void BulletInit() {

	g_firstBullet.next = NULL;
	g_firstBullet.previous = &g_firstBullet;

}

void BulletCreate(const D3DXVECTOR2& launchingSite, int bulletKind, float plusDeg) {

	//最後尾の弾の実体のアドレスまで移動
	Bullet* pSearchBullet = &g_firstBullet;
	while (1) {
		if (pSearchBullet->next == NULL) {
			break;
		}
		pSearchBullet = pSearchBullet->next;
	}

	//bulletを生成
	Bullet* newBullet = (Bullet*)malloc(sizeof(Bullet));

	//連結
	pSearchBullet->next = newBullet;
	newBullet->previous = pSearchBullet;

	//初期化
	newBullet->next = NULL;
	newBullet->BulletKind = bulletKind;
	newBullet->WorldPos = launchingSite;
	D3DXVECTOR2 BasePoint0 = D3DXVECTOR2(DISPLAY_WIDTH / 2, DISPLAY_HEIGHT / 2);
	D3DXVECTOR2* pBasePoint = GetBasePoint();
	newBullet->WindowPos.x = newBullet->WorldPos.x - (pBasePoint->x - BasePoint0.x);
	newBullet->WindowPos.y = newBullet->WorldPos.y - (pBasePoint->y - BasePoint0.y);
	newBullet->wasReflect = false;
	newBullet->MovementX = newBullet->MovementY = 0;
	newBullet->ReflectCnt = 0;
	newBullet->frcnt = 0;

	Player* pPlayer = GetplayerData();

	newBullet->Atk = g_BulletInitialData[bulletKind].Atk;
	newBullet->Speed = g_BulletInitialData[bulletKind].Speed;
	newBullet->ReflectMax = g_BulletInitialData[bulletKind].ReflectMax;
	newBullet->SaveCoordinate = pPlayer->WindowPos;

	if (bulletKind / 10 == 0 || bulletKind / 10 == 2 || bulletKind == FIREWORKS) {
		newBullet->Rad = 0;
	}
	else {
		newBullet->Rad = Calculate_rad(
			newBullet->WindowPos.x,
			newBullet->WindowPos.y,
			newBullet->SaveCoordinate.x,
			newBullet->SaveCoordinate.y
		);
	}

	newBullet->Rad += D3DXToRadian(plusDeg);
}

void BulletControl() {

	Player* player = GetplayerData();

	for (Bullet* pSearchBullet = g_firstBullet.next; pSearchBullet != NULL; pSearchBullet = pSearchBullet->next) {

		//画面外で消える
		if (pSearchBullet->WindowPos.x < -64 || DISPLAY_WIDTH + 64 < pSearchBullet->WindowPos.x) {
			DeleteBullet(&pSearchBullet);
			continue;
		}
		if (pSearchBullet->WindowPos.y < -64 || DISPLAY_HEIGHT + 64 < pSearchBullet->WindowPos.y) {
			DeleteBullet(&pSearchBullet);
			continue;
		}

		if (pSearchBullet->BulletKind / 10 == 4) {
			if (!pSearchBullet->wasReflect) {
				pSearchBullet->SaveCoordinate = player->WindowPos;
				pSearchBullet->Rad = Calculate_rad(
					pSearchBullet->WindowPos.x,
					pSearchBullet->WindowPos.y,
					pSearchBullet->SaveCoordinate.x,
					pSearchBullet->SaveCoordinate.y
				);
			}
		}

		if (pSearchBullet->BulletKind == FIREWORKS) {
			if (pSearchBullet->frcnt == FIREWORKSMAXFR) {
				BulletCreate(pSearchBullet->WorldPos, BULLETNORMAL2, 0.0f);
				BulletCreate(pSearchBullet->WorldPos, BULLETNORMAL2, 45.0f);
				BulletCreate(pSearchBullet->WorldPos, BULLETNORMAL2, 90.0f);
				BulletCreate(pSearchBullet->WorldPos, BULLETNORMAL2, 135.0f);
				BulletCreate(pSearchBullet->WorldPos, BULLETNORMAL2, 180.0f);
				BulletCreate(pSearchBullet->WorldPos, BULLETNORMAL2, 225.0f);
				BulletCreate(pSearchBullet->WorldPos, BULLETNORMAL2, 270.0f);
				BulletCreate(pSearchBullet->WorldPos, BULLETNORMAL2, 315.0f);

				DeleteBullet(&pSearchBullet);

			}
			else {
				pSearchBullet->frcnt++;
			}
		}

		SetBulletMovement(pSearchBullet);
	}
}

void MoveBullet() {
	D3DXVECTOR2 BasePoint0 = D3DXVECTOR2(DISPLAY_WIDTH / 2, DISPLAY_HEIGHT / 2);
	D3DXVECTOR2* basePoint = GetBasePoint();
	for (Bullet* pSearchBullet = g_firstBullet.next; pSearchBullet != NULL; pSearchBullet = pSearchBullet->next) {
		
		pSearchBullet->WorldPos.x += pSearchBullet->MovementX;
		pSearchBullet->WorldPos.y += pSearchBullet->MovementY;
		pSearchBullet->WindowPos.x = pSearchBullet->WorldPos.x - (basePoint->x - BasePoint0.x);
		pSearchBullet->WindowPos.y = pSearchBullet->WorldPos.y - (basePoint->y - BasePoint0.y);
		pSearchBullet->MovementX = pSearchBullet->MovementY = 0;
		}
	}

void SetBulletMovement(Bullet* pBullet) {
	pBullet->MovementX = pBullet->Speed*cos(pBullet->Rad);
	pBullet->MovementY = -(pBullet->Speed*sin(pBullet->Rad));
}

void DeleteBullet(Bullet** DeletionBullet) {
	Bullet* pre = (*DeletionBullet)->previous;
	((*DeletionBullet)->previous)->next = (*DeletionBullet)->next;
	if ((*DeletionBullet)->next != NULL) {
		((*DeletionBullet)->next)->previous = (*DeletionBullet)->previous;

	}
	free(*DeletionBullet);
	*DeletionBullet = pre;
}

void DeleteALLBullet() {

	Bullet* pSearchBullet = &g_firstBullet;
	while (1) {

		//最後尾の時
		if (pSearchBullet->next == NULL) {
			g_firstBullet.next = NULL;
			//g_firstBulletはmallocじゃないから開放しなくていい
			if (pSearchBullet == &g_firstBullet) {
				break;
			}
			free(pSearchBullet);
			break;
		}

		pSearchBullet = pSearchBullet->next;
		//g_firstBulletはmallocじゃないから開放しなくていい
		if (pSearchBullet->previous != &g_firstBullet) {
			free(pSearchBullet->previous);
		}
	}
}