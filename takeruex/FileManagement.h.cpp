#include"FileManagement.h"
#include"CommonRender.h"
#include"DirectXGraphics.h"
#include"MainRender.h"
#include<stdio.h>
#include"MapRender.h"
#include"DirectXSound.h"
#include"TitleRender.h"
#include"GameOverRender.h"

#define StageNum 5

static LPDIRECT3DTEXTURE9* g_pTexture;
static LPDIRECT3DTEXTURE9 g_pBlackOutTexture;
int* g_mapData = NULL;
STAGEXYMAX g_MapNumMax[StageNum * 2];

int* GetMapData() {
	return g_mapData;
}

LPDIRECT3DTEXTURE9* GetBlackOutTexture() {
	return &g_pBlackOutTexture;
}

LPDIRECT3DTEXTURE9* GetTexture() {
	return g_pTexture;
}

void TextureInit() {

	IDirect3DDevice9* pD3Device = GetGraphicsDevice();

	//描画方法の設定--------------------------------------------------------------------わからん
	pD3Device->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	pD3Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);  //SRCの設定
	pD3Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	pD3Device->SetRenderState(D3DRS_DIFFUSEMATERIALSOURCE, D3DMCS_COLOR1);

	pD3Device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	pD3Device->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);

	pD3Device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);

	pD3Device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	pD3Device->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);

	pD3Device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);

	D3DXCreateTextureFromFile(pD3Device, "Picture/BlackOut/blackout.png", &g_pBlackOutTexture);
}

void ReleaseTexture(int num) {
	for (int i = 0; i < num; i++)
	{
		if (g_pTexture[i] != NULL) {
			g_pTexture[i]->Release();
			g_pTexture[i] = NULL;
		}
	}
}

void FreeTexture() {
	free(g_pTexture);
}

void ReleaseBlackOutTexture() {
	g_pBlackOutTexture->Release();
	g_pBlackOutTexture = NULL;
}

void CSVLoad(char* mapdata, int* map,int height,int width) {

	FILE* pFile;

	fopen_s(&pFile, mapdata, "r");

	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			//ポインタずらしてます
			fscanf_s(pFile, "%d,", map + (j + i*width));
		}
	}
	fclose(pFile);

}

void TitleSceneLoad() {

	IDirect3DDevice9* pD3Device = GetGraphicsDevice();
	g_pTexture = (LPDIRECT3DTEXTURE9*)malloc(sizeof(LPDIRECT3DTEXTURE9)*TITLESCENE_TEXMAX);

	D3DXCreateTextureFromFile(pD3Device, "Picture/TitleScene/titlebackground.png", &g_pTexture[TITLE_TEX]);
	D3DXCreateTextureFromFile(pD3Device, "Picture/TitleScene/pushZkeylogo.png", &g_pTexture[TITLELOGO_TEX]);

	CreateBufferForWave("Sound/SE_Start.wav", SOUND03);
}

void MainSceneLoad(STAGE_ID stage_ID) {

	IDirect3DDevice9* pD3Device = GetGraphicsDevice();
	g_pTexture=(LPDIRECT3DTEXTURE9*)malloc(sizeof(LPDIRECT3DTEXTURE9)*MAINSCENE_TEXMAX);

	switch (stage_ID) {
	case STAGE_1:
		D3DXCreateTextureFromFile(pD3Device, "Picture/MainScene/MapTip.png", &g_pTexture[MAP_TEX]);
		D3DXCreateTextureFromFile(pD3Device, "Picture/MainScene/background.png", &g_pTexture[BACKGROUND_TEX]);
		D3DXCreateTextureFromFile(pD3Device, "Picture/MainScene/bullet0.png", &g_pTexture[BULLET01_TEX]);
		D3DXCreateTextureFromFile(pD3Device, "Picture/MainScene/bullet1.png", &g_pTexture[BULLET02_TEX]);
		D3DXCreateTextureFromFile(pD3Device, "Picture/MainScene/player.png", &g_pTexture[PLAYER_TEX]);
		D3DXCreateTextureFromFile(pD3Device, "Picture/MainScene/enemy1.png", &g_pTexture[ENEMY01_TEX]);
		D3DXCreateTextureFromFile(pD3Device, "Picture/MainScene/hp0.png", &g_pTexture[HPUI_TEX]);
		D3DXCreateTextureFromFile(pD3Device, "Picture/MainScene/hp1.png", &g_pTexture[HPUIIN_TEX]);
		D3DXCreateTextureFromFile(pD3Device, "Picture/MainScene/boss1.png", &g_pTexture[BOSS1_TEX]);
		
		int MaxX = GetStageXYMAX(stage_ID, X);
		int MaxY = GetStageXYMAX(stage_ID, Y);

		g_mapData = (int*)malloc(sizeof(int)*MaxX*MaxY);
		CSVLoad("CSV/mainscene/stage1_map.csv", g_mapData, MaxY, MaxX);

		CreateBufferForWave("Sound/SE_ShotBom1.wav", SOUND01);
		CreateBufferForWave("Sound/BGM_Stage.wav", SOUND02);
	}
}

void GameOverSceneLoad() {
	IDirect3DDevice9* pD3Device = GetGraphicsDevice();
	g_pTexture = (LPDIRECT3DTEXTURE9*)malloc(sizeof(LPDIRECT3DTEXTURE9)*GAMEOVER_TEXMAX);

	D3DXCreateTextureFromFile(pD3Device, "Picture/GameOverScene/gameoverbackground.png", &g_pTexture[GAMEOVERBACKGROUND_TEX]);
	D3DXCreateTextureFromFile(pD3Device, "Picture/GameOverScene/gameoverlogo.png", &g_pTexture[GAMEOVERLOGO_TEX]);
	D3DXCreateTextureFromFile(pD3Device, "Picture/GameOverScene/pushZkeylogo.png", &g_pTexture[GAMEOVERPUSH_Z_KEY_TEX]);

}

void FreeMapData() {
	free(g_mapData);
	g_mapData = NULL;
}

void StageMapNumMaxInit() {
	FILE* pFile;

	fopen_s(&pFile, "CSV/mainscene/stage_num.csv", "r");

	for (int i = 0; i < StageNum * 2; i++) {

		fscanf_s(pFile, "%d,", &g_MapNumMax[i]);
	}

	fclose(pFile);
}

STAGEXYMAX GetStageXYMAX(STAGE_ID stage_ID, X_OR_Y XOrY) {
	switch (stage_ID) {

	case STAGE_1:
		if (XOrY == X) {
			return g_MapNumMax[0];
		}
		else {
			return g_MapNumMax[1];
		}

	case STAGE_2:
		if (XOrY == X) {
			return g_MapNumMax[2];
		}
		else {
			return g_MapNumMax[3];
		}

	case STAGE_3:
		if (XOrY == X) {
			return g_MapNumMax[4];
		}
		else {
			return g_MapNumMax[5];
		}
	}
}