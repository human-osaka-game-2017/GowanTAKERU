#ifndef BOSS2CONTROL_H
#define BOSS2CONTROL_H

#include<d3dx9.h>

#define BOSS2WIDTH 168
#define BOSS2HEIGHT 197

enum BOSS2STATE {
	UDMOVE,GOLEFT,GOPOSB
};

struct Boss2Data {
	D3DXVECTOR2 WorldPos;
	D3DXVECTOR2 WindowPos;
	BOSS2STATE boss2State;//ルート分岐した位置を覚えておく
	float MovementX, MovementY;
	int Hp;
	int Atk;
	bool UporDown_UP;
	bool hasDamage;//ダメージを与えられたかどうかのフラグ
	bool isLeft;//向きtrueで左向き
	bool isDead;//生死
	bool isActive;//描画アクティブかどうか
	bool isExistence;//存在しているかどうか
	int lastbullet;//前に打った球がどれか記憶する
};




//プロトタイプ群
void Boss2Init();
void Boss2Control();
void MoveBoss2();
Boss2Data* GetBoss2Data();//ボス２のデータ

#endif