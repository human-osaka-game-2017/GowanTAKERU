#include<d3dx9.h>
#include<d3d9.h>
#include <windows.h>
#include<dinput.h>


#ifndef CONTROL_H
#define CONTROL_H

enum KEYSTATE {
	KEY_PUSH,
	KEY_RELEASE,
	KEY_ON,
	KEY_OFF
};

enum KEYKIND
{
	KEY_LEFT,
	KEY_RIGHT,
	KEY_UP,
	KEY_DOWN,
	KEY_A,
	KEY_B,
	KEY_C,
	KEY_D,
	KEY_E,
	KEY_F,
	KEY_G,
	KEY_H,
	KEY_I,
	KEY_J,
	KEY_K,
	KEY_L,
	KEY_M,
	KEY_N,
	KEY_O,
	KEY_P,
	KEY_Q,
	KEY_R,
	KEY_S,
	KEY_T,
	KEY_U,
	KEY_V,
	KEY_W,
	KEY_X,
	KEY_Y,
	KEY_Z,
	KEY_SPACE,
	KEY_ESC,
	KEYMAX
};

struct Ammo {
	float cx, cy;
	float radius;
	float rad;
	float dmg;
	//bool wasHit;
	bool wasReflect;
	int reflect_max;
	int reflect_cnt;

};

extern LPDIRECTINPUTDEVICE8 g_pKeyDevice;

void Control();
void KeyCheck(KEYSTATE* Key, int DIK);
void DXInputKeybourdInit(HWND hWnd, HINSTANCE hInstance);
bool Circle_Hit(float cx1, float cy1, float r1, float cx2, float cy2, float r2);
void FreeDxInput();

//struct 
//{
//	float x, y, height, width, scale, rad;
//};

#endif
