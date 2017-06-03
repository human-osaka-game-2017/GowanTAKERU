#include<stdio.h>
#include"Render.h"
#include"MapRender.h"
#include"BackgroundRender.h"
#include"AmmoRender.h"
#include"PlayerRender.h"

LPDIRECT3DTEXTURE9	  g_pTexture[TEXMAX];

IDirect3D9*			  g_pDirect3D;
IDirect3DDevice9*	  g_pD3Device;

#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)

void CSVLoad(char* mapdata);

void DXGraphicsInit(HWND hWnd, bool Window_mode) {

	D3DPRESENT_PARAMETERS D3dPresentParameters;
	D3DDISPLAYMODE		  D3DdisplayMode;

	//DirectX オブジェクトの生成
	g_pDirect3D = Direct3DCreate9(
		D3D_SDK_VERSION);

	//Display Mode の設定
	g_pDirect3D->GetAdapterDisplayMode(
		D3DADAPTER_DEFAULT,
		&D3DdisplayMode);

	ZeroMemory(&D3dPresentParameters,
		sizeof(D3DPRESENT_PARAMETERS));
	D3dPresentParameters.BackBufferFormat = D3DdisplayMode.Format;
	D3dPresentParameters.BackBufferCount = 1;
	D3dPresentParameters.SwapEffect = D3DSWAPEFFECT_DISCARD;
	D3dPresentParameters.Windowed = TRUE;

	//デバイスを作る
	g_pDirect3D->CreateDevice(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING,
		&D3dPresentParameters, &g_pD3Device);

	g_pD3Device->SetFVF(D3DFVF_CUSTOMVERTEX);

	//---------------------------------------------------
	//viewport設定
	//-------------------------------------------------
	D3DVIEWPORT9 Viewport{ 0,0,D3dPresentParameters.BackBufferWidth,D3dPresentParameters.BackBufferHeight,0.0f,1.0f };
	g_pD3Device->SetViewport(&Viewport);

}

void TextureInit() {
	//描画方法の設定--------------------------------------------------------------------わからん
	g_pD3Device->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	g_pD3Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);  //SRCの設定
	g_pD3Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	g_pD3Device->SetRenderState(D3DRS_DIFFUSEMATERIALSOURCE, D3DMCS_COLOR1);

	g_pD3Device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	g_pD3Device->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);

	g_pD3Device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);

	//画像選択
	D3DXCreateTextureFromFile(g_pD3Device, "picture/floor01.png", &g_pTexture[MAP_TEX]);
	D3DXCreateTextureFromFile(g_pD3Device, "picture/background.png", &g_pTexture[BACKGROUND_TEX]);
	D3DXCreateTextureFromFile(g_pD3Device, "picture/ammo01.png", &g_pTexture[ENEMYAMMO01_TEX]);

	CSVLoad("map.csv");
}

void CSVLoad(char* mapdata) {

	FILE* pFile;

	fopen_s(&pFile, mapdata, "r");

	for (int i = 0; i < MAP_HEIGHT; i++)
	{
		for (int j = 0; j < MAP_WIDTH; j++)
		{
			fscanf_s(pFile, "%d,", &g_map[i][j]);
		}
	}
	fclose(pFile);

}



void Render() {
	//-----------------------------------------------------
	//シーン描画
	//-----------------------------------------------------
	g_pD3Device->Clear(0, NULL,
		D3DCLEAR_TARGET,
		D3DCOLOR_XRGB(0xff, 0xff, 0xff),
		1.0, 0);

	g_pD3Device->BeginScene();

	BackgroundRender();
	MapRender();
	AMMORender();
	
	g_pD3Device->EndScene();
	g_pD3Device->Present(NULL, NULL, NULL, NULL);
}


void TextureFree() {
	for (int i = 0; i <= TEXMAX; i++)
	{
		g_pTexture[i]->Release();
		g_pTexture[i] = NULL;
	}
}

void FreeDxGraphics()
{

	g_pD3Device->Release();

	g_pDirect3D->Release();
}

