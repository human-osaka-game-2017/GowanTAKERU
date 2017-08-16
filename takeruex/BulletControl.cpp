#include "BulletControl.h"
#include"PlayerControl.h"
#include"EnemyControl.h"
#include"MainControl.h"
#include"MapControl.h"
#include"CommonRender.h"

static Bullet g_bullet[BULLETNUMBER];
static Bullet g_firstBullet;

Bullet* GetBullet() {
	return g_bullet;
}

Bullet* GetFirstBulletAddress() {
	return &g_firstBullet;
}

void BulletInit() {

	g_firstBullet.next = NULL;
	g_firstBullet.previous = &g_firstBullet;

}

void BulletCreate(int bulletNum,int enemyNum, BULLETKIND bulletKind) {

	Enemy* enemy = GetenemyData();
	Player* player = GetplayerData();

	g_bullet[bulletNum].beActive = true;
	g_bullet[bulletNum].WindowPos = enemy[enemyNum].WindowPos;
	g_bullet[bulletNum].WorldPos = enemy[enemyNum].WorldPos;

	switch (bulletKind) {

	case BULLET01:
		g_bullet[bulletNum].Speed = 6.0f;
		g_bullet[bulletNum].Size = 22;
		g_bullet[bulletNum].ReflectMax = 3;
		g_bullet[bulletNum].SaveCoordinate = player->WindowPos;

		g_bullet[bulletNum].Rad = Calculate_rad(
			g_bullet[bulletNum].WindowPos.x,
			g_bullet[bulletNum].WindowPos.y,
			g_bullet[bulletNum].SaveCoordinate.x,
			g_bullet[bulletNum].SaveCoordinate.y
		);
		break;

	}

}

void BulletCreate(const D3DXVECTOR2& launchingSite, BULLETKIND bulletKind) {

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

	Player* pPlayer = GetplayerData();

	switch (bulletKind) {

	case BULLET01:
		newBullet->Speed = 6.0f;
		newBullet->Atk = 10;
		newBullet->Size = 22;
		newBullet->ReflectMax = 3;
		newBullet->SaveCoordinate = pPlayer->WindowPos;
		newBullet->Rad = Calculate_rad(
			newBullet->WindowPos.x,
			newBullet->WindowPos.y,
			newBullet->SaveCoordinate.x,
			newBullet->SaveCoordinate.y
		);
		break;
	case HOMING:
		newBullet->Speed = 6.0f;
		newBullet->Atk = 10;
		newBullet->Size = 22;
		newBullet->ReflectMax = 3;
		newBullet->SaveCoordinate = pPlayer->WindowPos;
		newBullet->Rad = Calculate_rad(
			newBullet->WindowPos.x,
			newBullet->WindowPos.y,
			newBullet->SaveCoordinate.x,
			newBullet->SaveCoordinate.y
		);
		break;

	}
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

		if (pSearchBullet->BulletKind == HOMING) {
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


void SetBulletMovement(int bulletNum) {
	g_bullet[bulletNum].MovementX += g_bullet[bulletNum].Speed*cos(g_bullet[bulletNum].Rad);
	g_bullet[bulletNum].MovementY -= g_bullet[bulletNum].Speed*sin(g_bullet[bulletNum].Rad);
}

void SetBulletMovement(Bullet* pBullet) {
	pBullet->MovementX = pBullet->Speed*cos(pBullet->Rad);
	pBullet->MovementY = -(pBullet->Speed*sin(pBullet->Rad));
}

void DeactivateBullet(int num) {
	g_bullet[num].beActive = false;
	g_bullet[num].wasReflect = false;
}

void DeleteBullet(Bullet** DeletionBullet) {
	Bullet* pre = (*DeletionBullet)->previous;
	((*DeletionBullet)->previous)->next = (*DeletionBullet)->next;
	((*DeletionBullet)->next)->previous = (*DeletionBullet)->previous;
	free(*DeletionBullet);
	*DeletionBullet = pre;
}

void DeleteALLBullet() {

	Bullet* pSearchBullet = &g_firstBullet;
	while (1) {

		//最後尾の時
		if (pSearchBullet->next == NULL) {

			//g_firstBulletはmallocじゃないから開放しなくていい
			if (pSearchBullet != &g_firstBullet) {
				free(pSearchBullet);
				break;
			}
		}

		pSearchBullet = pSearchBullet->next;
		free(pSearchBullet->previous);
	}
}