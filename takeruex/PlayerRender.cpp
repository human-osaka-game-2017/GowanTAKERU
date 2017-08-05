#include"PlayerRender.h"
#include"PlayerControl.h"
#include"MainRender.h"
#include"FileManagement.h"
#include"DirectXGraphics.h"
#include"CommonControl.h"

//プロトタイプ宣言
int DecidePlayerAnimMotion(Player* player);

void PlayerRender() {

	Player* player = GetplayerData();

	if (player->beActive) {
		IDirect3DDevice9* pD3Device = GetGraphicsDevice();
		LPDIRECT3DTEXTURE9* pTexture = GetTexture();

		CUSTOMVERTEX Player[] = {
			{ -PLAYERSIZEWIDTH / 2,-PLAYERSIZEHEIGHT / 2,0.5f,1.0f, 0xFFFFFFFF,0.0f,0.0f },
			{ PLAYERSIZEWIDTH / 2,-PLAYERSIZEHEIGHT / 2,0.5f,1.0f,0xFFFFFFFF,PLAYERSIZEWIDTH / 1024.0f - 1 / 1024.0f,0.0f },
			{ PLAYERSIZEWIDTH / 2,PLAYERSIZEHEIGHT / 2,0.5f,1.0f,0xFFFFFFFF,PLAYERSIZEWIDTH / 1024.0f - 1 / 1024.0f,PLAYERSIZEHEIGHT / 1024.0f },
			{ -PLAYERSIZEWIDTH / 2,PLAYERSIZEHEIGHT / 2,0.5f,1.0f,0xFFFFFFFF,0.0f,PLAYERSIZEHEIGHT / 1024.0f }
		};

		CUSTOMVERTEX CurrentDrawVertex[4];
		for (int i = 0; i < 4; i++) {
			CurrentDrawVertex[i] = Player[i];
			CurrentDrawVertex[i].x += player->WindowPos.x;
			CurrentDrawVertex[i].y += player->WindowPos.y;

			int trimX = player->currentAnimState % 10;
			int trimY = player->currentAnimState / 10;;

			//runswing中の例外処理（画像の場所によるもの）
			if ((RUNDOWNSWING1 <= player->currentAnimState) && (player->currentAnimState <= RUNUPSWING6)) {
				CurrentDrawVertex[i].tu += PLAYERSIZEWIDTH * trimY / 1024.f;
				CurrentDrawVertex[i].tv += PLAYERSIZEHEIGHT * trimX / 1024.f;
			}
			//そうでないなら
			else {

				CurrentDrawVertex[i].tu += PLAYERSIZEWIDTH * trimX / 1024.f;
				CurrentDrawVertex[i].tv += PLAYERSIZEHEIGHT * trimY / 1024.f;

			}

			
		}

		if (player->beLeft) {
			TurnVertex_tu(CurrentDrawVertex);
		}

		static int frcntInvincible = 0;
		if (player->beInvincible) {
			frcntInvincible++;
		}
		else {
			frcntInvincible = 0;
		}
		if (!player->beInvincible || (frcntInvincible % 18) < 9) {
			// テクスチャをステージに割り当てる
			pD3Device->SetTexture(0, pTexture[PLAYER_TEX]);
			// 描画
			pD3Device->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, CurrentDrawVertex, sizeof(CUSTOMVERTEX));
		}
	}
}

