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

void MainRender() {

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

	GO_NEXT_STAGE_STATE goNextStageState = GetGoNextStageWait();
	if(goNextStageState != RUNNING){

		bool startup = false;
		if (goNextStageState = STARTWAIT) {
			startup = true;
		}

		FontRender(100.0f + FONTWIDTH * 0, 300.0f + FONTHEIGHT * 0, FONT_S, startup);
		FontRender(100.0f + FONTWIDTH * 1, 300.0f + FONTHEIGHT * 0, FONT_T, startup);
		FontRender(100.0f + FONTWIDTH * 2, 300.0f + FONTHEIGHT * 0, FONT_A, startup);
		FontRender(100.0f + FONTWIDTH * 3, 300.0f + FONTHEIGHT * 0, FONT_G, startup);
		FontRender(100.0f + FONTWIDTH * 4, 300.0f + FONTHEIGHT * 0, FONT_E, startup);
		FontRender(100.0f + FONTWIDTH * 6, 300.0f + FONTHEIGHT * 0, FONT_C, startup);
		FontRender(100.0f + FONTWIDTH * 7, 300.0f + FONTHEIGHT * 0, FONT_L, startup);
		FontRender(100.0f + FONTWIDTH * 8, 300.0f + FONTHEIGHT * 0, FONT_E, startup);
		FontRender(100.0f + FONTWIDTH * 9, 300.0f + FONTHEIGHT * 0, FONT_A, startup);
		FontRender(100.0f + FONTWIDTH * 10, 300.0f + FONTHEIGHT * 0, FONT_R, startup);
		FontRender(100.0f + FONTWIDTH * 11, 300.0f + FONTHEIGHT * 0, FONT_SURPRISED, startup);

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