#include"MainRender.h"
#include"MainControl.h"
#include"DirectXGraphics.h"
#include"CommonRender.h"
#include"BackGroundRender.h"
#include"MapRender.h"
#include"PlayerRender.h"
#include"BulletRender.h"
#include"EnemyRender.h"
#include"BlackOutRender.h"
#include"Boss1Render.h"
#include"Boss2Render.h"
#include"Boss3Render.h"
#include"Boss4Render.h"
#include"UIRendr.h"
#include"PlayerControl.h"

void MainRender(RENDER_STATE stageStartState) {

	IDirect3DDevice9* pD3Device = GetGraphicsDevice();

	//-----------------------------------------------------
	//ƒV[ƒ“•`‰æ
	//-----------------------------------------------------
	pD3Device->Clear(0, NULL,
		D3DCLEAR_TARGET,
		D3DCOLOR_XRGB(0xff, 0xff, 0xff),
		1.0, 0);

	pD3Device->BeginScene();

	BackgroundRender();
	MapRender();
	EnemyRender();
	Boss1Render();
	Boss2Render();
	Boss3Render();
	Boss4Render();
	BulletRender();
	PlayerRender();

	if (stageStartState != WAITING) {
		bool startup = false;
		if (stageStartState = STARTUP) {
			startup = true;
		}

		float width = 40.0f;
		float height = 53.0f;
		Player* pPlayer = GetplayerData();
		float posY = pPlayer->WindowPos.y - 120.0f;
		FontRender(pPlayer->WindowPos.x - 10 + width * -1, posY + height * 0 - 10, width + 10, height + 10, 300, FONT_RETRYICON, startup);
		FontRender(pPlayer->WindowPos.x + width * 0, posY + height * 0, width - 10, height - 10,300, FONT_PLUS, startup);
		FontRender(pPlayer->WindowPos.x + width * 1, posY + height * 0, width - 10, height - 10,300, FONT_1, startup);
	}

	RENDER_STATE goNextStageState = GetGoNextStageWait();
	if(goNextStageState != WAITING){

		bool startup = false;
		if (goNextStageState = STARTUP) {
			startup = true;
		}

		FontRender(100.0f + 86.4f * 0, 220.0f + 115.2f * 0,86.4f, 115.2f,600, FONT_S, startup);
		FontRender(100.0f + 86.4f * 1, 220.0f + 115.2f * 0,86.4f, 115.2f,600, FONT_T, startup);
		FontRender(100.0f + 86.4f * 2, 220.0f + 115.2f * 0,86.4f, 115.2f,600, FONT_A, startup);
		FontRender(100.0f + 86.4f * 3, 220.0f + 115.2f * 0,86.4f, 115.2f,600, FONT_G, startup);
		FontRender(100.0f + 86.4f * 4, 220.0f + 115.2f * 0,86.4f, 115.2f,600, FONT_E, startup);
		FontRender(100.0f + 86.4f * 6, 220.0f + 115.2f * 0,86.4f, 115.2f,600, FONT_C, startup);
		FontRender(100.0f + 86.4f * 7, 220.0f + 115.2f * 0,86.4f, 115.2f,600, FONT_L, startup);
		FontRender(100.0f + 86.4f * 8, 220.0f + 115.2f * 0,86.4f, 115.2f,600, FONT_E, startup);
		FontRender(100.0f + 86.4f * 9, 220.0f + 115.2f * 0,86.4f, 115.2f,600, FONT_A, startup);
		FontRender(100.0f + 86.4f * 10, 220.0f + 115.2f * 0, 86.4f, 115.2f,600, FONT_R, startup);
		FontRender(100.0f + 86.4f * 11, 220.0f + 115.2f * 0, 86.4f, 115.2f,600, FONT_SURPRISED, startup);

	}

	UIRender();

	BlackOutData* blackOutData = GetBlackOutData();
	if (blackOutData->BlackOutflg) {
		BlackOutRender();
	}

	pD3Device->EndScene();
	pD3Device->Present(NULL, NULL, NULL, NULL);
}

void TurnVertex_tu(CUSTOMVERTEX* vertex) {

	CUSTOMVERTEX tmpVertex;

	tmpVertex.tu = vertex[0].tu;
	vertex[0].tu = vertex[1].tu;
	vertex[1].tu = tmpVertex.tu;

	tmpVertex.tu = vertex[3].tu;
	vertex[3].tu = vertex[2].tu;
	vertex[2].tu = tmpVertex.tu;

}

void TrimingVertex(CUSTOMVERTEX* vertex, float leftTopTu, float leftTopTv, float width, float height,float pngWidth,float pngHeight) {
	vertex[0].tu = leftTopTu / pngWidth;
	vertex[0].tv = leftTopTv / pngHeight;

	vertex[1].tu = (leftTopTu + width) / pngWidth;
	vertex[1].tv = leftTopTv / pngHeight;

	vertex[2].tu = (leftTopTu + width) / pngWidth;
	vertex[2].tv = (leftTopTv + height) / pngHeight;

	vertex[3].tu = leftTopTu / pngWidth;
	vertex[3].tv = (leftTopTv + height) / pngHeight;

}