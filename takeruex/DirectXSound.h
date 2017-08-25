#ifndef DIRECTXSOUND_H
#define DIRECTXSOUND_H

#include<Windows.h>

//enum MAINSCENESOUND_ID { MAINSCENE_BGM01, MAINSCENE_SE_ATTACK, MAINSCENE_SE_JUMP, MAINSCENE_SE_SWING, MAINSCENE_SOUNDMAX };
//enum TITLESCENESOUND_ID { TITLESCENE_BGM01, TITLESCENE_SOUNDMAX };
//enum GAMECLEARSCENESOUND_ID { GAMECLEAR_BGM01, GAMECLEAR_SOUNDMAX };
//enum GAMEOVERSCENESOUND_ID { GAMEOVERSCENE_BGM01, GAMEOVER_SOUNDMAX };

enum SOUND_ID {
	MAINSCENE_STAGEBGM01,
	MAINSCENE_STAGEBGM02,
	MAINSCENE_STAGEBGM03,
	MAINSCENE_STAGEBGM04,
	MAINSCENE_BOSSBGM01,
	MAINSCENE_BOSSBGM02,
	MAINSCENE_BOSSBGM03,
	MAINSCENE_SE_ATTACK,
	MAINSCENE_SE_JUMP,
	MAINSCENE_SE_SWING,
	TITLESCENE_BGM01,
	GAMECLEAR_BGM01,
	GAMEOVERSCENE_BGM01,
	SOUND_MAX
};

//Directsoundの初期化を行います
//第一引数 ウィンドウのハンドル
void DirectXSoundInit(HWND hWnd);

//bufferを動的に確保します
void SetBuffer(int num);

//動的に確保した物の開放
void FreeBuffer();

//バッファをwaveファイルから作成します。バッファはグローバルです。
//第一引数 waveファイルへのパス
//第二引数 識別するためのID（enum)
//戻り値 成功＝true,失敗＝false
bool CreateBufferForWave(char* soundfile, int soundID);

//サウンドを再生します
//第一引数 識別するためのID（enum)
//第二引数 ループするならtrue,しないならfalse
void PlayBackSound(int soundID, bool loop, LONG volume);

// サウンドを停止します
//第一引数 識別するためのID（enum)
void StopSound(int soundID);

//バッファのリリース
void ReleaseBuffer(int soundMax);

//サウンドデバイスのリリース
void ReleaseSoundDevice();

#endif

