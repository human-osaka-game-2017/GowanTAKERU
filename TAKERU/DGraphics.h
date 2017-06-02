#include<Windows.h>
#include<d3dx9.h>

#ifndef DGRAPHICS_H
#define DGRAPHICS_H

#define DISPLAY_WIDTH 1280
#define DISPLAY_HEIGHT 720

extern IDirect3DDevice9* g_pD3Device;

struct CUSTOMVERTEX
{
	FLOAT	x, y, z, rhw;
	DWORD	color;
	FLOAT	tu, tv;
};

enum TEXTURE
{
	BACKGROUND_TEX,
	MAP_TEX,
	PLAYER_TEX,
	ENEMY_TEX,
	PLAYERAMMO01_TEX,
	ENEMYAMMO01_TEX,
	TEXMAX
};

IDirect3DDevice9* GetDevice();

void DXGraphicsInit(HWND hWnd, bool Window_mode);

void FreeDxGraphics();

#endif