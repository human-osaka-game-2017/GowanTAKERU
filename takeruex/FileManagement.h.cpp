#include"FileManagement.h"
#include"CommonRender.h"
#include"DirectXGraphics.h"
#include"MainRender.h"
#include<stdio.h>
#include"MapRender.h"

static LPDIRECT3DTEXTURE9 g_pTexture[TEXMAX];

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

}

void TextureFree() {
	for (int i = 0; i <= TEXMAX; i++)
	{
		if (g_pTexture[i] != NULL) {
			g_pTexture[i]->Release();
			g_pTexture[i] = NULL;
		}
	}
}

void CSVLoad(char* mapdata, int* map,int height,int width) {

	FILE* pFile;

	fopen_s(&pFile, mapdata, "r");

	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			//ポインタずらしてます
			fscanf_s(pFile, "%d,", map + (j + i*MAPCHIPNUM_WIDTH));
		}
	}
	fclose(pFile);

}

void MainSceneLoad() {

	IDirect3DDevice9* pD3Device = GetGraphicsDevice();

	D3DXCreateTextureFromFile(pD3Device, "picture/mainscene/floor01.png", &g_pTexture[MAP_TEX]);
	D3DXCreateTextureFromFile(pD3Device, "picture/mainscene/background.png", &g_pTexture[BACKGROUND_TEX]);
	//D3DXCreateTextureFromFile(pD3Device, "picture/mainscene/Bullet01.png", &g_pTexture[ENEMYBullet01_TEX]);
	D3DXCreateTextureFromFile(pD3Device, "picture/mainscene/player.png", &g_pTexture[PLAYER_TEX]);
	
	int* mapdata = GetMapchipData();
	CSVLoad("CSV/mainscene/map.csv", mapdata, MAPCHIPNUM_HEIGHT, MAPCHIPNUM_WIDTH);
}