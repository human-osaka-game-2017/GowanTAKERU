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
LPDIRECT3DTEXTURE9* GetBlackOutTexture();

//textureをリリース
void ReleaseTexture(int num);
void ReleaseBlackOutTexture();
void FreeTexture();

int* GetMapData();

//mainsceneのテキスチャとマップの読み込み
//Releseを行ってください
void MainSceneLoad(STAGE_ID stage_ID);
void FreeMapData();

//タイトルシーンの読み込み
void TitleSceneLoad();

//ゲームオーバーシーンの読み込み
void GameOverSceneLoad();

//ゲームクリアシーンの読み込み
void GameClearSceneLoad();

void CSVLoad(char* mapdata, int* map, int height, int width);

void StageMapNumMaxInit();

STAGEXYMAX GetStageXYMAX(STAGE_ID stage_ID, X_OR_Y XOrY);

#endif
