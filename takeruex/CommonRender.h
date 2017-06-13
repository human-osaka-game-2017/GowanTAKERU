#ifndef COMMONRENDER_H
#define COMMONRENDER_H

#include<Windows.h>

#define DISPLAY_WIDTH 1280
#define DISPLAY_HEIGHT 720

struct CUSTOMVERTEX
{
	FLOAT	x, y, z, rhw;
	DWORD	color;
	FLOAT	tu, tv;
};

#endif
