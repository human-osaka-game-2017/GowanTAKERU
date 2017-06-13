#ifndef DIRECTXGRAPHICS_H
#define DIRECTXGRAPHICS_H

#include<d3dx9.h>

//グラフィックデバイス使いたい人向け
IDirect3DDevice9* GetGraphicsDevice();

//ダイレクトエックスグラフィック初期化関数
void DXGraphicsInit(HWND hWnd, bool Window_mode);

//描画方法設定
void TextureInit();

//デバイス、実態をリリース
void FreeDxGraphics();


#endif

