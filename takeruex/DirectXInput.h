#ifndef DIRECTXINPUT_H
#define DIRECTXINPUT_H

#include<Windows.h>
#include <XInput.h>

void DXInputKeybourdInit(HWND hWnd, HINSTANCE hInstance);
void FreeDxInput();

#endif
