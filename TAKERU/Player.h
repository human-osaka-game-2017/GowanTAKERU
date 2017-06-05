
#ifndef PLAYER_H
#define PLAYER_H

#define PlayerSizeW 50
#define PlayerSizeH 100

#define Gravity 2.0;
#define PlayerMoveSpeed 5;

struct Player {
	float cx, cy;
	float jump_v0;
	float hp;
	bool beshotUP;
	bool beshotDOWN;
	int shot_cnt;

};

extern struct Player g_player;

void PlayerInit();
void Player_control();

#endif
