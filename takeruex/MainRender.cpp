#include"MainRender.h"
#include"DirectXGraphics.h"
#include"BackGroundRender.h"
#include"MapRender.h"
#include"PlayerRender.h"
#include"BulletRender.h"
#include"EnemyRender.h"

void MainRender() {

	IDirect3DDevice9* pD3Device = GetGraphicsDevice();

	//-----------------------------------------------------
	//ƒV[ƒ“•`‰æ
	//-----------------------------------------------------
	pD3Device->Clear(0, NULL,
		D3DCLEAR_TARGET,
		D3DCOLOR_XRGB(0xff, 0xff, 0xff),
		1.0, 0);

	pD3Device->BeginScene();

	BackgroundRender();
	MapRender();
	EnemyRender();
	BulletRender();
	PlayerRender();

	pD3Device->EndScene();
	pD3Device->Present(NULL, NULL, NULL, NULL);
}