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
			fscanf_s(pFile, "%d,", (map + (j + i*width)));
		}
	}
	fclose(pFile);

}

void LoadEnemyDataCSV(EditableEnemyData pInEnemyData[]) {

	FILE* pFile;

	fopen_s(&pFile, "CSV/Mainscene/enemyData.csv", "r");

	for (int i = 0; i < ENEMY_KIND_MAX; i++) {
		int ID;
		fscanf_s(pFile, "%d,%d,%d,%f,%f,%f,%d,%f,%f,%f,%f", &ID, &pInEnemyData[i].Hp, &pInEnemyData[i].BulletKind, &pInEnemyData[i].BulletDeg, &pInEnemyData[i].ShotInterval, &pInEnemyData[i].Speed, &pInEnemyData[i].Atk,&pInEnemyData[i].tu, &pInEnemyData[i].tv, &pInEnemyData[i].Width, &pInEnemyData[i].Height);
	}
	fclose(pFile);
}

void LoadBulletDataCSV(EditableBulletData* pInBulletData) {

	FILE* pFile;

	fopen_s(&pFile, "CSV/Mainscene/BulletData.csv", "r");

	for (int i = 0; i < BULLET_MAX - 1; i++) {
		int ID;
		fscanf_s(pFile, "%d,%d,%d,%f,%f,%f,%f", &ID, &pInBulletData[i].Atk, &pInBulletData[i].ReflectMax, &pInBulletData[i] .Speed, &pInBulletData[i].Size, &pInBulletData[i].tu, &pInBulletData[i].tv);
	}

	pInBulletData[FIREWORKS].Atk = 10;
	pInBulletData[FIREWORKS].ReflectMax = 1;
	pInBulletData[FIREWORKS].Speed = 4.0f;
	pInBulletData[FIREWORKS].Size = 22.0f; 
	pInBulletData[FIREWORKS].tu = 0.0f;
	pInBulletData[FIREWORKS].tu = 22.0f;
	fclose(pFile);
}

void TitleSceneLoad() {

	IDirect3DDevice9* pD3Device = GetGraphicsDevice();
	g_pTexture = (LPDIRECT3DTEXTURE9*)malloc(sizeof(LPDIRECT3DTEXTURE9)*TITLESCENE_TEXMAX);

	D3DXCreateTextureFromFile(pD3Device, "Picture/TitleScene/titlebackground.png", &g_pTexture[TITLE_TEX]);
	D3DXCreateTextureFromFile(pD3Device, "Picture/TitleScene/pushZkeylogo.png", &g_pTexture[TITLELOGO_TEX]);

	CreateBufferForWave("Sound/BGM_title.wav", TITLESCENE_BGM01);
}

