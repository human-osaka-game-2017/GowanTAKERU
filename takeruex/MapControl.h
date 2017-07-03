#ifndef MAPCONTROL_H
#define MAPCONTROL_H

#include<d3dx9.h>
#include"MapRender.h"

enum Direction { UP, RIGHTUP, RIGHT, RIGHTDOWN, DOWN, LEFTDOWN, LEFT, LEFTUP };

struct MapNumXY {
	int NumX, NumY;
};

void MapComtrol();

//第一引数ーーーーMapNumXYの構造体でXのマップチップ番号、Yのマップチップ番号を返してくれます
//第二引数ーーーーワールド座標を入れてください
void MapchipNumberSpecify(MapNumXY* pMapNumXY, D3DXVECTOR2* pWorldPos);

//第一引数ーーーーマップチップの番号(MapNumXY)をいれて
//第二引数ーーーーワールド座標を返してくれます
void PosSpecifyForMapchipNumber(D3DXVECTOR2* pWorldPos, MapNumXY* pMapNumXY);

//マップチップの番号(MapNumXY)をいれて、そのマップチップの種類を返してくれます
int MapKindSpecify(MapNumXY* pMapNumXY);

//ワールド座標を入れて、そこのマップチップの番号を返してくれます
int MapKindSpecifyForPos(D3DXVECTOR2* pWorldPos);

//第一引数ーーーーマップチップの番号(MapNumXY)をいれて
//第二引数ーーーー方向（Direction）を指定するとその方向のマップチップの種類を返してくれます
int MapKindSpecify_Plus1(MapNumXY* pMapNumXY, Direction direction);

int CalculateNumInRange(float l) ;

#endif 

