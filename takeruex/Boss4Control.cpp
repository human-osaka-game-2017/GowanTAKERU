#include"Boss4Control.h"
#include"StageSelect.h"
#include"FileManagement.h"
#include"MapRender.h"
#include"MapControl.h"
#include"PlayerControl.h"
#include"CommonRender.h"
#include"BulletControl.h"

#include<time.h>

#define JUMPV0 -6.0f
#define HIGHJUMPV0 -15.0f
#define BOSS4GRAVITY 0.2f
#define BOSS4SPEED 5.0f

struct Pos {
	float posX, posY;
};

enum BOSS4POS {
	SKY, POINT_A, POINT_B, POINT_C, INITIALPOINT
};

//プロトタイプ群
int Random(int min, int max);
bool CheckInPoint(const Pos& Point, const D3DXVECTOR2& LeftTop, const D3DXVECTOR2& RightBottom);
bool Boss4InitialControl();
void Boss4BasicControl();

Boss4Data g_Boss4;
static bool g_isControlInitial = false;

Boss4Data* GetBoss4Data() {
	return &g_Boss4;
}

void Boss4Init() {
	g_Boss4.isExistence = false;
	g_isControlInitial = false;

	STAGE_ID stage_ID = GetStage_ID();
	int MaxX = GetStageXYMAX(stage_ID, X);
	int MaxY = GetStageXYMAX(stage_ID, Y);
	int* gimmickData = (int*)malloc(sizeof(int)*MaxX*MaxY);

	switch (stage_ID) {
	case STAGE_3:

		CSVLoad("CSV/mainscene/stage3_gimmick.csv", gimmickData, MaxY, MaxX);//CSV呼び出し

		for (int i = 0; i < MaxY; i++) {
			for (int j = 0; j < MaxX; j++) {
				if (gimmickData[j + i*MaxX] == BOSS_STAGE4) {
					g_Boss4.isExistence = true;
					g_Boss4.WolrdPos.x = TIPSIZE*j;
					g_Boss4.WolrdPos.y = TIPSIZE*i;
					goto BREAK;
				}
			}
		}
		break;

	case STAGE_5:

		CSVLoad("CSV/mainscene/stage5_gimmick.csv", gimmickData, MaxY, MaxX);//CSV呼び出し

		for (int i = 0; i < MaxY; i++) {
			for (int j = 0; j < MaxX; j++) {
				if (gimmickData[j + i*MaxX] == BOSS_STAGE4) {
					g_Boss4.isExistence = true;
					g_Boss4.WolrdPos.x = TIPSIZE*j;
					g_Boss4.WolrdPos.y = TIPSIZE*i;
					goto BREAK;
				}
			}
		}
	}

BREAK:
	if (g_Boss4.isExistence) {
		g_Boss4.Boss4JumpState = GROUND;
		g_Boss4.MovementX = g_Boss4.MovementY = 0;
		g_Boss4.Hp = 100;
		g_Boss4.Atk = 20;
		g_Boss4.FrCnt = 0;
		g_Boss4.JumpFrCnt = 0;
		g_Boss4.ga = 0;
		g_Boss4.isJumping = false;
		g_Boss4.isLeft = true;
		g_Boss4.isDead = false;
		g_Boss4.isActive = false;

	}
	else {
		g_Boss4.isDead = false;
		g_Boss4.isActive = false;
	}

	free(gimmickData);
}

void Boss4Control() {

	if (g_Boss4.isExistence && !g_Boss4.isDead) {

		//活動状態かチェック
		D3DXVECTOR2 BasePoint0 = D3DXVECTOR2(DISPLAY_WIDTH / 2, DISPLAY_HEIGHT / 2);
		D3DXVECTOR2* basePoint = GetBasePoint();
		g_Boss4.WindowPos.x = g_Boss4.WolrdPos.x - (basePoint->x - BasePoint0.x);
		g_Boss4.WindowPos.y = g_Boss4.WolrdPos.y - (basePoint->y - BasePoint0.y);
		if (-(2 * TIPSIZE) < g_Boss4.WindowPos.x && g_Boss4.WindowPos.x < DISPLAY_WIDTH + (2 * TIPSIZE) &&
			-(2 * TIPSIZE) < g_Boss4.WindowPos.y && g_Boss4.WindowPos.y < DISPLAY_HEIGHT + (2 * TIPSIZE)) {
			g_Boss4.isActive = true;
		}
		else {
			g_Boss4.isActive = false;
		}

		//活動状態である
		if (g_Boss4.isActive) {
			if (!g_isControlInitial) {
				g_isControlInitial = Boss4InitialControl();
			}
			else {
				Boss4BasicControl();
			}
		}
	}
}

