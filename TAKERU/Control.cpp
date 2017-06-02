#include"Control.h"

LPDIRECTINPUT8 g_pDInput;
LPDIRECTINPUTDEVICE8 g_pKeyDevice;

void KeyCheck(KEYSTATE* Key, int DIK) {
	BYTE diks[256];
	static BYTE old_diks[256] = { KEY_OFF };

	if (false != g_pKeyDevice->Acquire()) {
		g_pKeyDevice->GetDeviceState(sizeof(diks), diks);
		if (diks[DIK] & 0x80)
		{
			if (old_diks[DIK] == KEY_OFF)
			{
				*Key = KEY_PUSH;
				old_diks[DIK] = KEY_ON;
			}
			else
			{
				*Key = KEY_ON;
				old_diks[DIK] = KEY_ON;
			}
		}
		else
		{
			if (old_diks[DIK] == KEY_ON)
			{
				*Key = KEY_RELEASE;
				old_diks[DIK] = KEY_OFF;
			}
			else
			{
				*Key = KEY_OFF;
				old_diks[DIK] = KEY_OFF;
			}
		}
	}

}

void DXInputKeybourdInit(HWND hWnd, HINSTANCE hInstance) {

	DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (VOID**)&g_pDInput, NULL);
	g_pDInput->CreateDevice(GUID_SysKeyboard, &g_pKeyDevice, NULL);
	g_pKeyDevice->SetDataFormat(&c_dfDIKeyboard);
	g_pKeyDevice->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE | DISCL_BACKGROUND);
	g_pKeyDevice->Acquire();

}
void FreeDxInput()
{
	g_pKeyDevice->Unacquire();

	g_pKeyDevice->Release();

	g_pDInput->Release();
}