#include "Direct2D.h"
#include <memory>

#pragma comment(lib, "d2d1.lib")
using namespace Microsoft::WRL;

Direct2D::Direct2D( HWND WinHandle, IDXGIDevice1 *DxgiDevice,
					IDXGISurface *BackBuffer, LPTSTR FailMessage )
{
	ComPtr<IDXGISurface> dxgiBackBuffer( BackBuffer );

	FailMessage = L"Failed to create Direct2D factory.";
	HRESULT hr = D2D1CreateFactory( D2D1_FACTORY_TYPE_SINGLE_THREADED, m_pFactory.GetAddressOf() );

	if( SUCCEEDED( hr ) )
	{
		FailMessage = L"Failed to create Direct2D device.";
		hr = m_pFactory->CreateDevice( DxgiDevice, &m_pDevice );
	}
	if( SUCCEEDED( hr ) )
	{
		FailMessage = L"Failed to create Direct2D device context.";
		hr = m_pDevice->CreateDeviceContext( D2D1_DEVICE_CONTEXT_OPTIONS_NONE, &m_pContext );
	}
	if( SUCCEEDED( hr ) )
	{
		D2D1_BITMAP_PROPERTIES1 bp{};
		bp.bitmapOptions = D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW;
		bp.pixelFormat.alphaMode = D2D1_ALPHA_MODE_PREMULTIPLIED;
		bp.pixelFormat.format = DXGI_FORMAT_B8G8R8A8_UNORM;
		m_pFactory->GetDesktopDpi( &bp.dpiX, &bp.dpiY );

		FailMessage = L"Failed to create Direct2D bitmap from DXGI surface.";
		hr = m_pContext->CreateBitmapFromDxgiSurface( 
			dxgiBackBuffer.Get(), &bp, &m_pRenderTarget );
	}

	if( SUCCEEDED( hr ) )
	{
		m_pContext->SetTarget( m_pRenderTarget.Get() );
	}


}

Direct2D::Direct2D( Direct2D &&D2D )
	:
	m_pDevice( std::move( D2D.m_pDevice ) ),
	m_pContext( std::move( D2D.m_pContext ) ),
	m_pFactory( std::move( D2D.m_pFactory ) ),
	m_pRenderTarget( std::move( D2D.m_pRenderTarget ) )
{}

ID2D1DeviceContext1 *Direct2D::GetContext()const
{
	return m_pContext.Get();
}

void Direct2D::BeginFrame(D2D1::ColorF &Color)
{
	m_pContext->BeginDraw();
	m_pContext->Clear(Color);
}

void Direct2D::EndFrame()
{
	m_pContext->EndDraw();
}