bool Boss4InitialControl() {

	float moveSpeed = -3.0f;
	float jumpPower = -10.0f;

	D3DXVECTOR2 LeftBottom = { (g_Boss4.WolrdPos.x - BOSS4WIDTH / 2),(g_Boss4.WolrdPos.y + BOSS4HEIGHT / 2) + 1 };
	D3DXVECTOR2 RightBottom = { (g_Boss4.WolrdPos.x + BOSS4WIDTH / 2),(g_Boss4.WolrdPos.y + BOSS4HEIGHT / 2) + 1 };
	if (g_Boss4.isJumping) {

		g_Boss4.MovementX = moveSpeed;

		if (MapKindSpecifyForPos(&LeftBottom) != NOTHING || MapKindSpecifyForPos(&RightBottom) != NOTHING) {
			g_Boss4.isJumping = false;
			g_Boss4.FrCnt = 0;
			g_Boss4.ga = 0;
			g_Boss4.MovementX = 0;
			return true;
		}
		
	}

	if (g_Boss4.FrCnt == 120) {
		g_Boss4.isJumping = true;
		g_Boss4.ga = jumpPower;
	}

	g_Boss4.FrCnt++;

	g_Boss4.ga += BOSS4GRAVITY;
	g_Boss4.MovementY = g_Boss4.ga;
	return false;
}

void Boss4BasicControl() {

	//データ群
	Pos pointA = { 371 * TIPSIZE,20 * TIPSIZE };
	Pos pointB = { 379 * TIPSIZE,16 * TIPSIZE };
	Pos pointC = { 387 * TIPSIZE,20 * TIPSIZE };

	D3DXVECTOR2 LeftTop = { (g_Boss4.WolrdPos.x - BOSS4WIDTH / 2),(g_Boss4.WolrdPos.y - BOSS4HEIGHT / 2) };
	D3DXVECTOR2 RightTop = { (g_Boss4.WolrdPos.x + BOSS4WIDTH / 2),(g_Boss4.WolrdPos.y - BOSS4HEIGHT / 2) };
	D3DXVECTOR2 LeftBottom = { (g_Boss4.WolrdPos.x - BOSS4WIDTH / 2),(g_Boss4.WolrdPos.y + BOSS4HEIGHT / 2) + 1 };
	D3DXVECTOR2 RightBottom = { (g_Boss4.WolrdPos.x + BOSS4WIDTH / 2),(g_Boss4.WolrdPos.y + BOSS4HEIGHT / 2) + 1 };

	Player* pPlayer = GetplayerData();

	//左向きかを決める
	if (pPlayer->WorldPos.x <= g_Boss4.WolrdPos.x) {
		g_Boss4.isLeft = true;
	}
	else {
		g_Boss4.isLeft = false;
	}

	//3秒に一回打つ処理
	if (g_Boss4.FrCnt == 180) {
		BulletCreate(g_Boss4.WolrdPos, NONREFLECTTARGET1);
	}
	if (g_Boss4.FrCnt == 360) {
		BulletCreate(g_Boss4.WolrdPos, HOMING);
		g_Boss4.FrCnt = 0;
	}
	g_Boss4.FrCnt++;

	//jump頂点時の処理を何度も行わないためのフラグ
	static bool isJumpTop = false;

	//jump中
	if (g_Boss4.isJumping) {

		//着地処理
		if (MapKindSpecifyForPos(&LeftBottom) != NOTHING || MapKindSpecifyForPos(&RightBottom) != NOTHING) {

			g_Boss4.isJumping = false;
			g_Boss4.ga = 0;
			g_Boss4.Boss4JumpState = GROUND;

			//着地時に打つ処理
			if (CheckInPoint(pointA, LeftTop, RightBottom) || CheckInPoint(pointC, LeftTop, RightBottom)) {
				if (g_Boss4.isLeft) {
					BulletCreate(g_Boss4.WolrdPos, BULLETNORMAL2, 180.0f);
					BulletCreate(g_Boss4.WolrdPos, BULLETNORMAL3, 135.0f);
				}
				else {
					BulletCreate(g_Boss4.WolrdPos, BULLETNORMAL2, 0.0f);
					BulletCreate(g_Boss4.WolrdPos, BULLETNORMAL3, 45.0f);
				}
			}
			else if (CheckInPoint(pointB, LeftTop, RightBottom)) {
				BulletCreate(g_Boss4.WolrdPos, BULLETNORMAL1, 210.0f);
				BulletCreate(g_Boss4.WolrdPos, BULLETNORMAL1, 240.0f);
				BulletCreate(g_Boss4.WolrdPos, BULLETNORMAL1, 300.0f);
				BulletCreate(g_Boss4.WolrdPos, BULLETNORMAL1, 330.0f);
			}
		}

		//ジャンプ頂点時に打つ処理
		if (!isJumpTop) {
			if (0 < g_Boss4.ga) {
				isJumpTop = true;
				switch (g_Boss4.Boss4JumpState) {

				case JUMP:
				case LEFTJUMP:
				case RIGHTJUMP:
					BulletCreate(g_Boss4.WolrdPos, BULLETTARGET2);

				case LEFTHIGHJUMP:
				case RIGHTHIGHJUMP:
					BulletCreate(g_Boss4.WolrdPos, BULLETNORMAL1, 210.0f);
					BulletCreate(g_Boss4.WolrdPos, BULLETNORMAL1, 240.0f);
					BulletCreate(g_Boss4.WolrdPos, BULLETNORMAL1, 300.0f);
					BulletCreate(g_Boss4.WolrdPos, BULLETNORMAL1, 330.0f);
					break;

				}
			}
		}
	}

	//ただ立っているときの処理
	else {

		g_Boss4.JumpFrCnt++;
		g_Boss4.ga = 0;

		//jumpする処理
		if (g_Boss4.JumpFrCnt == 30) {

			g_Boss4.JumpFrCnt = 0;
			g_Boss4.isJumping = true;
			isJumpTop = false;

			//どこに飛ぶのかを決める
			//まず今どこにいるのか
			BOSS4POS currentPoint;
			if (CheckInPoint(pointA, LeftTop, RightBottom)) {
				currentPoint = POINT_A;
			}
			if (CheckInPoint(pointB, LeftTop, RightBottom)) {
				currentPoint = POINT_B;
			}
			if (CheckInPoint(pointC, LeftTop, RightBottom)) {
				currentPoint = POINT_C;
			}

			//それを踏まえてランダムで決める
			do {
				g_Boss4.Boss4JumpState = (BOSS4JUMPSTATE)Random(1, 4);
				if (g_Boss4.Boss4JumpState == JUMP) {
					break;
				}
				if (currentPoint == POINT_A) {
					if (g_Boss4.Boss4JumpState == RIGHTJUMP || g_Boss4.Boss4JumpState == RIGHTHIGHJUMP) {
						break;
					}
				}
				if (currentPoint == POINT_B) {
					if (g_Boss4.Boss4JumpState == RIGHTJUMP || g_Boss4.Boss4JumpState == LEFTJUMP) {
						break;
					}
				}
				if (currentPoint == POINT_C) {
					if (g_Boss4.Boss4JumpState == LEFTJUMP || g_Boss4.Boss4JumpState == LEFTHIGHJUMP) {
						break;
					}
				}
			} while (1);
			
			switch (g_Boss4.Boss4JumpState) {

			case JUMP:
			case LEFTJUMP:
			case RIGHTJUMP:
				g_Boss4.ga = JUMPV0;
				break;

			case LEFTHIGHJUMP:
			case RIGHTHIGHJUMP:
				g_Boss4.ga = HIGHJUMPV0;
				break;

			}
		}
	}

	//移動量の決定
	switch (g_Boss4.Boss4JumpState) {

	case GROUND:
	case JUMP:
		g_Boss4.MovementX = 0;
		break;

	case RIGHTJUMP:
	case RIGHTHIGHJUMP:
		g_Boss4.MovementX = BOSS4SPEED;
		break;

	case LEFTJUMP:
	case LEFTHIGHJUMP:
		g_Boss4.MovementX = -BOSS4SPEED;
		break;
	}

	g_Boss4.ga += BOSS4GRAVITY;
	g_Boss4.MovementY = g_Boss4.ga;
}

