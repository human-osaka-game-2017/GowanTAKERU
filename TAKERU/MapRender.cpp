#include"MapRender.h"
#include"Render.h"

int g_map[MAP_HEIGHT][MAP_WIDTH];

void MapRender() {

	CUSTOMVERTEX map01[] = {
		{ 0.0f,0.0f,0.5f,1.0f, 0xFFFFFFFF,0.0f,0.0f },
		{ TIPSIZE,0.0f,0.5f,1.0f,0xFFFFFFFF,1.0f,0.0f },
		{ TIPSIZE,TIPSIZE,0.5f,1.0f,0xFFFFFFFF,1.0f,1.0f },
		{ 0.0f,TIPSIZE,0.5f,1.0f,0xFFFFFFFF,0.0f,1.0f }
	};

	for (int y = 0; y < MAP_HEIGHT; y++)
	{
		for (int x = 0; x < MAP_WIDTH; x++)
		{
			if (g_map[y][x] == 1) {
				CUSTOMVERTEX drawMapVertex[4];
				for (int i = 0; i < 4; i++)
				{
					drawMapVertex[i] = map01[i];
				}

				for (int i = 0; i < 4; i++)
				{
					drawMapVertex[i].x += (x * TIPSIZE);
					drawMapVertex[i].y += (y * TIPSIZE);
				}
				// テクスチャをステージに割り当てる
				g_pD3Device->SetTexture(0, g_pTexture[MAP_TEX]);
				// 描画
				g_pD3Device->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, drawMapVertex, sizeof(CUSTOMVERTEX));
			}
		}
	}
}