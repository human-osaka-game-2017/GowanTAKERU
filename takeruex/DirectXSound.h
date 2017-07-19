#ifndef DIRECTXSOUND_H
#define DIRECTXSOUND_H

#include<Windows.h>

enum SOUND_ID { SOUND01,SOUND02, SOUNDMAX };

//Directsoundの初期化を行います
//第一引数 ウィンドウのハンドル
void DirectXSoundInit(HWND hWnd);

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
void ReleaseBuffer();

//サウンドデバイスのリリース
void ReleaseSoundDevice();
#endif

