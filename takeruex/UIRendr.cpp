#include"UIRendr.h"
#include"DirectXGraphics.h"
#include"MainRender.h"
#include "FileManagement.h"
#include"PlayerControl.h"

void UIRender()
{
	IDirect3DDevice9* pD3Device = GetGraphicsDevice();
	LPDIRECT3DTEXTURE9* pTexture = GetTexture();

	CUSTOMVERTEX UI[] = {
		{ 18.0f,20.0f,0.5f,1.0f, 0xFFFFFFFF,0.0f,0.0f },
		{ 46.0f,20.0f,0.5f,1.0f,0xFFFFFFFF,1.0f,0.0f },
		{ 46.0f,220.0f,0.5f,1.0f,0xFFFFFFFF,1.0f,1.0f },
		{ 18.0f,220.0f,0.5f,1.0f,0xFFFFFFFF,0.0f,1.0f }
	};
	CUSTOMVERTEX UI_in[] = {
		{ 20.0f,20.0f,0.5f,1.0f, 0xFFFFFFFF,0.0f,0.0f },
		{ 44.0f,20.0f,0.5f,1.0f,0xFFFFFFFF,1.0f,0.0f },
		{ 44.0f,220.0f,0.5f,1.0f,0xFFFFFFFF,1.0f,1.0f },
		{ 20.0f,220.0f,0.5f,1.0f,0xFFFFFFFF,0.0f,1.0f }
	};

	Player* player = GetplayerData();
	CUSTOMVERTEX UIdrawvertex[4];
	for (int i = 0; i < 4; i++) {
		UIdrawvertex[i] = UI_in[i];
	}
	int x = 200 - (player->Hp / 100) * 200;
	UIdrawvertex[0].y = UIdrawvertex[1].y += 200 - (player->Hp / 100.0f) * 200;
	UIdrawvertex[0].tv = UIdrawvertex[1].tv = 1 - player->Hp / 100.0f;

	// テクスチャをステージに割り当てる
	pD3Device->SetTexture(0, pTexture[HPUI_TEX]);
	// 描画
	pD3Device->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, UI, sizeof(CUSTOMVERTEX));
	// テクスチャをステージに割り当てる
	pD3Device->SetTexture(0, pTexture[HPUIIN_TEX]);
	// 描画
	pD3Device->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, UIdrawvertex, sizeof(CUSTOMVERTEX));


	CUSTOMVERTEX RetryIcon[] = {
		{ 50.0f,20.0f,0.5f,1.0f, 0xFFFFFFFF,0.0f,0.0f },
		{ 100.0f,20.0f,0.5f,1.0f,0xFFFFFFFF,1.0f,0.0f },
		{ 100.0f,70.0f,0.5f,1.0f,0xFFFFFFFF,1.0f,1.0f },
		{ 50.0f,70.0f,0.5f,1.0f,0xFFFFFFFF,0.0f,1.0f }
	};

	// テクスチャをステージに割り当てる
	pD3Device->SetTexture(0, pTexture[RETRYICON_TEX]);
	// 描画
	pD3Device->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, RetryIcon, sizeof(CUSTOMVERTEX));

	CUSTOMVERTEX Font[] = {
		{ 100.0f,20.0f,0.5f,1.0f, 0xFFFFFFFF,0.0f,0.0f },
		{ 140.0f,20.0f,0.5f,1.0f,0xFFFFFFFF,1.0f,0.0f },
		{ 140.0f,73.0f,0.5f,1.0f,0xFFFFFFFF,1.0f,1.0f },
		{ 100.0f,73.0f,0.5f,1.0f,0xFFFFFFFF,0.0f,1.0f }
	};

	CUSTOMVERTEX fontDrawVertex[4];
	for (int i = 0; i < 4; i++) {
		fontDrawVertex[i] = Font[i];
	}
	
	float trimPosX = 108.0f * 4;
	float trimPosY = 144.0f * 3;

	TrimingVertex(fontDrawVertex, trimPosX, trimPosY, 108.0f, 144.0f, 1024.0f, 1024.0f);

	// テクスチャをステージに割り当てる
	pD3Device->SetTexture(0, pTexture[FONT_TEX]);
	// 描画
	pD3Device->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, fontDrawVertex, sizeof(CUSTOMVERTEX));

	for (int i = 0; i < 4; i++) {
		fontDrawVertex[i].x += 40.0f;
	}

	if (player->LifeReduced == 1) {
		trimPosX = 108.0f * 0;
		trimPosY = 144.0f * 5;
	}
	else {
		trimPosX = 108.0f * (player->LifeReduced-2);
		trimPosY = 144.0f * 4;
	}

	TrimingVertex(fontDrawVertex, trimPosX, trimPosY, 108.0f, 144.0f, 1024.0f, 1024.0f);

	// 描画
	pD3Device->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, fontDrawVertex, sizeof(CUSTOMVERTEX));
}