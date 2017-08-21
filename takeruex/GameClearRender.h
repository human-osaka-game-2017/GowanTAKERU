#ifndef GAMECLEARRENDER_H
#define GAMECLEARRENDER_H

#include"GameClear.h"

enum GAMECLEARTEX {
	GAMECLEARROLL_TEX,
	GAMECLEARPUSH_Z_KEY_TEX,
	GAMECLEAR_TEXMAX
};

void GameCleaRollRender(GameClearObj* pGameClear);

void GameClearPushZkeyRender(bool pushKey);

#endif
