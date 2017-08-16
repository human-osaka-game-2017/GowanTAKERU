#ifndef MAPRENDER_H
#define MAPRENDER_H

#include<d3dx9.h>

//描画するマップチップの個数です
#define RENDERSTAGE1MAPCHIPNUM_HEIGHT 26
#define RENDERSTAGE1MAPCHIPNUM_WIDTH 44

//マップチップのサイズです
#define TIPSIZE 32 

enum MapKind {  
	NOTHING, 
	FLOOR,
	WALL,
	BLOCK,
	CEILING,//天井 
	UPNEEDLE,
	LEFTNEEDLE,
	RIGHTNEEDLE,
	NEEDLE2,
	TRANSPARENTBLOCK,
	BOSS_SHUTTER, 
	SHUTTER_1, 
	SHUTTER_2, 
	SHUTTER_3, 
	SHUTTER_A, 
	SHUTTER_B, 
	SHUTTER_C,
	SWITCH_1,
	SWITCH_2,
	SWITCH_3,
	HOLE,
	WALKINGENEMY_1,//地上敵
	WALKINGENEMY_2,
	WALKINGENEMY_3,
	WALKINGENEMY_4,
	WALKINGENEMY_5,
	WALKINGENEMY_HAS_KEY_1,
	WALKINGENEMY_HAS_KEY_2,
	WALKINGENEMY_HAS_KEY_3,
	FixedBattery1,//固定砲台
	Dummy,//予備
	FLYINGENEMY1,//空中敵
	FLYINGENEMY2,
	FLYINGENEMY3,
	FLYINGENEMY4,
	FLYINGENEMY5,
	FLYINGENEMY_HAS_KEY1,
	FLYINGENEMY_HAS_KEY2,
	FLYINGENEMY_HAS_KEY3,
	FixedBattery2,
	StartLocation,//たけるスタート地点
	BOSS_Stage1,
	BOSS_Stage2,
	BOSS_Stage3,
	BOSS_Stage4,
	Dummy2,//予備隠し用
	Respawn_point1,//リスポーン地点
	Respawn_point2,
};
enum GimmickKind{ START = 40, CHECKPOINT_1=46, CHECKPOINT_2};

void MapRender();

#endif
