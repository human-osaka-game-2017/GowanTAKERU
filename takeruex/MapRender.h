#ifndef MAPRENDER_H
#define MAPRENDER_H

#include<d3dx9.h>

//------------------------重要-------------------------------------
//マップチップの００番目(左上)は０でお願いします
//------------------------------------------------------------------

//マップチップ全体の個数です
#define MAPCHIPNUM_HEIGHT 50
#define MAPCHIPNUM_WIDTH 70

//描画するマップチップの個数です
#define RENDERMAPCHIPNUM_HEIGHT 25
#define RENDERMAPCHIPNUM_WIDTH 44

//マップチップのサイズです
#define TIPSIZE 32 

enum MapKind { NOTHING, FLOOR, START};

int* GetMapchipData();
void MapRender();

#endif
