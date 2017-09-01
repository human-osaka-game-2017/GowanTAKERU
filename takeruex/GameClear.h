#ifndef GAMECLEAR_H
#define GAMECLEAR_H

#include<d3dx9.h>
#include"SceneManagement.h"

#define GAMECLEARWIDTH 1280
#define GAMECLEARHEIGTH 3000
#define ROLLSPEED 2

struct GameClearObj {
	//この点を左下として画像が展開される
	D3DXVECTOR2 Pos;
	int frcnt;
};

SCENE_ID RunGameClearScene(bool willbetrancefar);

#endif
