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


						switch (map[j + i*MaxX]) {

						case FLOOR:
							drawMapVertex[k].tv += 0.125 * 1;
							break;

						case WALL:
							drawMapVertex[k].tv += 0.125 * 1;
							drawMapVertex[k].tu += 0.125;
							break;

						case BLOCK:
							drawMapVertex[k].tv += 0.125 * 1;
							drawMapVertex[k].tu += 0.125 * 2;
							break;

						case CEILING:
							drawMapVertex[k].tv += 0.125 * 1;
							drawMapVertex[k].tu += 0.125 * 3;
							break;

						case FLOOR2:
							drawMapVertex[k].tv += 0.125 * 2;
							break;

						case WALL2:
							drawMapVertex[k].tv += 0.125 * 2;
							drawMapVertex[k].tu += 0.125;
							break;

						case BLOCK2:
							drawMapVertex[k].tv += 0.125 * 2;
							drawMapVertex[k].tu += 0.125 * 2;
							break;

						case CEILING2:
							drawMapVertex[k].tv += 0.125 * 2;
							drawMapVertex[k].tu += 0.125 * 3;
							break;

						case FLOOR3:
							drawMapVertex[k].tv += 0.125 * 3;
							break;

						case WALL3:
							drawMapVertex[k].tv += 0.125 * 3;
							drawMapVertex[k].tu += 0.125;
							break;

						case BLOCK3:
							drawMapVertex[k].tv += 0.125 * 3;
							drawMapVertex[k].tu += 0.125 * 2;
							break;

						case CEILING3:
							drawMapVertex[k].tv += 0.125 * 3;
							drawMapVertex[k].tu += 0.125 * 3;
							break;

						case FLOOR4:
							drawMapVertex[k].tv += 0.125 * 4;
							break;

						case WALL4:
							drawMapVertex[k].tv += 0.125 * 4;
							drawMapVertex[k].tu += 0.125;
							break;

						case BLOCK4:
							drawMapVertex[k].tv += 0.125 * 4;
							drawMapVertex[k].tu += 0.125 * 2;
							break;

						case CEILING4:
							drawMapVertex[k].tv += 0.125 * 4;
							drawMapVertex[k].tu += 0.125 * 3;
							break;

						case UPNEEDLE:
							break;

						case RIGHTNEEDLE:
							drawMapVertex[k].tu += 0.125 * 1;
							TurnVertex_tu(drawMapVertex);
							break;

						case LEFTNEEDLE:
							drawMapVertex[k].tu += 0.125 * 1;
							break;

						case SHUTTER_1:
						case SHUTTER_2:
						case SHUTTER_3:
						case SHUTTER_4:
						case SHUTTER_5:
						case SHUTTER_A:
						case SHUTTER_B:
						case SHUTTER_C:
							drawMapVertex[k].tu += 0.125 * 4;
							int a = 0;
							break;


						}
					}

					if (map[j + i*MaxX] != NOTHING) {
						// テクスチャをステージに割り当てる
						pD3Device->SetTexture(0, pTexture[MAP_TEX]);
						// 描画
						pD3Device->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, drawMapVertex, sizeof(CUSTOMVERTEX));
					}
				}
			}
		}
	}
}