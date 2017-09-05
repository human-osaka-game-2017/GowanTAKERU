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

#define FONTPNGWIDTH 108.0f
#define FONTPNGHEIGHT 144.0f
#define FADEFRM 600

void FontRender(float x, float y, FONT font, bool startup) {

	IDirect3DDevice9* pD3Device = GetGraphicsDevice();
	LPDIRECT3DTEXTURE9* pTexture = GetTexture();

	CUSTOMVERTEX Font[] = {
		{ 0.0f,0.0f,0.5f,1.0f, 0xFFFFFFFF,0.0f,0.0f },
		{ FONTWIDTH,0.0f,0.5f,1.0f,0xFFFFFFFF,1.0f,0.0f },
		{ FONTWIDTH,FONTHEIGHT,0.5f,1.0f,0xFFFFFFFF,1.0f,1.0f },
		{ 0.0f,FONTHEIGHT,0.5f,1.0f,0xFFFFFFFF,0.0f,1.0f }
	};

	static int frcnt = 0;
	if (startup) {
		frcnt = 0;
	}

	DWORD color = 0xFFFFFFFF;
	if (frcnt < FADEFRM) {
		color = ((0xFF * frcnt / FADEFRM) << 24) | 0x00FFFFFF;
	}

	CUSTOMVERTEX fontDrawVertex[4];
	for (int i = 0; i < 4; i++) {
		fontDrawVertex[i] = Font[i];
		fontDrawVertex[i].x += x;
		fontDrawVertex[i].y += y;
		fontDrawVertex[i].color = color;
	}
	frcnt++;

	switch (font) {
	case FONT_A:
		TrimingVertex(fontDrawVertex, FONTPNGWIDTH * 0, FONTPNGHEIGHT * 0, 108.0f, 144.0f, 1024.0f, 1024.0f);
		break;
	case FONT_B:
		TrimingVertex(fontDrawVertex, FONTPNGWIDTH * 1, FONTPNGHEIGHT * 0, 108.0f, 144.0f, 1024.0f, 1024.0f);
		break;
	case FONT_C:
		TrimingVertex(fontDrawVertex, FONTPNGWIDTH * 2, FONTPNGHEIGHT * 0, 108.0f, 144.0f, 1024.0f, 1024.0f);
		break;
	case FONT_D:
		TrimingVertex(fontDrawVertex, FONTPNGWIDTH * 3, FONTPNGHEIGHT * 0, 108.0f, 144.0f, 1024.0f, 1024.0f);
		break;
	case FONT_E:
		TrimingVertex(fontDrawVertex, FONTPNGWIDTH * 4, FONTPNGHEIGHT * 0, 108.0f, 144.0f, 1024.0f, 1024.0f);
		break;
	case FONT_F:
		TrimingVertex(fontDrawVertex, FONTPNGWIDTH * 5, FONTPNGHEIGHT * 0, 108.0f, 144.0f, 1024.0f, 1024.0f);
		break;
	case FONT_G:
		TrimingVertex(fontDrawVertex, FONTPNGWIDTH * 6, FONTPNGHEIGHT * 0, 108.0f, 144.0f, 1024.0f, 1024.0f);
		break;
	case FONT_H:
		TrimingVertex(fontDrawVertex, FONTPNGWIDTH * 7, FONTPNGHEIGHT * 0, 108.0f, 144.0f, 1024.0f, 1024.0f);
		break;
	case FONT_I:
		TrimingVertex(fontDrawVertex, FONTPNGWIDTH * 8, FONTPNGHEIGHT * 0, 108.0f, 144.0f, 1024.0f, 1024.0f);
		break;
	case FONT_J:
		TrimingVertex(fontDrawVertex, FONTPNGWIDTH * 0, FONTPNGHEIGHT * 1, 108.0f, 144.0f, 1024.0f, 1024.0f);
		break;
	case FONT_K:
		TrimingVertex(fontDrawVertex, FONTPNGWIDTH * 1, FONTPNGHEIGHT * 1, 108.0f, 144.0f, 1024.0f, 1024.0f);
		break;
	case FONT_L:
		TrimingVertex(fontDrawVertex, FONTPNGWIDTH * 2, FONTPNGHEIGHT * 1, 108.0f, 144.0f, 1024.0f, 1024.0f);
		break;
	case FONT_M:
		TrimingVertex(fontDrawVertex, FONTPNGWIDTH * 3, FONTPNGHEIGHT * 1, 108.0f, 144.0f, 1024.0f, 1024.0f);
		break;
	case FONT_N:
		TrimingVertex(fontDrawVertex, FONTPNGWIDTH * 4, FONTPNGHEIGHT * 1, 108.0f, 144.0f, 1024.0f, 1024.0f);
		break;
	case FONT_O:
		TrimingVertex(fontDrawVertex, FONTPNGWIDTH * 5, FONTPNGHEIGHT * 1, 108.0f, 144.0f, 1024.0f, 1024.0f);
		break;
	case FONT_P:
		TrimingVertex(fontDrawVertex, FONTPNGWIDTH * 6, FONTPNGHEIGHT * 1, 108.0f, 144.0f, 1024.0f, 1024.0f);
		break;
	case FONT_Q:
		TrimingVertex(fontDrawVertex, FONTPNGWIDTH * 7, FONTPNGHEIGHT * 1, 108.0f, 144.0f, 1024.0f, 1024.0f);
		break;
	case FONT_R:
		TrimingVertex(fontDrawVertex, FONTPNGWIDTH * 8, FONTPNGHEIGHT * 1, 108.0f, 144.0f, 1024.0f, 1024.0f);
		break;
	case FONT_S:
		TrimingVertex(fontDrawVertex, FONTPNGWIDTH * 0, FONTPNGHEIGHT * 2, 108.0f, 144.0f, 1024.0f, 1024.0f);
		break;
	case FONT_T:
		TrimingVertex(fontDrawVertex, FONTPNGWIDTH * 1, FONTPNGHEIGHT * 2, 108.0f, 144.0f, 1024.0f, 1024.0f);
		break;
	case FONT_U:
		TrimingVertex(fontDrawVertex, FONTPNGWIDTH * 2, FONTPNGHEIGHT * 2, 108.0f, 144.0f, 1024.0f, 1024.0f);
		break;
	case FONT_V:
		TrimingVertex(fontDrawVertex, FONTPNGWIDTH * 3, FONTPNGHEIGHT * 2, 108.0f, 144.0f, 1024.0f, 1024.0f);
		break;
	case FONT_W:
		TrimingVertex(fontDrawVertex, FONTPNGWIDTH * 4, FONTPNGHEIGHT * 2, 108.0f, 144.0f, 1024.0f, 1024.0f);
		break;
	case FONT_X:
		TrimingVertex(fontDrawVertex, FONTPNGWIDTH * 5, FONTPNGHEIGHT * 2, 108.0f, 144.0f, 1024.0f, 1024.0f);
		break;
	case FONT_Y:
		TrimingVertex(fontDrawVertex, FONTPNGWIDTH * 6, FONTPNGHEIGHT * 2, 108.0f, 144.0f, 1024.0f, 1024.0f);
		break;
	case FONT_Z:
		TrimingVertex(fontDrawVertex, FONTPNGWIDTH * 7, FONTPNGHEIGHT * 2, 108.0f, 144.0f, 1024.0f, 1024.0f);
		break;
	case FONT_SURPRISED:
		TrimingVertex(fontDrawVertex, FONTPNGWIDTH * 1, FONTPNGHEIGHT * 3, 108.0f, 144.0f, 1024.0f, 1024.0f);
		break;
	case FONT_QUESTION:
		TrimingVertex(fontDrawVertex, FONTPNGWIDTH * 2, FONTPNGHEIGHT * 3, 108.0f, 144.0f, 1024.0f, 1024.0f);
		break;
	case FONT_MULTIPLY:
		TrimingVertex(fontDrawVertex, FONTPNGWIDTH * 4, FONTPNGHEIGHT * 3, 108.0f, 144.0f, 1024.0f, 1024.0f);
		break;
	case FONT_COLON:
		TrimingVertex(fontDrawVertex, FONTPNGWIDTH * 5, FONTPNGHEIGHT * 3, 108.0f, 144.0f, 1024.0f, 1024.0f);
		break;
	case FONT_0:
		TrimingVertex(fontDrawVertex, FONTPNGWIDTH * 0, FONTPNGHEIGHT * 5, 108.0f, 144.0f, 1024.0f, 1024.0f);
		break;
	case FONT_1:
		TrimingVertex(fontDrawVertex , FONTPNGWIDTH * 0, FONTPNGHEIGHT * 4, 108.0f, 144.0f, 1024.0f, 1024.0f);
		break;
	case FONT_2:
		TrimingVertex(fontDrawVertex, FONTPNGWIDTH * 1, FONTPNGHEIGHT * 4, 108.0f, 144.0f, 1024.0f, 1024.0f);
		break;
	case FONT_3:
		TrimingVertex(fontDrawVertex, FONTPNGWIDTH * 2, FONTPNGHEIGHT * 4, 108.0f, 144.0f, 1024.0f, 1024.0f);
		break;
	case FONT_4:
		TrimingVertex(fontDrawVertex, FONTPNGWIDTH * 3, FONTPNGHEIGHT * 4, 108.0f, 144.0f, 1024.0f, 1024.0f);
		break;
	case FONT_5:
		TrimingVertex(fontDrawVertex, FONTPNGWIDTH * 4, FONTPNGHEIGHT * 4, 108.0f, 144.0f, 1024.0f, 1024.0f);
		break;
	case FONT_6:
		TrimingVertex(fontDrawVertex, FONTPNGWIDTH * 5, FONTPNGHEIGHT * 4, 108.0f, 144.0f, 1024.0f, 1024.0f);
		break;
	case FONT_7:
		TrimingVertex(fontDrawVertex, FONTPNGWIDTH * 6, FONTPNGHEIGHT * 4, 108.0f, 144.0f, 1024.0f, 1024.0f);
		break;
	case FONT_8:
		TrimingVertex(fontDrawVertex, FONTPNGWIDTH * 7, FONTPNGHEIGHT * 4, 108.0f, 144.0f, 1024.0f, 1024.0f);
		break;
	case FONT_9:
		TrimingVertex(fontDrawVertex, FONTPNGWIDTH * 8, FONTPNGHEIGHT * 4, 108.0f, 144.0f, 1024.0f, 1024.0f);
		break;
	}

	// テクスチャをステージに割り当てる
	pD3Device->SetTexture(0, pTexture[FONT_TEX]);
	// 描画
	pD3Device->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, fontDrawVertex, sizeof(CUSTOMVERTEX));
}