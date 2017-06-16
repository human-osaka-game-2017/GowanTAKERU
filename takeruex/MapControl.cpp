#include"MapControl.h"
#include"MapRender.h"
#include"PlayerControl.h"

void MapchipNumberSpecify(MapNumXY* pMapNumXY,D3DXVECTOR2* pWorldPos) {
	pMapNumXY->NumX = (pWorldPos->x / TIPSIZE);
	pMapNumXY->NumY = (pWorldPos->y / TIPSIZE);
}

void PosSpecifyForMapchipNumber(D3DXVECTOR2* pWorldPos, MapNumXY* pMapNumXY) {
	pWorldPos->x = pMapNumXY->NumX*TIPSIZE;
	pWorldPos->y = pMapNumXY->NumY*TIPSIZE;
}

int MapKindSpecify(MapNumXY* pMapNumXY) {
	int* map = GetMapchipData();

	//アクセス違反を防ぐ
	if (pMapNumXY->NumX < 0 || MAPCHIPNUM_WIDTH < pMapNumXY->NumX) {
		pMapNumXY->NumX = 0;
		pMapNumXY->NumY = 0;
	}
	if (pMapNumXY->NumY < 0 || MAPCHIPNUM_HEIGHT < pMapNumXY->NumY) {
		pMapNumXY->NumX = 0;
		pMapNumXY->NumY = 0;
	}

	int tmp = *(map + ((pMapNumXY->NumY)*MAPCHIPNUM_WIDTH + pMapNumXY->NumX));
	return tmp;
}

int MapKindSpecify_Plus1(MapNumXY* pMapNumXY, Direction direction) {

	int* map = GetMapchipData();
	int tmp;

	//アクセス違反を防ぐ
	if (pMapNumXY->NumX < 0 || MAPCHIPNUM_WIDTH < pMapNumXY->NumX) {
		pMapNumXY->NumX = 0;
		pMapNumXY->NumY = 0;
	}
	if (pMapNumXY->NumY < 0 || MAPCHIPNUM_HEIGHT < pMapNumXY->NumY) {
		pMapNumXY->NumX = 0;
		pMapNumXY->NumY = 0;
	}

	switch (direction) {

	case UP:
		tmp = *(map+(pMapNumXY->NumX + MAPCHIPNUM_HEIGHT*(pMapNumXY->NumY - 1)));

	case RIGHTUP:
		tmp = *(map + ((pMapNumXY->NumX + 1) + MAPCHIPNUM_HEIGHT*(pMapNumXY->NumY - 1)));

	case RIGHT:
		tmp = *(map + ((pMapNumXY->NumX + 1) + MAPCHIPNUM_HEIGHT*pMapNumXY->NumY));

	case RIGHTDOWN:
		tmp = *(map + ((pMapNumXY->NumX + 1) + MAPCHIPNUM_HEIGHT*(pMapNumXY->NumY + 1)));

	case DOWN:
		tmp = *(map + (pMapNumXY->NumX + MAPCHIPNUM_HEIGHT*(pMapNumXY->NumY + 1)));

	case LEFTDOWN:
		tmp = *(map + ((pMapNumXY->NumX - 1) + MAPCHIPNUM_HEIGHT*(pMapNumXY->NumY + 1)));

	case LEFT:
		tmp = *(map + ((pMapNumXY->NumX - 1) + MAPCHIPNUM_HEIGHT*pMapNumXY->NumY));

	case LEFTUP:
		tmp = *(map + ((pMapNumXY->NumX - 1) + MAPCHIPNUM_HEIGHT*(pMapNumXY->NumY - 1)));
	}

	return tmp;
}