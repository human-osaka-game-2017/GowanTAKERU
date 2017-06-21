#ifndef MAINHITMANAGEMENT_H
#define MAINHITMANAGEMENT_H

#include<d3dx9.h>

void HitManage();

//‰~‚Æ‰~‚Ì‚ ‚½‚è”»’è
bool CircleHit(float cx1, float cy1, float r1, float cx2, float cy2, float r2);

//‹éŒ`‚Æ‹éŒ`‚Ì‚ ‚½‚è”»’è
bool SquareHit(D3DXVECTOR2* pPos1, float width1, float height1, D3DXVECTOR2* pPos2, float width2, float height2);
#endif
