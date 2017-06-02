#include"DGraphics.h"

IDirect3D9*			  g_pDirect3D;
IDirect3DDevice9*	  g_pD3Device;

#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)

IDirect3DDevice9* GetDevice() {

	return g_pD3Device;
}

void DXGraphicsInit(HWND hWnd, bool Window_mode) {

	D3DPRESENT_PARAMETERS D3dPresentParameters;
	D3DDISPLAYMODE		  D3DdisplayMode;

	//DirectX オブジェクトの生成
	g_pDirect3D = Direct3DCreate9(
		D3D_SDK_VERSION);

	//Display Mode の設定
	g_pDirect3D->GetAdapterDisplayMode(
		D3DADAPTER_DEFAULT,
		&D3DdisplayMode);

	ZeroMemory(&D3dPresentParameters,
		sizeof(D3DPRESENT_PARAMETERS));
	D3dPresentParameters.BackBufferFormat = D3DdisplayMode.Format;
	D3dPresentParameters.BackBufferCount = 1;
	D3dPresentParameters.SwapEffect = D3DSWAPEFFECT_DISCARD;
	D3dPresentParameters.Windowed = TRUE;

	//デバイスを作る
	g_pDirect3D->CreateDevice(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING,
		&D3dPresentParameters, &g_pD3Device);

	g_pD3Device->SetFVF(D3DFVF_CUSTOMVERTEX);

	//---------------------------------------------------
	//viewport設定
	//-------------------------------------------------
	D3DVIEWPORT9 Viewport{ 0,0,D3dPresentParameters.BackBufferWidth,D3dPresentParameters.BackBufferHeight,0.0f,1.0f };
	g_pD3Device->SetViewport(&Viewport);

}

void FreeDxGraphics()
{

	g_pD3Device->Release();

	g_pDirect3D->Release();
}