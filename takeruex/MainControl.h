#ifndef MAINCONTROL_H
#define MAINCONTROL_H

#define GRAVITY 0.3f

void MainControl();

//角度を求めます
//第一引数、第二引数 始点
//第三引数、第四引数 終点
double Calculate_rad(float x1, float y1, float x2, float y2);

float Calculate_distance(float x1, float y1, float x2, float y2);

void SetBulletMovement(int bulletNum);

//void PreventIntoPos(D3DXVECTOR2& oldPos, D3DXVECTOR2* currentPos, float width, float height);

#endif
