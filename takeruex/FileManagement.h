#ifndef FILEMANAGEMENT_H
#define FILEMANAGEMENT_H

#include<d3dx9.h>
#include"StageSelect.h"

enum STAGEXYMAX {
	STAGE1_XMAX,
	STAGE1_YMAX,
	STAGE2_XMAX,
	STAGE2_YMAX,
	STAGE3_XMAX,
	STAGE3_YMAX
};

enum X_OR_Y {
	X, Y
};


//pTexture使いたい人つかって
LPDIRECT3DTEXTURE9* GetTexture();

//textureをリリース
void TextureFree();

int* GetMapData();

//mainsceneのテキスチャとマップの読み込み
//Releseを行ってください
void MainSceneLoad(STAGE_ID stage_ID);
void ReleseMapData();

void CSVLoad(char* mapdata, int* map, int height, int width);

void StageMapNumMaxInit();

STAGEXYMAX GetStageXYMAX(STAGE_ID stage_ID, X_OR_Y XOrY);

#endif
