#ifndef BOSS4CONTROL_H
#define BOSS4CONTROL_H

#include<d3dx9.h>

#define BOSS4WIDTH 114
#define BOSS4HEIGHT 152

#define BOSS4TELEPORTFRM 40.0f
#define BOSS4WAAITTIME 30

enum BOSS4TELEPORTSTATE {
	TELEPORT_START,
	TELEPORT_FIRSTHALF,
	TELEPORT_HARF,
	TELEPORT_SECONDHALF,
	TELEPORT_FIN,
	FADEOUT_START = 10,
	FADEOUT_FIRSTHALF,
	FADEOUT_HARF,
	FADEOUT_SECONDHALF,
	FADEOUT_FIN,
	NONMOVE
};

enum BOSS4POINT {
	POINT_A,
	POINT_B,
	POINT_C,
	POINT_D,
	POINT_E,
	POINT_F,
	POINT_G,
	POINT_H
};

enum BOSS4ACTIONLIST {
	GO_ACF,
	GO_ABD,
	GO_DBA,
	GO_DEF,
	GO_FED,
	GO_FCA,
	GO_AG,
	GO_DG,
	GO_FG,
	GO_GA,
	GO_GD,
	GO_GF,
	GO_AH,
	GO_DH,
	GO_FH,
	GO_GH,
	GO_H_LARIAT_D,
	TELPORTCONTINUE,
	NONACTION
};

enum BOSS4ACTUONSTATE {
	ACTION_START,
	ACTION_FIRSTHALF,
	ACTION_HALF,
	ACTION_SECONDHALF,
	ACTION_FIN,
};

struct Boss4Data {
	D3DXVECTOR2 WolrdPos;
	D3DXVECTOR2 WindowPos;
	BOSS4TELEPORTSTATE Boss4TeleportState;
	BOSS4ACTIONLIST BOSS4ACTIONLIST;
	BOSS4ACTUONSTATE currentActionState;
	BOSS4POINT currentPoint;
	int Hp;
	int Atk;
	int shotFrCnt;
	int waitFrCnt;
	int TeleportFrCnt;
	int ActionWaitFrCnt;
	float MovementX, MovementY;
	bool isLeft;
	bool isDead;
	bool isActive;
	bool isExistence;//ë∂ç›ÇµÇƒÇ¢ÇÈÇ©Ç«Ç§Ç©
	bool goNextStage;
};

Boss4Data* GetBoss4Data();

void Boss4Init();

void Boss4Control();

void MoveBoss4();

#endif
