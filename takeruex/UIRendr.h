#ifndef UIRENDER_H
#define UIRENDER_H

#define FONTWIDTH 86.4f
#define FONTHEIGHT 115.2f

enum FONT {
	FONT_A,
	FONT_B,
	FONT_C,
	FONT_D,
	FONT_E,
	FONT_F,
	FONT_G,
	FONT_H,
	FONT_I,
	FONT_J,
	FONT_K,
	FONT_L,
	FONT_M,
	FONT_N,
	FONT_O,
	FONT_P,
	FONT_Q,
	FONT_R,
	FONT_S,
	FONT_T,
	FONT_U,
	FONT_V,
	FONT_W,
	FONT_X,
	FONT_Y,
	FONT_Z,
	FONT_SURPRISED,
	FONT_QUESTION,
	FONT_MULTIPLY,
	FONT_COLON,
	FONT_0,
	FONT_1,
	FONT_2,
	FONT_3,
	FONT_4,
	FONT_5,
	FONT_6,
	FONT_7,
	FONT_8,
	FONT_9,

};

void UIRender();

void FontRender(float x, float y, FONT font, bool startup);

#endif