#ifndef BOSS2CONTROL_H
#define BOSS2CONTROL_H
#include<d3dx9.h>

struct Boss2Data {
	D3DXVECTOR2 WorldPos;
	D3DXVECTOR2 WindowPos;
	float MovementX, MovementY;
	int Hp;
	int saveShotFrmcnt;
	int saveActionCntForNORMALSHOT;
	int saveActionCntForDUALSHOT;
	int saveDUALSHOTActionCntForLARIAT;
	int Atk;
	bool isLeft;//向きtrueで左向き
	bool isDead;//生死
	bool isActive;//描画アクティブかどうか
	bool isExistence;//存在しているかどうか
};




//プロトタイプ群
void Boss2Init();
void Boss2Control();
void MoveBoss2();
Boss2Data* GetBoss2Data();//ボス２のデータ

#endif