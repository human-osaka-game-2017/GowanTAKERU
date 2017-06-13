#include"MapControl.h"
#include"MapRender.h"

void MapchipNumberSpecify(MapNumXY* pMapNumXY,D3DXVECTOR2* pWorldPos) {
	pMapNumXY->NumX = pWorldPos->x / TIPSIZE;
	pMapNumXY->NumY = pWorldPos->y / TIPSIZE;
}

int MapKindSpecify(MapNumXY* pMapNumXY, Direction direction) {

	int* map = GetMapchipData();
	int tmp;

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