#ifndef GAMECLEAR_H
#define GAMECLEAR_H

#include<d3dx9.h>
#include"SceneManagement.h"

#define GAMECLEARWIDTH 1280
#define GAMECLEARHEIGTH 3000
#define ROLLSPEED 5

struct GameClearObj {
	//‚±‚Ì“_‚ğ¶‰º‚Æ‚µ‚Ä‰æ‘œ‚ª“WŠJ‚³‚ê‚é
	D3DXVECTOR2 Pos;
	int frcnt;
};

SCENE_ID RunGameClearScene(bool willbetrancefar);

#endif
