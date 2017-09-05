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

					case FLOOR1:
						TrimingVertex(drawMapVertex, TIPSIZE * 0, TIPSIZE * 1, TIPSIZE, TIPSIZE, MAPPNGSIZE, MAPPNGSIZE);
						break;

					case WALL1:
						TrimingVertex(drawMapVertex, TIPSIZE * 1, TIPSIZE * 1, TIPSIZE, TIPSIZE, MAPPNGSIZE, MAPPNGSIZE);
						break;

					case BLOCK1:
						TrimingVertex(drawMapVertex, TIPSIZE * 2, TIPSIZE * 1, TIPSIZE, TIPSIZE, MAPPNGSIZE, MAPPNGSIZE);
						break;

					case CEILING1:
						TrimingVertex(drawMapVertex, TIPSIZE * 3, TIPSIZE * 1, TIPSIZE, TIPSIZE, MAPPNGSIZE, MAPPNGSIZE);
						break;

					case FLOOR2:
						TrimingVertex(drawMapVertex, TIPSIZE * 0, TIPSIZE * 2, TIPSIZE, TIPSIZE, MAPPNGSIZE, MAPPNGSIZE);
						break;

					case WALL2:
						TrimingVertex(drawMapVertex, TIPSIZE * 1, TIPSIZE * 2, TIPSIZE, TIPSIZE, MAPPNGSIZE, MAPPNGSIZE);
						break;

					case BLOCK2:
						TrimingVertex(drawMapVertex, TIPSIZE * 2, TIPSIZE * 2, TIPSIZE, TIPSIZE, MAPPNGSIZE, MAPPNGSIZE);
						break;

					case CEILING2:
						TrimingVertex(drawMapVertex, TIPSIZE * 3, TIPSIZE * 2, TIPSIZE, TIPSIZE, MAPPNGSIZE, MAPPNGSIZE);
						break;

					case FLOOR3:
						TrimingVertex(drawMapVertex, TIPSIZE * 0, TIPSIZE * 3, TIPSIZE, TIPSIZE, MAPPNGSIZE, MAPPNGSIZE);
						break;

					case WALL3:
						TrimingVertex(drawMapVertex, TIPSIZE * 1, TIPSIZE * 3, TIPSIZE, TIPSIZE, MAPPNGSIZE, MAPPNGSIZE);
						break;

					case BLOCK3:
						TrimingVertex(drawMapVertex, TIPSIZE * 2, TIPSIZE * 3, TIPSIZE, TIPSIZE, MAPPNGSIZE, MAPPNGSIZE);
						break;

					case CEILING3:
						TrimingVertex(drawMapVertex, TIPSIZE * 3, TIPSIZE * 3, TIPSIZE, TIPSIZE, MAPPNGSIZE, MAPPNGSIZE);
						break;

					case FLOOR4:
						TrimingVertex(drawMapVertex, TIPSIZE * 0, TIPSIZE * 4, TIPSIZE, TIPSIZE, MAPPNGSIZE, MAPPNGSIZE);
						break;

					case WALL4:
						TrimingVertex(drawMapVertex, TIPSIZE * 1, TIPSIZE * 4, TIPSIZE, TIPSIZE, MAPPNGSIZE, MAPPNGSIZE);
						break;

					case BLOCK4:
						TrimingVertex(drawMapVertex, TIPSIZE * 2, TIPSIZE * 4, TIPSIZE, TIPSIZE, MAPPNGSIZE, MAPPNGSIZE);
						break;

					case CEILING4:
						TrimingVertex(drawMapVertex, TIPSIZE * 3, TIPSIZE * 4, TIPSIZE, TIPSIZE, MAPPNGSIZE, MAPPNGSIZE);
						break;

					case UPNEEDLE:
						TrimingVertex(drawMapVertex, TIPSIZE * 0, TIPSIZE * 0, TIPSIZE, TIPSIZE, MAPPNGSIZE, MAPPNGSIZE);
						break;

					case LEFTNEEDLE:
						TrimingVertex(drawMapVertex, TIPSIZE * 1, TIPSIZE * 0, TIPSIZE, TIPSIZE, MAPPNGSIZE, MAPPNGSIZE);
						break;

					case RIGHTNEEDLE:
						TrimingVertex(drawMapVertex, TIPSIZE * 1, TIPSIZE * 0, TIPSIZE, TIPSIZE, MAPPNGSIZE, MAPPNGSIZE);
						TurnVertex_tu(drawMapVertex);
						break; 

					case DANGEROUS:
						TrimingVertex(drawMapVertex, TIPSIZE * 2, TIPSIZE * 0, TIPSIZE, TIPSIZE, MAPPNGSIZE, MAPPNGSIZE);
						break;

					case SHUTTER1:
					case SHUTTER2:
					case SHUTTER3:
					case SHUTTER4:
					case SHUTTER5:
					case SHUTTER6:
					case SHUTTER7:
					case SHUTTER8:
					case BOSS_SHUTTER:
						TrimingVertex(drawMapVertex, TIPSIZE * 4, TIPSIZE * 0, TIPSIZE, TIPSIZE, MAPPNGSIZE, MAPPNGSIZE);
						break;

					case SHUTTER_A:
					case SHUTTER_B:
					case SHUTTER_C:
						TrimingVertex(drawMapVertex, TIPSIZE * 4, TIPSIZE * 1, TIPSIZE, TIPSIZE, MAPPNGSIZE, MAPPNGSIZE);
						break;

					case TRANSPARENTBLOCK:
						TrimingVertex(drawMapVertex, TIPSIZE * 4, TIPSIZE * 2, TIPSIZE, TIPSIZE, MAPPNGSIZE, MAPPNGSIZE);
						break;
						
					default:
						TrimingVertex(drawMapVertex, TIPSIZE * 3, TIPSIZE * 0, TIPSIZE, TIPSIZE, MAPPNGSIZE, MAPPNGSIZE);
						break;
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