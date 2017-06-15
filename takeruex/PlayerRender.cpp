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
		{ PLAYERSIZEWIDHE / 2,-PLAYERSIZEHEIGHT / 2,0.5f,1.0f,0xFFFFFFFF,1.0f,0.0f },
		{ PLAYERSIZEWIDHE / 2,PLAYERSIZEHEIGHT / 2,0.5f,1.0f,0xFFFFFFFF,1.0f,1.0f },
		{ -PLAYERSIZEWIDHE / 2,PLAYERSIZEHEIGHT / 2,0.5f,1.0f,0xFFFFFFFF,0.0f,1.0f }
	};

	CUSTOMVERTEX DrawVertex[4];
	for (int i = 0; i < 4; i++) {
		DrawVertex[i] = Player[i];
		DrawVertex[i].x += player->WindowPos.x;
		DrawVertex[i].y += player->WindowPos.y;
	}

	// テクスチャをステージに割り当てる
	pD3Device->SetTexture(0, pTexture[PLAYER_TEX]);
	// 描画
	pD3Device->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, DrawVertex, sizeof(CUSTOMVERTEX));

}