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
		{ -BOSS3OBJWIDTH / 2, -BOSS3OBJHEIGHT / 2, 0.5f,1.0f,0xFFFFFFFF,0.0f,BOSS3OBJHEIGHT / ENEMYPNGSIZE },
		{ BOSS3OBJWIDTH / 2, -BOSS3OBJHEIGHT / 2, 0.5f,1.0f,0xFFFFFFFF,BOSS3OBJWIDTH / ENEMYPNGSIZE,BOSS3OBJHEIGHT / ENEMYPNGSIZE },
		{ BOSS3OBJWIDTH / 2,  BOSS3OBJHEIGHT / 2, 0.5f,1.0f,0xFFFFFFFF,BOSS3OBJWIDTH / ENEMYPNGSIZE,BOSS3OBJHEIGHT / ENEMYPNGSIZE},
		{ -BOSS3OBJWIDTH / 2,  BOSS3OBJHEIGHT / 2, 0.5f,1.0f,0xFFFFFFFF,0.0f,BOSS3OBJHEIGHT / ENEMYPNGSIZE }
	};
	if (pBoss3->isActive) {

		static int frcnt = 0;
		DWORD color = 0xFFFFFFFF;
		if (pBoss3->isDead) {
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

		CUSTOMVERTEX drawBoss3Vertex[4];
		CUSTOMVERTEX drawShieldVertex[4];
		for (int j = 0; j < 4; j++) {
			drawBoss3Vertex[j] = Boss3[j];
			drawShieldVertex[j] = Boss3[j];
			drawBoss3Vertex[j].x += pBoss3->WindowPos.x;
			drawShieldVertex[j].x += pBoss3->WindowPos.x;
			drawBoss3Vertex[j].y += pBoss3->WindowPos.y;
			drawShieldVertex[j].y += pBoss3->WindowPos.y;
			drawBoss3Vertex[j].color = color;
			drawShieldVertex[j].color = color;
		}

		TrimingVertex(drawBoss3Vertex,512.0f,512.0f, BOSS3OBJWIDTH, BOSS3OBJHEIGHT, ENEMYPNGSIZE, ENEMYPNGSIZE);
		TrimingVertex(drawShieldVertex, 512.0f, 768.0f, BOSS3OBJWIDTH, BOSS3OBJHEIGHT, ENEMYPNGSIZE, ENEMYPNGSIZE);

		if (!pBoss3->isLeft) {
			TurnVertex_tu(drawBoss3Vertex);
			TurnVertex_tu(drawShieldVertex);
		}

		// テクスチャをステージに割り当てる
		pD3Device->SetTexture(0, pTexture[ENEMY_TEX]);
		// 描画
		pD3Device->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, drawBoss3Vertex, sizeof(CUSTOMVERTEX));
		// 描画
		pD3Device->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, drawShieldVertex, sizeof(CUSTOMVERTEX));
	}
}
