#include"Boss4Render.h"
#include"Boss4Control.h"
#include"DirectXGraphics.h"
#include"CommonRender.h"
#include"FileManagement.h"
#include"MainRender.h"

void Boss4Render() {
	IDirect3DDevice9* pD3Device = GetGraphicsDevice();
	LPDIRECT3DTEXTURE9* pTexture = GetTexture();

	Boss4Data* pBoss4 = GetBoss4Data();

	CUSTOMVERTEX boss4[] = {
		{ -BOSS4WIDTH / 2, -BOSS4HEIGHT / 2, 0.5f,1.0f,0xFFFFFFFF,0.0f,0.0f },
		{ BOSS4WIDTH / 2, -BOSS4HEIGHT / 2, 0.5f,1.0f,0xFFFFFFFF,BOSS4WIDTH / ENEMYPNGSIZE,0.0f },
		{ BOSS4WIDTH / 2,  BOSS4HEIGHT / 2, 0.5f,1.0f,0xFFFFFFFF,BOSS4WIDTH / ENEMYPNGSIZE,BOSS4HEIGHT / ENEMYPNGSIZE },
		{ -BOSS4WIDTH / 2,  BOSS4HEIGHT / 2, 0.5f,1.0f,0xFFFFFFFF,0.0f,BOSS4HEIGHT / ENEMYPNGSIZE }
	};
	if (pBoss4->isActive && !pBoss4->isDead) {
		CUSTOMVERTEX drawVertex[4];
		for (int j = 0; j < 4; j++) {
			drawVertex[j] = boss4[j];
			drawVertex[j].x += pBoss4->WindowPos.x;
			drawVertex[j].y += pBoss4->WindowPos.y;
		}

		TrimingVertex(drawVertex, 768.0f, 512.0f, BOSS4WIDTH, BOSS4HEIGHT, ENEMYPNGSIZE, ENEMYPNGSIZE);

		if (!pBoss4->isLeft) {
			TurnVertex_tu(drawVertex);
		}

		// テクスチャをステージに割り当てる
		pD3Device->SetTexture(0, pTexture[ENEMY_TEX]);
		// 描画
		pD3Device->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, drawVertex, sizeof(CUSTOMVERTEX));
	}
}