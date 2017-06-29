#ifndef FILEMANAGEMENT_H
#define FILEMANAGEMENT_H

#include<d3dx9.h>

//pTexture使いたい人つかって
LPDIRECT3DTEXTURE9* GetTexture();

//textureをリリース
void TextureFree();

//mainsceneのテキスチャを読み込んでます
void MainSceneLoad();
void CSVLoad(char* mapdata, int* map, int height, int width);

#endif
