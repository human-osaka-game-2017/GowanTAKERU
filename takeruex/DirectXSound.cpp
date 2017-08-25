#include"DirectXSound.h"
#include<dsound.h>

IDirectSound8* g_pDS8;
IDirectSoundBuffer8** g_pDSBuffer;

void SetBuffer(int num) {
	g_pDSBuffer = (IDirectSoundBuffer8**)malloc(sizeof(IDirectSoundBuffer8*) * num);
}

void FreeBuffer() {
	free(*g_pDSBuffer);
}

void DirectXSoundInit(HWND hWnd) {

	// COMの初期化
	CoInitialize(NULL);

	//サウンドデバイスの作成
	DirectSoundCreate8(NULL, &g_pDS8, NULL);

	//協調レベルの設定
	g_pDS8->SetCooperativeLevel(hWnd, DSSCL_NORMAL);
}

bool CreateBufferForWave(char* soundfile,int soundID) {
	MMIOINFO mmioInfo;
	HMMIO hMmio = NULL;

	// Waveファイルオープン
	memset(&mmioInfo, 0, sizeof(MMIOINFO));
	hMmio = mmioOpen(soundfile, &mmioInfo, MMIO_READ);
	if (!hMmio) {
		return false; // ファイルオープン失敗
	}

	MMRESULT mmRes;

	//riffchunkの設定-----------------------------------
	MMCKINFO riffChunk;
	riffChunk.fccType = mmioFOURCC('W', 'A', 'V', 'E');
	mmRes = mmioDescend(hMmio, &riffChunk, NULL, MMIO_FINDRIFF);
	if (mmRes != MMSYSERR_NOERROR) {
		mmioClose(hMmio, 0);
		return false;
	}
	//---------------------------------------------------

	//formatchunkの設定----------------------------------
	MMCKINFO formatChunk;
	formatChunk.ckid = mmioFOURCC('f', 'm', 't', ' ');
	mmRes = mmioDescend(hMmio, &formatChunk, &riffChunk, MMIO_FINDCHUNK);
	if (mmRes != MMSYSERR_NOERROR) {
		mmioClose(hMmio, 0);
		return false;
	}

	DSBUFFERDESC dsBufferDesc;
	dsBufferDesc.dwSize = sizeof(DSBUFFERDESC);
	dsBufferDesc.dwFlags = DSBCAPS_GETCURRENTPOSITION2;
	dsBufferDesc.dwReserved = 0;
	dsBufferDesc.lpwfxFormat = NULL;
	dsBufferDesc.guid3DAlgorithm = GUID_NULL;
	dsBufferDesc.lpwfxFormat = (WAVEFORMATEX*)malloc(sizeof(WAVEFORMATEX));

	//waveFormatEx構造体の設定
	DWORD fmsize = formatChunk.cksize;
	DWORD size = mmioRead(hMmio, (HPSTR)dsBufferDesc.lpwfxFormat, fmsize);
	if (size != fmsize) {
		mmioClose(hMmio, 0);
		return false;
	}

	mmioAscend(hMmio, &formatChunk, 0);
	//-----------------------------------------------------

	//datachunkの設定----------------------------------
	MMCKINFO dataChunk;
	dataChunk.ckid = mmioFOURCC('d', 'a', 't', 'a');
	mmRes = mmioDescend(hMmio, &dataChunk, &riffChunk, MMIO_FINDCHUNK);
	if (mmRes != MMSYSERR_NOERROR) {
		mmioClose(hMmio, 0);
		return false;
	}
	//---------------------------------------------------------

	//音データに設定---------------------------------------------

	//DWORD dwFileSize = mmioSeek(*hMmio, 0, SEEK_END);
	//mmioSeek(*hMmio, 0, SEEK_SET);
	char* pData = NULL;
	pData = (char*)malloc(sizeof(char)*dataChunk.cksize);
	size = mmioRead(hMmio, (HPSTR)pData, dataChunk.cksize);
	if (size != dataChunk.cksize) {
		free(pData);
		return false;
	}
	//-----------------------------------------------------------------
	dsBufferDesc.dwBufferBytes = size;

	mmioClose(hMmio, 0);

	//セカンダリバッファの作成
	IDirectSoundBuffer *ptmpBuf = 0;
	g_pDS8->CreateSoundBuffer(&dsBufferDesc, &ptmpBuf, NULL);
	ptmpBuf->QueryInterface(IID_IDirectSoundBuffer8, (void**)&g_pDSBuffer[soundID]);//IDirectSoundBufferをIDirectSoundBuffer8に変更
	ptmpBuf->Release();

	// セカンダリバッファにWaveデータ書き込み
	LPVOID lpvWrite = 0;
	DWORD dwLength = 0;
	if (DS_OK == g_pDSBuffer[soundID]->Lock(0, 0, &lpvWrite, &dwLength, NULL, NULL, DSBLOCK_ENTIREBUFFER)) {
		memcpy(lpvWrite, pData, dwLength);
		g_pDSBuffer[soundID]->Unlock(lpvWrite, dwLength, NULL, 0);
	}

	free(pData);
	free(dsBufferDesc.lpwfxFormat);

	return true;
}

void PlayBackSound(int soundID,bool loop, LONG volume) {

	g_pDSBuffer[soundID]->SetVolume(volume);
	
	if (loop) {
		g_pDSBuffer[soundID]->Play(0, 0, DSBPLAY_LOOPING);
	}
	else {
		g_pDSBuffer[soundID]->Play(0, 0, 0);
	}
	
}

void StopSound(int soundID) {
	g_pDSBuffer[soundID]->Stop();
}

//void PauseSound(int soundID) {
//	g_pDSBuffer[soundID]->
//}

void ReleaseBuffer(int soundMax) {
	for (int i = 0; i < soundMax; i++) {
		if (g_pDSBuffer[i] != NULL) {
			g_pDSBuffer[i]->Release();
			g_pDSBuffer[i] = NULL;
		}
	}
}

void ReleaseSoundDevice() {
	CoUninitialize();// COMの終了
	g_pDS8->Release();
}