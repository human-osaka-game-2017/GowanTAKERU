#include"MapRender.h"
#include"DirectXGraphics.h"
#include"FileManagement.h"
#include"MainRender.h"
#include"MapControl.h"
#include"PlayerControl.h"

void MapRender() {

	D3DXVECTOR2* BasePoint = GetBasePoint();

	IDirect3DDevice9* pD3Device = GetGraphicsDevice();
	LPDIRECT3DTEXTURE9* pTexture = GetTexture();

	CUSTOMVERTEX mapvertex[] = {
		{ 0,0,0.5f,1.0f, 0xFFFFFFFF,0.0f,0.0f },
		{ TIPSIZE,0,0.5f,1.0f,0xFFFFFFFF,0.125f,0.0f },
		{ TIPSIZE,TIPSIZE,0.5f,1.0f,0xFFFFFFFF,0.125f,0.125f },
		{ 0,TIPSIZE,0.5f,1.0f,0xFFFFFFFF,0.0f,0.125f }
	};

	//basepointの初期位置
	D3DXVECTOR2 BasePoint0 = D3DXVECTOR2(DISPLAY_WIDTH / 2, DISPLAY_HEIGHT / 2);

	//初期位置からの差分
	float differenceX = BasePoint->x - BasePoint0.x;
	float differenceY = BasePoint->y - BasePoint0.y;

	//描画範囲の要素番号を指定
	MapNumXY RendArrayIndexLeftUp;
	MapNumXY RendArrayIndexRightDown;
	MapNumXY RendArrayIndex;

	MapchipNumberSpecify(&RendArrayIndex, BasePoint);

	RendArrayIndexLeftUp.NumX = RendArrayIndex.NumX - RENDERSTAGE1MAPCHIPNUM_WIDTH / 2;
	RendArrayIndexLeftUp.NumY = RendArrayIndex.NumY - RENDERSTAGE1MAPCHIPNUM_HEIGHT / 2;
	RendArrayIndexRightDown.NumX = RendArrayIndex.NumX + RENDERSTAGE1MAPCHIPNUM_WIDTH / 2;
	RendArrayIndexRightDown.NumY = RendArrayIndex.NumY + RENDERSTAGE1MAPCHIPNUM_HEIGHT / 2;

	//描画用vertex
	CUSTOMVERTEX drawMapVertex[4];

	STAGE_ID stage_ID = GetStage_ID();
	int MaxX = GetStageXYMAX(stage_ID, X);
	int MaxY = GetStageXYMAX(stage_ID, Y);
	int* map = GetMapData();

	for (int i = 0; i < MaxY; i++) {
		for (int j = 0; j < MaxX; j++) {

			//そのマップチップを描画するのかを判定
			if (RendArrayIndexLeftUp.NumY < i && i < RendArrayIndexRightDown.NumY) {
				if (RendArrayIndexLeftUp.NumX < j && j < RendArrayIndexRightDown.NumX) {

					//描画する場所を設定
					for (int k = 0; k < 4; k++) {
						drawMapVertex[k] = mapvertex[k];
						drawMapVertex[k].x += -differenceX + TIPSIZE*j;
						drawMapVertex[k].y += -differenceY + TIPSIZE*i;
					}


					switch (map[j + i*MaxX]) {

					case NOTHING:
						continue;

					case FLOOR:
						for (int k = 0; k < 4; k++) {
							drawMapVertex[k].tv += 0.125;
						}
						break;

					case NEEDLE:
						for (int k = 0; k < 4; k++) {
							drawMapVertex[k].tv += 0.25;
						}
						break;
					}
					// テクスチャをステージに割り当てる
					pD3Device->SetTexture(0, pTexture[MAP_TEX]);
					// 描画
					pD3Device->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, drawMapVertex, sizeof(CUSTOMVERTEX));
				}
			}
		}
	}
}