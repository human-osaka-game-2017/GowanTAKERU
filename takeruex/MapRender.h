#ifndef MAPRENDER_H
#define MAPRENDER_H

#include<d3dx9.h>

//描画するマップチップの個数です
#define RENDERSTAGE1MAPCHIPNUM_HEIGHT 25
#define RENDERSTAGE1MAPCHIPNUM_WIDTH 44

//マップチップのサイズです
#define TIPSIZE 32 

enum MapKind { NOTHING, FLOOR, NEEDLE };
enum GimmickKind{ START = 13 };

void MapRender();

#endif
