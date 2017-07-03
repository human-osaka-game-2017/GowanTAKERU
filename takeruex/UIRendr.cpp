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
	CUSTOMVERTEX drawvertex[4];
	for (int i = 0; i < 4; i++) {
		drawvertex[i] = UI_in[i];
	}
	int x = 200 - (player->Hp / 100) * 200;
	drawvertex[0].y = drawvertex[1].y += 200-(player->Hp/100.0f)*200;
	drawvertex[0].tv = drawvertex[1].tv = 1-player->Hp / 100.0f;

		// テクスチャをステージに割り当てる
		pD3Device->SetTexture(0, pTexture[HPUI_TEX]);
		// 描画
		pD3Device->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, UI, sizeof(CUSTOMVERTEX));
		// テクスチャをステージに割り当てる
		pD3Device->SetTexture(0, pTexture[HPUIIN_TEX]);
		// 描画
		pD3Device->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, drawvertex, sizeof(CUSTOMVERTEX));
}