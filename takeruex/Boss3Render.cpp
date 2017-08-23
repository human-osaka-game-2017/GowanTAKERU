#include"Boss3Render.h"
#include"Boss3Control.h"
#include"FileManagement.h"
#include"DirectXGraphics.h"
#include"CommonRender.h"
#include"MainRender.h"

void Boss3Render() {
	IDirect3DDevice9* pD3Device = GetGraphicsDevice();
	LPDIRECT3DTEXTURE9* pTexture = GetTexture();

	Boss3Data* pBoss3 = GetBoss3Data();

	CUSTOMVERTEX Boss3[] = {
		{ -BOSS3PNGWIDTH / 2, -BOSS3PNGHEIGHT / 2, 0.5f,1.0f,0xFFFFFFFF,0.0f,BOSS3PNGHEIGHT / BOSSPNGSIZE },
		{ BOSS3PNGWIDTH / 2, -BOSS3PNGHEIGHT / 2, 0.5f,1.0f,0xFFFFFFFF,BOSS3PNGWIDTH / BOSSPNGSIZE,BOSS3PNGHEIGHT / BOSSPNGSIZE },
		{ BOSS3PNGWIDTH / 2,  BOSS3PNGHEIGHT / 2, 0.5f,1.0f,0xFFFFFFFF,BOSS3PNGWIDTH / BOSSPNGSIZE,BOSS3PNGHEIGHT / BOSSPNGSIZE*2 },
		{ -BOSS3PNGWIDTH / 2,  BOSS3PNGHEIGHT / 2, 0.5f,1.0f,0xFFFFFFFF,0.0f,BOSS3PNGHEIGHT / BOSSPNGSIZE*2 }
	};
	if (pBoss3->isActive && !pBoss3->isDead) {
		CUSTOMVERTEX drawVertex[4];
		for (int j = 0; j < 4; j++) {
			drawVertex[j] = Boss3[j];
			drawVertex[j].x += pBoss3->WindowPos.x;
			drawVertex[j].y += pBoss3->WindowPos.y;
		}

		// テクスチャをステージに割り当てる
		pD3Device->SetTexture(0, pTexture[BOSS_TEX]);
		// 描画
		pD3Device->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, drawVertex, sizeof(CUSTOMVERTEX));
	}
}
