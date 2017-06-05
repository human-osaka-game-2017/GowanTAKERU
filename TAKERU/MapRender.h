#ifndef MAPRENDER_H
#define MAPRENDER_H

#define MAP_HEIGHT 50
#define MAP_WIDTH 50
#define TIPSIZE 32

enum MapKind { nothing, yuka };

extern MapKind g_map[MAP_HEIGHT][MAP_WIDTH];

void MapRender();

#endif
