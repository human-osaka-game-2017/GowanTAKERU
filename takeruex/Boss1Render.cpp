#include"Boss1Render.h"
#include"DirectXGraphics.h"
#include"CommonRender.h"
#include"Boss1Control.h"
#include"FileManagement.h"
#include"MainRender.h"

void Boss1Render() {
	IDirect3DDevice9* pD3Device = GetGraphicsDevice();
	LPDIRECT3DTEXTURE9* pTexture = GetTexture();

	Boss1Data* pBoss1 = GetBoss1Data();

	CUSTOMVERTEX boss1[] = {
		{ -BOSS1WIDTH / 2, -BOSS1HEIGHT / 2, 0.5f,1.0f,0xFFFFFFFF,0.0f,0.0f },
		{ BOSS1WIDTH / 2, -BOSS1HEIGHT / 2, 0.5f,1.0f,0xFFFFFFFF,BOSS1WIDTH / ENEMYPNGSIZE,0.0f },
		{ BOSS1WIDTH / 2,  BOSS1HEIGHT / 2, 0.5f,1.0f,0xFFFFFFFF,BOSS1WIDTH / ENEMYPNGSIZE,BOSS1HEIGHT / ENEMYPNGSIZE },
		{ -BOSS1WIDTH / 2,  BOSS1HEIGHT / 2, 0.5f,1.0f,0xFFFFFFFF,0.0f,BOSS1HEIGHT / ENEMYPNGSIZE }
	};
	if (pBoss1->isActive) {

		static int frcnt = 0;
		DWORD color = 0xFFFFFFFF;
		if (pBoss1->isDead) {
			if (frcnt < BOSSNEXTSTAGEWAIT) {
				color = ((0xFF * (BOSSMAXFRCNT - frcnt) / BOSSMAXFRCNT) << 24) | 0x00FFFFFF;
			}
			if (frcnt < BOSSMAXFRCNT) {
				frcnt++;
			}
		}
		else {
			frcnt = 0;
		}

		CUSTOMVERTEX drawVertex[4];
		for (int j = 0; j < 4; j++) {
			drawVertex[j] = boss1[j];
			drawVertex[j].x += pBoss1->WindowPos.x;
			drawVertex[j].y += pBoss1->WindowPos.y;
			drawVertex[j].color = color;
		}

		TrimingVertex(drawVertex, 0.0f, 512.0f, BOSS1WIDTH, BOSS1HEIGHT, ENEMYPNGSIZE, ENEMYPNGSIZE);

		if (!pBoss1->isLeft) {
			TurnVertex_tu(drawVertex);
		}

		// テクスチャをステージに割り当てる
		pD3Device->SetTexture(0, pTexture[ENEMY_TEX]);
		// 描画
		pD3Device->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, drawVertex, sizeof(CUSTOMVERTEX));
	}
}