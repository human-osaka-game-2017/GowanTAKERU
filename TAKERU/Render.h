#include<d3dx9.h>
#include<Windows.h>

#ifndef RENDER_H
#define RENDER_H

#define DISPLAY_WIDTH 1280
#define DISPLAY_HEIGHT 720

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

extern LPDIRECT3DTEXTURE9	  g_pTexture[TEXMAX];
extern IDirect3D9*			  g_pDirect3D;
extern IDirect3DDevice9*	  g_pD3Device;

void TextureInit();

void DXGraphicsInit(HWND hWnd, bool Window_mode);

void Render();

void FreeDxGraphics();

#endif
