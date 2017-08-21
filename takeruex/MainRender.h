#ifndef MAINRENDER_H
#define MAINRENDER_H

#include"CommonRender.h"

enum MAINSCENE_TEXTURE
{
	BACKGROUND_TEX,
	PLAYER_TEX,
	BULLET_TEX,
	MAP_TEX,
	ENEMY01_TEX,
	BOSS1_TEX,
	HPUI_TEX,
	HPUIIN_TEX,
	MAINSCENE_TEXMAX
};

void MainRender();

//画像の左右を反転させます
//第一引数 反転させる４頂点の先頭アドレス
void TurnVertex_tu(CUSTOMVERTEX* vertex);

#endif
