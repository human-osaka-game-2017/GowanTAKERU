#ifndef COMMONCONTROL_H
#define COMMONCONTROL_H

#include<dinput.h>
#include <XInput.h>

enum KEYSTATE {
	KEY_PUSH,
	KEY_RELEASE,
	KEY_ON,
	KEY_OFF
};

enum KEYKIND
{
	KEY_LEFT,
	KEY_RIGHT,
	KEY_UP,
	KEY_DOWN,
	KEY_A,
	KEY_B,
	KEY_C,
	KEY_D,
	KEY_E,
	KEY_F,
	KEY_G,
	KEY_H,
	KEY_I,
	KEY_J,
	KEY_K,
	KEY_L,
	KEY_M,
	KEY_N,
	KEY_O,
	KEY_P,
	KEY_Q,
	KEY_R,
	KEY_S,
	KEY_T,
	KEY_U,
	KEY_V,
	KEY_W,
	KEY_X,
	KEY_Y,
	KEY_Z,
	KEY_SPACE,
	KEY_ESC,
	KEYMAX
};

enum PADSTATE
{
	PAD_OFF,			//!< 離され続けている.
	PAD_ON,			//!< 押され続けている.
	PAD_RELEASE,	//!< 離されたら.
	PAD_PUSH		//!< 押されたら.
	
};

enum XINPUT_ID
{
	GAMEPAD_DANALOG_UP,			//!< アナログ十字キー上.
	GAMEPAD_DANALOG_DOWN,		//!< アナログ十字キー下.
	GAMEPAD_DANALOG_LEFT,		//!< アナログ十字キー左.
	GAMEPAD_DANALOG_RIGHT,		//!< アナログ十字キー右.
	GAMEPAD_START,				//!< スタートボタン.
	GAMEPAD_BACK,				//!< バックボタン.
	GAMEANALOG_LEFT_THUMB,		//!< 左スティック押し込み.
	GAMEANALOG_RIGHT_THUMB,		//!< 右スティック押し込み.
	GAMEANALOG_LEFT_SHOULDER,	//!< LBキー.
	GAMEANALOG_RIGHT_SHOULDER,	//!< RBキー.
	GAMEPAD_A,					//!< Aボタン.
	GAMEPAD_B,					//!< Bボタン.
	GAMEPAD_X,					//!< Xボタン.
	GAMEPAD_Y,					//!< Yボタン.
	XINPUT_ID_MAX				//!< ボタンの総数.
};

enum KEYSTATE* GetKey();

void KeyCheck(KEYSTATE* Key, int DIK);

void OutputDebug_Number(float outputNum, HWND hWnd);

PADSTATE GetButtonState(XINPUT_ID _buttonId);

void GamePadCheckButton(XINPUT_ID _buttonId, WORD _xinputButton);

void UpdatePad();

#endif 

