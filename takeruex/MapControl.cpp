#include"MapControl.h"
#include"MapRender.h"
#include"PlayerControl.h"
#include"FileManagement.h"



void MapchipNumberSpecify(MapNumXY* pMapNumXY,D3DXVECTOR2* pWorldPos) {
	pMapNumXY->NumX = (pWorldPos->x / TIPSIZE);
	pMapNumXY->NumY = (pWorldPos->y / TIPSIZE);
}

void PosSpecifyForMapchipNumber(D3DXVECTOR2* pWorldPos, MapNumXY* pMapNumXY) {
	pWorldPos->x = pMapNumXY->NumX*TIPSIZE;
	pWorldPos->y = pMapNumXY->NumY*TIPSIZE;
}

int MapKindSpecify(MapNumXY* pMapNumXY) {
	STAGE_ID stage_ID = GetStage_ID();
	int MaxX = GetStageXYMAX(stage_ID, X);
	int MaxY = GetStageXYMAX(stage_ID, Y);
	int* map = GetMapData();

	//アクセス違反を防ぐ
	if (pMapNumXY->NumX < 0 || MaxX < pMapNumXY->NumX) {
		pMapNumXY->NumX = 0;
		pMapNumXY->NumY = 0;
	}
	if (pMapNumXY->NumY < 0 || MaxY < pMapNumXY->NumY) {
		pMapNumXY->NumX = 0;
		pMapNumXY->NumY = 0;
	}

	int tmp = *(map + ((pMapNumXY->NumY)*MaxX + pMapNumXY->NumX));
	return tmp;
}

int MapKindSpecify_Plus1(MapNumXY* pMapNumXY, Direction direction) {

	STAGE_ID stage_ID = GetStage_ID();
	int MaxX = GetStageXYMAX(stage_ID, X);
	int MaxY = GetStageXYMAX(stage_ID, Y);
	int* map = GetMapData();
	int tmp;

	//アクセス違反を防ぐ
	if (pMapNumXY->NumX < 0 || MaxX < pMapNumXY->NumX) {
		pMapNumXY->NumX = 0;
		pMapNumXY->NumY = 0;
	}
	if (pMapNumXY->NumY < 0 || MaxY < pMapNumXY->NumY) {
		pMapNumXY->NumX = 0;
		pMapNumXY->NumY = 0;
	}

	switch (direction) {

	case UP:
		tmp = *(map+(pMapNumXY->NumX + MaxX*(pMapNumXY->NumY - 1)));
		break;

	case RIGHTUP:
		tmp = *(map + ((pMapNumXY->NumX + 1) + MaxX*(pMapNumXY->NumY - 1)));
		break;

	case RIGHT:
		tmp = *(map + ((pMapNumXY->NumX + 1) + MaxX*pMapNumXY->NumY));
		break;

	case RIGHTDOWN:
		tmp = *(map + ((pMapNumXY->NumX + 1) + MaxX*(pMapNumXY->NumY + 1)));
		break;

	case DOWN:
		tmp = *(map + (pMapNumXY->NumX + MaxX*(pMapNumXY->NumY + 1)));
		break;

	case LEFTDOWN:
		tmp = *(map + ((pMapNumXY->NumX - 1) + MaxX*(pMapNumXY->NumY + 1)));
		break;

	case LEFT:
		tmp = *(map + ((pMapNumXY->NumX - 1) + MaxX*pMapNumXY->NumY));
		break;

	case LEFTUP:
		tmp = *(map + ((pMapNumXY->NumX - 1) + MaxX*(pMapNumXY->NumY - 1)));
		break;
	}

	return tmp;
}

int MapKindSpecifyForPos(D3DXVECTOR2* pWorldPos) {
	MapNumXY MapNumXY;
	MapNumXY.NumX = (pWorldPos->x / TIPSIZE);
	MapNumXY.NumY = (pWorldPos->y / TIPSIZE);
	return MapKindSpecify(&MapNumXY);
}

int CalculateNumInRange(float l) {
	return (int)l / TIPSIZE + 1 ;
}