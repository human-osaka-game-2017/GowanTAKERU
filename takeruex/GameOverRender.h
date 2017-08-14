#ifndef GAMEOVERRENDER_H
#define GAMEOVERRENDER_H

enum GAMEOVERTEX {
	GAMEOVERBACKGROUND_TEX,
	GAMEOVERLOGO_TEX,
	GAMEOVERPUSH_Z_KEY_TEX,
	GAMEOVER_TEXMAX
};

void GameOverBackGroundRender();

void GameOverLogoRender();

void GameOverPushZkeyRender(bool pushKey);

#endif
