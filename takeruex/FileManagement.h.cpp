#include"FileManagement.h"
#include"CommonRender.h"
#include"DirectXGraphics.h"
#include"MainRender.h"
#include<stdio.h>
#include"MapRender.h"
#include"DirectXSound.h"
#include"TitleRender.h"
#include"GameOverRender.h"
#include"GameClearRender.h"

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

	//CreateBufferForWave("Sound/BGM_title.wav", SOUND03);
}

void MainSceneSoundLoad(bool boss) {

	STAGE_ID srage_ID = GetStage_ID();

	switch (srage_ID) {
	STAGE_1:
		if (boss) {
			CreateBufferForWave("Sound/BGM_boss1.wav", MAINSCENE_BGM01);
		}
		else {
			CreateBufferForWave("Sound/BGM_stage1.wav", MAINSCENE_BGM01);
		}
		break;
	STAGE_2:
		if (boss) {
			CreateBufferForWave("Sound/BGM_boss2.wav", MAINSCENE_BGM01);
		}
		else {
			CreateBufferForWave("Sound/BGM_stage2.wav", MAINSCENE_BGM01);
		}
		break;
	STAGE_3:
		if (boss) {
			CreateBufferForWave("Sound/BGM_boss1.wav", MAINSCENE_BGM01);
		}
		else {
			CreateBufferForWave("Sound/BGM_stage3.wav", MAINSCENE_BGM01);
		}
		break;
	STAGE_4:
		if (boss) {
			CreateBufferForWave("Sound/BGM_boss4.wav", MAINSCENE_BGM01);
		}
		else {
			CreateBufferForWave("Sound/BGM_stage4.wav", MAINSCENE_BGM01);
		}
		break;
	}
}
void MainSceneLoad(STAGE_ID stage_ID) {

	IDirect3DDevice9* pD3Device = GetGraphicsDevice();
	g_pTexture=(LPDIRECT3DTEXTURE9*)malloc(sizeof(LPDIRECT3DTEXTURE9)*MAINSCENE_TEXMAX);

	D3DXCreateTextureFromFile(pD3Device, "Picture/MainScene/MapChip.png", &g_pTexture[MAP_TEX]);
	D3DXCreateTextureFromFile(pD3Device, "Picture/MainScene/background.png", &g_pTexture[BACKGROUND1_TEX]);
	D3DXCreateTextureFromFile(pD3Device, "Picture/MainScene/bullets.png", &g_pTexture[BULLET_TEX]);
	D3DXCreateTextureFromFile(pD3Device, "Picture/MainScene/player.png", &g_pTexture[PLAYER_TEX]);
	D3DXCreateTextureFromFile(pD3Device, "Picture/MainScene/enemys.png", &g_pTexture[ENEMY_TEX]);
	D3DXCreateTextureFromFile(pD3Device, "Picture/MainScene/hp0.png", &g_pTexture[HPUI_TEX]);
	D3DXCreateTextureFromFile(pD3Device, "Picture/MainScene/hp1.png", &g_pTexture[HPUIIN_TEX]);

	//SetBuffer(MAINSCENE_SOUNDMAX);

	//CreateBufferForWave("Sound/SE_attack.wav", MAINSCENE_SE_ATTACK);
	//CreateBufferForWave("Sound/SE_jump.wav", MAINSCENE_SE_JUMP);
	//CreateBufferForWave("Sound/SE_swing.wav", MAINSCENE_SE_SWING);

	int MaxX = GetStageXYMAX(stage_ID, X);
	int MaxY = GetStageXYMAX(stage_ID, Y);

	g_mapData = (int*)malloc(sizeof(int)*MaxX*MaxY);

	switch (stage_ID) {
	case STAGE_1:
		CSVLoad("CSV/mainscene/stage1_map.csv", g_mapData, MaxY, MaxX);
		break;

	case STAGE_2:
		CSVLoad("CSV/mainscene/stage2_map.csv", g_mapData, MaxY, MaxX);
		break;

	case STAGE_3:
		CSVLoad("CSV/mainscene/stage3_map.csv", g_mapData, MaxY, MaxX);
		break;

	case STAGE_4:
		CSVLoad("CSV/mainscene/stage4_map.csv", g_mapData, MaxY, MaxX);
		break;

	case STAGE_5:
		CSVLoad("CSV/mainscene/stage5_map.csv", g_mapData, MaxY, MaxX);
		break;
	}
}

void GameOverSceneLoad() {
	IDirect3DDevice9* pD3Device = GetGraphicsDevice();
	g_pTexture = (LPDIRECT3DTEXTURE9*)malloc(sizeof(LPDIRECT3DTEXTURE9)*GAMEOVER_TEXMAX);

	D3DXCreateTextureFromFile(pD3Device, "Picture/GameOverScene/gameoverbackground.png", &g_pTexture[GAMEOVERBACKGROUND_TEX]);
	D3DXCreateTextureFromFile(pD3Device, "Picture/GameOverScene/gameoverlogo.png", &g_pTexture[GAMEOVERLOGO_TEX]);
	D3DXCreateTextureFromFile(pD3Device, "Picture/GameOverScene/pushZkeylogo.png", &g_pTexture[GAMEOVERPUSH_Z_KEY_TEX]);

	//CreateBufferForWave("Sound/BGM_gameover.wav", SOUND02);

}

void GameClearSceneLoad() {
	IDirect3DDevice9* pD3Device = GetGraphicsDevice();
	g_pTexture = (LPDIRECT3DTEXTURE9*)malloc(sizeof(LPDIRECT3DTEXTURE9)*GAMECLEAR_TEXMAX);

	D3DXCreateTextureFromFile(pD3Device, "Picture/GameClearScene/StaffRoll.png", &g_pTexture[GAMECLEARROLL_TEX]);
	D3DXCreateTextureFromFile(pD3Device, "Picture/GameClearScene/pushZkeylogo.png", &g_pTexture[GAMECLEARPUSH_Z_KEY_TEX]);

	//CreateBufferForWave("Sound/BGM_gameclear.wav", SOUND02);
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

	case STAGE_4:
		if (XOrY == X) {
			return g_MapNumMax[6];
		}
		else {
			return g_MapNumMax[7];
		}
	case STAGE_5:
		if (XOrY == X) {
			return g_MapNumMax[8];
		}
		else {
			return g_MapNumMax[9];
		}
	}
}