//void MainSceneSoundLoad(bool boss) {
//
//	STAGE_ID srage_ID = GetStage_ID();
//
//	switch (srage_ID) {
//	STAGE_1:
//		if (boss) {
//			CreateBufferForWave("Sound/BGM_boss1.wav", MAINSCENE_BGM01);
//		}
//		else {
//			CreateBufferForWave("Sound/BGM_stage1.wav", MAINSCENE_BGM01);
//		}
//		break;
//	STAGE_2:
//		if (boss) {
//			CreateBufferForWave("Sound/BGM_boss2.wav", MAINSCENE_BGM01);
//		}
//		else {
//			CreateBufferForWave("Sound/BGM_stage2.wav", MAINSCENE_BGM01);
//		}
//		break;
//	STAGE_3:
//		if (boss) {
//			CreateBufferForWave("Sound/BGM_boss1.wav", MAINSCENE_BGM01);
//		}
//		else {
//			CreateBufferForWave("Sound/BGM_stage3.wav", MAINSCENE_BGM01);
//		}
//		break;
//	STAGE_4:
//		if (boss) {
//			CreateBufferForWave("Sound/BGM_boss4.wav", MAINSCENE_BGM01);
//		}
//		else {
//			CreateBufferForWave("Sound/BGM_stage4.wav", MAINSCENE_BGM01);
//		}
//		break;
//	}
//}
void MainSceneLoad(STAGE_ID stage_ID) {

	IDirect3DDevice9* pD3Device = GetGraphicsDevice();
	g_pTexture=(LPDIRECT3DTEXTURE9*)malloc(sizeof(LPDIRECT3DTEXTURE9)*MAINSCENE_TEXMAX);

	D3DXCreateTextureFromFile(pD3Device, "Picture/MainScene/MapChip.png", &g_pTexture[MAP_TEX]);
	D3DXCreateTextureFromFile(pD3Device, "Picture/MainScene/stage1_background.png", &g_pTexture[STAGE_1_BACKGROUND_TEX]);
	D3DXCreateTextureFromFile(pD3Device, "Picture/MainScene/stage2_background.png", &g_pTexture[STAGE_2_BACKGROUND_TEX]);
	D3DXCreateTextureFromFile(pD3Device, "Picture/MainScene/stage3_background.png", &g_pTexture[STAGE_3_BACKGROUND_TEX]);
	D3DXCreateTextureFromFile(pD3Device, "Picture/MainScene/stage4_background.png", &g_pTexture[STAGE_4_BACKGROUND_TEX]);
	D3DXCreateTextureFromFile(pD3Device, "Picture/MainScene/stage5_background.png", &g_pTexture[STAGE_5_BACKGROUND_TEX]);
	D3DXCreateTextureFromFile(pD3Device, "Picture/MainScene/bullets.png", &g_pTexture[BULLET_TEX]);
	D3DXCreateTextureFromFile(pD3Device, "Picture/MainScene/player.png", &g_pTexture[PLAYER_TEX]);
	D3DXCreateTextureFromFile(pD3Device, "Picture/MainScene/enemys.png", &g_pTexture[ENEMY_TEX]);
	D3DXCreateTextureFromFile(pD3Device, "Picture/MainScene/hp0.png", &g_pTexture[HPUI_TEX]);
	D3DXCreateTextureFromFile(pD3Device, "Picture/MainScene/hp1.png", &g_pTexture[HPUIIN_TEX]);
	D3DXCreateTextureFromFile(pD3Device, "Picture/MainScene/retryicon.png", &g_pTexture[RETRYICON_TEX]);
	D3DXCreateTextureFromFile(pD3Device, "Picture/MainScene/fonts.png", &g_pTexture[FONT_TEX]);
	
	//SetBuffer(MAINSCENE_SOUNDMAX);

	CreateBufferForWave("Sound/SE_attack.wav", MAINSCENE_SE_ATTACK);
	CreateBufferForWave("Sound/SE_jump.wav", MAINSCENE_SE_JUMP);
	CreateBufferForWave("Sound/SE_swing.wav", MAINSCENE_SE_SWING);
	CreateBufferForWave("Sound/SE_PlayerDamaged.wav", MAINSCENE_SE_PLAYYERDAMAGE);
	CreateBufferForWave("Sound/SE_EnemyShot.wav", MAINSCENE_SE_ENEMYSHOT);
	CreateBufferForWave("Sound/SE_Explosion1.wav", MAINSCENE_SE_ENEMYDEAD);
	CreateBufferForWave("Sound/SE_EnemyDamage.wav", MAINSCENE_SE_ENEMYDAMAGE);
	CreateBufferForWave("Sound/SE_Barrier.wav", MAINSCENE_SE_BARRIER);
	CreateBufferForWave("Sound/SE_StageClear.wav", MAINSCENE_SE_FANFARE);
	CreateBufferForWave("Sound/SE_shutter1.wav", MAINSCENE_SE_SHUTTERSWITCH);
	CreateBufferForWave("Sound/SE_shutter2.wav", MAINSCENE_SE_SHUTTER);
	CreateBufferForWave("Sound/SE_Explosion2.wav", MAINSCENE_SE_BOSSDEAD);
	CreateBufferForWave("Sound/SE_warning.wav", MAINSCENE_SE_BOSSSPAWN1); 
	CreateBufferForWave("Sound/BGM_stage1.wav", MAINSCENE_STAGEBGM01);
	CreateBufferForWave("Sound/BGM_stage2.wav", MAINSCENE_STAGEBGM02);
	CreateBufferForWave("Sound/BGM_stage3.wav", MAINSCENE_STAGEBGM03);
	CreateBufferForWave("Sound/BGM_stage4.wav", MAINSCENE_STAGEBGM04);
	CreateBufferForWave("Sound/BGM_boss1.wav", MAINSCENE_BOSSBGM01);
	CreateBufferForWave("Sound/BGM_boss2.wav", MAINSCENE_BOSSBGM02);
	CreateBufferForWave("Sound/BGM_boss4.wav", MAINSCENE_BOSSBGM03);

	int MaxX = GetStageXYMAX(stage_ID, X);
	int MaxY = GetStageXYMAX(stage_ID, Y);

	g_mapData = (int*)malloc(sizeof(int)*MaxX*MaxY);

	switch (stage_ID) {
	case STAGE_1:
		CSVLoad("CSV/Mainscene/stage1_map.csv", g_mapData, MaxY, MaxX);
		break;

	case STAGE_2:
		CSVLoad("CSV/Mainscene/stage2_map.csv", g_mapData, MaxY, MaxX);
		break;

	case STAGE_3:
		CSVLoad("CSV/Mainscene/stage3_map.csv", g_mapData, MaxY, MaxX);
		break;

	case STAGE_4:
		CSVLoad("CSV/Mainscene/stage4_map.csv", g_mapData, MaxY, MaxX);
		break;

	case STAGE_5:
		CSVLoad("CSV/Mainscene/stage5_map.csv", g_mapData, MaxY, MaxX);
		break;
	}
}

void GameOverSceneLoad() {
	IDirect3DDevice9* pD3Device = GetGraphicsDevice();
	g_pTexture = (LPDIRECT3DTEXTURE9*)malloc(sizeof(LPDIRECT3DTEXTURE9)*GAMEOVER_TEXMAX);

	D3DXCreateTextureFromFile(pD3Device, "Picture/GameOverScene/gameoverbackground.png", &g_pTexture[GAMEOVERBACKGROUND_TEX]);
	D3DXCreateTextureFromFile(pD3Device, "Picture/GameOverScene/gameoverlogo.png", &g_pTexture[GAMEOVERLOGO_TEX]);
	D3DXCreateTextureFromFile(pD3Device, "Picture/GameOverScene/pushZkeylogo.png", &g_pTexture[GAMEOVERPUSH_Z_KEY_TEX]);

	CreateBufferForWave("Sound/BGM_gameover.wav", GAMEOVERSCENE_BGM01);

}

void GameClearSceneLoad() {
	IDirect3DDevice9* pD3Device = GetGraphicsDevice();
	g_pTexture = (LPDIRECT3DTEXTURE9*)malloc(sizeof(LPDIRECT3DTEXTURE9)*GAMECLEAR_TEXMAX);

	D3DXCreateTextureFromFile(pD3Device, "Picture/GameClearScene/StaffRoll.png", &g_pTexture[GAMECLEARROLL_TEX]);
	D3DXCreateTextureFromFile(pD3Device, "Picture/GameClearScene/pushZkeylogo.png", &g_pTexture[GAMECLEARPUSH_Z_KEY_TEX]);

	CreateBufferForWave("Sound/BGM_gameclear.wav", GAMECLEAR_BGM01);
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