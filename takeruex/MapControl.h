#ifndef MAPCONTROL_H
#define MAPCONTROL_H

#include<d3dx9.h>
#include"MapRender.h"

enum Direction{UP,RIGHTUP,RIGHT,RIGHTDOWN,DOWN,LEFTDOWN,LEFT,LEFTUP};

struct MapNumXY {
	int NumX, NumY;
};

//第一引数ーーーー↑の構造体でXのマップチップ番号、Yのマップチップ番号を返してくれます
//第二引数ーーーーワールド座標を入れてください
void MapchipNumberSpecify(MapNumXY* pMapNumXY, D3DXVECTOR2* pWorldPos);

//マップチップの番号をいれて
//座標を返してくれます
void PosSpecifyForMapchipNumber(D3DXVECTOR2* pWorldPos, MapNumXY* pMapNumXY);

int MapKindSpecify(MapNumXY* pMapNumXY);

int MapKindSpecify_Plus1(MapNumXY* pMapNumXY, Direction direction);

#endif 

