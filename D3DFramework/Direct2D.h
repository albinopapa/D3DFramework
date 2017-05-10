#pragma once

#include <d2d1_2.h>
#include <wrl.h>

class Direct2D
{
public:
	Direct2D( HWND WinHandle, IDXGIDevice1 *DxgiDevice,
			  IDXGISurface *BackBuffer, LPTSTR FailMessage );
	Direct2D( Direct2D &&D2D );
	
	ID2D1DeviceContext1 *GetContext()const;

	void BeginFrame(D2D1::ColorF &Color);
	void EndFrame();
private:
	Microsoft::WRL::ComPtr<ID2D1Device1> m_pDevice;
	Microsoft::WRL::ComPtr<ID2D1DeviceContext1> m_pContext;
	Microsoft::WRL::ComPtr<ID2D1Factory2> m_pFactory;
	Microsoft::WRL::ComPtr<ID2D1Bitmap1> m_pRenderTarget;
};

