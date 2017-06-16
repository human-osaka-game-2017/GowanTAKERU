#ifndef MAINCONTROL_H
#define MAINCONTROL_H

#define GRAVITY 0.5f

void MainControl();

//Šp“x‚ğ‹‚ß‚Ü‚·
double Calculate_rad(float x1, float y1, float x2, float y2);

//‰~‚Æ‰~‚Ì‚ ‚½‚è”»’è
bool Circle_Hit(float cx1, float cy1, float r1, float cx2, float cy2, float r2);


#endif
