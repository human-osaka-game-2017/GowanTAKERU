#include"DirectXInput.h"
#include"CommonControl.h"
#include<dinput.h>

/**
* コントローラの状態構造体
*/
struct CONTROLER_STATE
{
	XINPUT_STATE		State;		//!< ボタンなどの状態.
	XINPUT_VIBRATION	Vib_State;	//!< 振動の状態.
};

LPDIRECTINPUT8 g_pDInput;
LPDIRECTINPUTDEVICE8 g_pKeyDevice;
enum KEYSTATE g_Key[KEYMAX];

PADSTATE g_PadState[XINPUT_ID_MAX] = { PAD_OFF };		//!< パッド状態構造体.
CONTROLER_STATE g_PadControlState;				//!< コントローラステート構造体.

void UpdatePad() {

	XINPUT_CAPABILITIES XInputCapabilities;
	if (XInputGetCapabilities(0, 0, &XInputCapabilities) == ERROR_DEVICE_NOT_CONNECTED)
	{
		return;
	}

	XInputGetState(0, &g_PadControlState.State);
}

enum KEYSTATE* GetKey() {
	return g_Key;
}


PADSTATE GetButtonState(XINPUT_ID _buttonId)
{
	return g_PadState[_buttonId];
}

void DXInputKeybourdInit(HWND hWnd, HINSTANCE hInstance) {

	DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (VOID**)&g_pDInput, NULL);
	g_pDInput->CreateDevice(GUID_SysKeyboard, &g_pKeyDevice, NULL);
	g_pKeyDevice->SetDataFormat(&c_dfDIKeyboard);
	g_pKeyDevice->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE | DISCL_BACKGROUND);
	g_pKeyDevice->Acquire();

}

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

void FreeDxInput()
{
	g_pKeyDevice->Unacquire();

	g_pKeyDevice->Release();

	g_pDInput->Release();
}

void GamePadCheckButton(XINPUT_ID _buttonId, WORD _xinputButton)
{
	XINPUT_CAPABILITIES XInputCapabilities;
	if (XInputGetCapabilities(0, 0, &XInputCapabilities) == ERROR_DEVICE_NOT_CONNECTED)
	{
		return;
	}

	static PADSTATE PadOldState[XINPUT_ID_MAX] = { PAD_OFF };
	
	//ZeroMemory(&g_PadControlState,sizeof(CONTROLER_STATE));


	if (g_PadControlState.State.Gamepad.wButtons & _xinputButton)
	{
		if (PadOldState[_buttonId] == PAD_ON)
		{
			g_PadState[_buttonId] = PAD_ON;
		}
		else
		{
			g_PadState[_buttonId] = PAD_PUSH;
		}
		PadOldState[_buttonId] = PAD_ON;
	}
	else
	{
		if (PadOldState[_buttonId] == PAD_ON)
		{
			g_PadState[_buttonId] = PAD_RELEASE;
		}
		else
		{
			g_PadState[_buttonId] = PAD_OFF;
		}
		PadOldState[_buttonId] = PAD_OFF;
	}
}