void MoveBoss4() {
	if (g_Boss4.isExistence && !g_Boss4.isDead) {
		D3DXVECTOR2 BasePoint0 = D3DXVECTOR2(DISPLAY_WIDTH / 2, DISPLAY_HEIGHT / 2);
		D3DXVECTOR2* basePoint = GetBasePoint();
		g_Boss4.WolrdPos.x += g_Boss4.MovementX;
		g_Boss4.WolrdPos.y += g_Boss4.MovementY;
		g_Boss4.WindowPos.x = g_Boss4.WolrdPos.x - (basePoint->x - BasePoint0.x);
		g_Boss4.WindowPos.y = g_Boss4.WolrdPos.y - (basePoint->y - BasePoint0.y);
		g_Boss4.MovementX = g_Boss4.MovementY = 0;
	}
}

bool CheckInPoint(const Pos& Point, const D3DXVECTOR2& LeftTop, const D3DXVECTOR2& RightBottom) {
	if (LeftTop.x < Point.posX &&  Point.posX < RightBottom.x) {
		if (LeftTop.y < Point.posY && Point.posY < RightBottom.y) {
			return true;
		}
	}
	return false;
}

//-----------------------------------
//ランダム生成関数
//-----------------------------------
//min<=random<=max
int Random(int min, int max) {
	static bool seedrandom = false;

	if (seedrandom == false) {
		srand((unsigned int)time(NULL));
		seedrandom = true;
	}

	return min + rand() % (max + 1);
}