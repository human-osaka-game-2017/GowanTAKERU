#ifndef MAPRENDER_H
#define MAPRENDER_H

#include<d3dx9.h>

//マップチップ全体の個数です
#define MAPCHIPNUM_HEIGHT 30
#define MAPCHIPNUM_WIDTH 350

//描画するマップチップの個数です
#define RENDERMAPCHIPNUM_HEIGHT 25
#define RENDERMAPCHIPNUM_WIDTH 44

//マップチップのサイズです
#define TIPSIZE 32 

enum MapKind { NOTHING, FLOOR, START=13};

int* GetMapchipData();
void MapRender();

#endif
