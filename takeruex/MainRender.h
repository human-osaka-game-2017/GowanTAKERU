#ifndef MAINRENDER_H
#define MAINRENDER_H

#include"CommonRender.h"

enum MAIN_SCENE_STAGE1_TEXTURE
{
	BACKGROUND_TEX,
	BLACKOUT_TEX,
	GAMEOVER_TEX,
	PLAYER_TEX,
	BULLET01_TEX,
	BULLET02_TEX,
	MAP_TEX,
	ENEMY01_TEX,
	HPUI_TEX,
	HPUIIN_TEX,
	/*MAIN_SCENE_*/TEXMAX
};

void MainRender();

//画像の左右を反転させます
//第一引数 反転させる４頂点の先頭アドレス
void TurnVertex_tu(CUSTOMVERTEX* vertex);

#endif
