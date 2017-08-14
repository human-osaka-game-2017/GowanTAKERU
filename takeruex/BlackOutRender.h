#ifndef BLACKOUTRENDER_H
#define BLACKOUTRENDER_H

//60フレームで完了するので30フレームで真っ暗になる
#define FINISHFRM 60

enum BLACKOUT_STATE {
	NON,
	FADE_IN,
	BLACKOUT,
	FADE_OUT,
	COMPLETION
};

struct BlackOutData {
	BLACKOUT_STATE BlackOutNextState;
	bool BlackOutflg;
};

//blackoutしている間はONです
//勝手にOFFにしてくれます
BlackOutData* GetBlackOutData();

void BlackOutRender();

#endif
