#include"PlayerRender.h"
#include"PlayerControl.h"
#include"MainRender.h"
#include"CommonRender.h"
#include"FileManagement.h"
#include"DirectXGraphics.h"

void PlayerRender() {

	IDirect3DDevice9* pD3Device = GetGraphicsDevice();
	LPDIRECT3DTEXTURE9* pTexture = GetTexture();

	Player* player = GetplayerData();

	CUSTOMVERTEX Player[] = {
		{ -PLAYERSIZEWIDHE / 2,-PLAYERSIZEHEIGHT / 2,0.5f,1.0f, 0xFFFFFFFF,0.0f,0.0f },
		{ PLAYERSIZEWIDHE / 2,-PLAYERSIZEHEIGHT / 2,0.5f,1.0f,0xFFFFFFFF,0.5f,0.0f },
		{ PLAYERSIZEWIDHE / 2,PLAYERSIZEHEIGHT / 2,0.5f,1.0f,0xFFFFFFFF,0.5f,0.5f },
		{ -PLAYERSIZEWIDHE / 2,PLAYERSIZEHEIGHT / 2,0.5f,1.0f,0xFFFFFFFF,0.0f,0.5f }
	};

	static int frcntPunch = 0;
	if (player->bePunchUP || player->bePunchDOWN) {
		frcntPunch++;
	}
	else {
		frcntPunch = 0;
	}

	CUSTOMVERTEX DrawVertex[4];
	for (int i = 0; i < 4; i++) {
		DrawVertex[i] = Player[i];
		DrawVertex[i].x += player->WindowPos.x;
		DrawVertex[i].y += player->WindowPos.y;
		if (player->bePunchUP) {
			if (frcntPunch < 12 && frcntPunch != 0) {
				DrawVertex[i].tu += 0.5f;
				DrawVertex[i].tv += 0.5f;
			}
			if (12 < frcntPunch&&frcntPunch < 24) {
				DrawVertex[i].tv += 0.5f;
			}
			if (24 < frcntPunch && frcntPunch < 36) {
				DrawVertex[i].tu += 0.5f;
			}
		}
		if (player->bePunchDOWN) {
			if (frcntPunch < 12 && frcntPunch != 0) {
				DrawVertex[i].tu += 0.5f;
			}
			if (12 < frcntPunch&&frcntPunch < 24) {
				DrawVertex[i].tv += 0.5f;
			}
			if (24 < frcntPunch && frcntPunch < 36) {
				DrawVertex[i].tu += 0.5f;
				DrawVertex[i].tv += 0.5f;
			}
		}
	}

	if (player->beLeft) {
		TurnVertex_tu(DrawVertex);
	}

	// テクスチャをステージに割り当てる
	pD3Device->SetTexture(0, pTexture[PLAYER_TEX]);
	// 描画
	pD3Device->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, DrawVertex, sizeof(CUSTOMVERTEX));

}