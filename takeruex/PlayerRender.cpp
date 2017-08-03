#include"PlayerRender.h"
#include"PlayerControl.h"
#include"MainRender.h"
#include"FileManagement.h"
#include"DirectXGraphics.h"

void PlayerRender() {

	Player* player = GetplayerData();

	if (player->beActive) {
		IDirect3DDevice9* pD3Device = GetGraphicsDevice();
		LPDIRECT3DTEXTURE9* pTexture = GetTexture();

		CUSTOMVERTEX Player[] = {
			{ -PLAYERSIZEWIDHE / 2,-PLAYERSIZEHEIGHT / 2,0.5f,1.0f, 0xFFFFFFFF,0.0f,0.0f },
			{ PLAYERSIZEWIDHE / 2,-PLAYERSIZEHEIGHT / 2,0.5f,1.0f,0xFFFFFFFF,PLAYERSIZEWIDHE / 1024.0f - 1 / 1024.0f,0.0f },
			{ PLAYERSIZEWIDHE / 2,PLAYERSIZEHEIGHT / 2,0.5f,1.0f,0xFFFFFFFF,PLAYERSIZEWIDHE / 1024.0f - 1 / 1024.0f,PLAYERSIZEHEIGHT / 1024.0f },
			{ -PLAYERSIZEWIDHE / 2,PLAYERSIZEHEIGHT / 2,0.5f,1.0f,0xFFFFFFFF,0.0f,PLAYERSIZEHEIGHT / 1024.0f }
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

				DrawVertex[i].tv += PLAYERSIZEHEIGHT * 4 / 1024.0f;

				if (12 <= frcntPunch && frcntPunch < 24) {
					DrawVertex[i].tu += PLAYERSIZEWIDHE / 1024.0f;
				}
				if (24 <= frcntPunch && frcntPunch <= 36) {
					DrawVertex[i].tu += PLAYERSIZEWIDHE * 2 / 1024.0f;
				}
			}
			if (player->bePunchDOWN) {

				DrawVertex[i].tv += PLAYERSIZEHEIGHT * 3 / 1024.0f;

				if (12 <= frcntPunch&&frcntPunch < 24) {
					DrawVertex[i].tu += PLAYERSIZEWIDHE / 1024.0f;
				}
				if (24 <= frcntPunch && frcntPunch <= 36) {
					DrawVertex[i].tu += PLAYERSIZEWIDHE * 2 / 1024.0f;
				}
			}
		}

		if (player->beLeft) {
			TurnVertex_tu(DrawVertex);
		}

		static int frcntInvincible = 0;

		if (!player->beInvincible || (frcntInvincible % 18) < 9) {

			// テクスチャをステージに割り当てる
			pD3Device->SetTexture(0, pTexture[PLAYER_TEX]);
			// 描画
			pD3Device->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, DrawVertex, sizeof(CUSTOMVERTEX));
		}
		if (player->beInvincible) {
			frcntInvincible++;
		}
		else {
			frcntInvincible = 0;
		}
	}
}