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

		static float tu = 0.0f;/*(768.0f + BOSS4WIDTH)/ ENEMYPNGSIZE - 768.0f/ ENEMYPNGSIZE;*/
		static float width = 0.0f;
		static int frcnt = 0;
		DWORD color = 0xFFFFFFFF;
		bool render = true;
		switch (pBoss4->Boss4TeleportState) {
		case TELEPORT_FIRSTHALF:
			tu += ((768.0f + BOSS4WIDTH) / ENEMYPNGSIZE - 768.0f / ENEMYPNGSIZE) / (BOSS4TELEPORTFRM) / 2;
			width += (BOSS4WIDTH / (BOSS4TELEPORTFRM) / 2);
			if ((width) == (BOSS4WIDTH / 2)) {
				int a = 0;
			}
			drawVertex[0].x += width;
			drawVertex[1].x -= width;
			drawVertex[2].x -= width;
			drawVertex[3].x += width;
			drawVertex[0].tu += tu;
			drawVertex[1].tu -= tu;
			drawVertex[2].tu -= tu;
			drawVertex[3].tu += tu;
			break;

		case TELEPORT_HARF:
			render = false;
			break;

		case TELEPORT_SECONDHALF:
			tu -= ((768.0f + BOSS4WIDTH) / ENEMYPNGSIZE - 768.0f / ENEMYPNGSIZE) / (BOSS4TELEPORTFRM) / 2;
			width -= (BOSS4WIDTH / (BOSS4TELEPORTFRM) / 2);
			if ((width) == (BOSS4WIDTH / 2)) {
				int a = 0;
			}
			drawVertex[0].x += width;
			drawVertex[1].x -= width;
			drawVertex[2].x -= width;
			drawVertex[3].x += width;
			drawVertex[0].tu += tu;
			drawVertex[1].tu -= tu;
			drawVertex[2].tu -= tu;
			drawVertex[3].tu += tu;
			break;

		case TELEPORT_FIN:
			tu = 0.0f;
			width = 0.0f;
			break;

		case FADEOUT_FIRSTHALF:
			frcnt++;
			color = ((0xFF * ((int)BOSS4TELEPORTFRM - frcnt) / (int)BOSS4TELEPORTFRM) << 24) | 0x00FFFFFF;
			for (int i = 0; i < 4; i++) {
				drawVertex[i].color = color;
			}
			break;

		case FADEOUT_HARF:
			frcnt = 0;
			render = false;
			break;

		case FADEOUT_SECONDHALF:
			frcnt++;
			color = ((0xFF * (frcnt) / (int)BOSS4TELEPORTFRM) << 24) | 0x00FFFFFF;
			for (int i = 0; i < 4; i++) {
				drawVertex[i].color = color;
			}
			break;

		case FADEOUT_FIN:
			frcnt = 0;
		}

		if (pBoss4->isDead) {
			if (frcnt < BOSSNEXTSTAGEWAIT) {
				color = ((0xFF * (BOSSMAXFRCNT - frcnt) / BOSSMAXFRCNT) << 24) | 0x00FFFFFF;
			}
			if (frcnt < BOSSMAXFRCNT) {
				frcnt++;
			}
		}
		

		if (!pBoss4->isLeft) {
			TurnVertex_tu(drawVertex);
		}

		if (render) {
			// テクスチャをステージに割り当てる
			pD3Device->SetTexture(0, pTexture[ENEMY_TEX]);
			// 描画
			pD3Device->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, drawVertex, sizeof(CUSTOMVERTEX));
		}
	}
}