#include"MapControl.h"
#include"MapRender.h"

void MapchipNumberSpecify(MapNumXY* pMapNumXY,D3DXVECTOR2* pWorldPos) {
	pMapNumXY->NumX = (pWorldPos->x / TIPSIZE) + 1;
	pMapNumXY->NumY = (pWorldPos->y / TIPSIZE) + 1;
}

int MapKindSpecify(MapNumXY* pMapNumXY, Direction direction) {

	int* map = GetMapchipData();
	int tmp;

	//アクセス違反を防ぐ
	if (pMapNumXY->NumX < 0 || MAPCHIPNUM_WIDTH < pMapNumXY->NumX) {
		pMapNumXY->NumX = 0;
	}
	if (pMapNumXY->NumY < 0 || MAPCHIPNUM_HEIGHT < pMapNumXY->NumY) {
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