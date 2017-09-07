#ifndef MAINRENDER_H
#define MAINRENDER_H

#include"CommonRender.h"
#include"MainControl.h"

#define ENEMYPNGSIZE 1024.0f
#define BOSSMAXFRCNT 180
#define BOSSNEXTSTAGEWAIT 300

enum MAINSCENE_TEXTURE
{
	STAGE_1_BACKGROUND_TEX,
	STAGE_2_BACKGROUND_TEX,
	STAGE_3_BACKGROUND_TEX,
	STAGE_4_BACKGROUND_TEX,
	STAGE_5_BACKGROUND_TEX,
	PLAYER_TEX,
	BULLET_TEX,
	MAP_TEX,
	ENEMY_TEX,
	HPUI_TEX,
	HPUIIN_TEX,
	RETRYICON_TEX,
	FONT_TEX,
	MAINSCENE_TEXMAX
};

void MainRender(RENDER_STATE stageStartState);

//画像の左右を反転させます
//第一引数 反転させる４頂点の先頭アドレス
void TurnVertex_tu(CUSTOMVERTEX* vertex);

//画像を切り取ります
//第一引数 切り取ってほしいvertex[4]の先頭アドレス
//第二引数 そのオブジェクトの画像上のｘ座標
//第三引数 そのオブジェクトの画像上のｙ座標
//第四引数 そのオブジェクトの幅
//第五引数 そのオブジェクトの高さ
//第六引数 画像全体の幅
//第七引数 画像全体の高さ
void TrimingVertex(CUSTOMVERTEX* vertex, float leftTopTu, float leftTopTv, float width, float height, float pngWidth, float pngHeight);
#endif
