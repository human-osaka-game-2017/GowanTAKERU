#include"Render.h"
#include"PlayerRender.h"
#include"Player.h"

void PlayerRender() {

	CUSTOMVERTEX Player[] = {
		{ g_player.cx- PlayerSizeW/2,g_player.cy- PlayerSizeH/2,0.5f,1.0f, 0xFFFFFFFF,0.0f,0.0f },
		{ g_player.cx + PlayerSizeW/2,g_player.cy - PlayerSizeH / 2,0.5f,1.0f,0xFFFFFFFF,1.0f,0.0f },
		{ g_player.cx + PlayerSizeW/2,g_player.cy + PlayerSizeH / 2,0.5f,1.0f,0xFFFFFFFF,1.0f,1.0f },
		{ g_player.cx - PlayerSizeW/2,g_player.cy + PlayerSizeH / 2,0.5f,1.0f,0xFFFFFFFF,0.0f,1.0f }
	};

	// テクスチャをステージに割り当てる
	g_pD3Device->SetTexture(0, g_pTexture[PLAYER_TEX]);
	// 描画
	g_pD3Device->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, Player, sizeof(CUSTOMVERTEX));

}

