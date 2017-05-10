#include "Texture.h"
#include "Direct2D.h"
#include "Direct3D.h"

using Microsoft::WRL::ComPtr;

Microsoft::WRL::ComPtr<IWICImagingFactory2> WICCreateImagingFactory()
{
	HRESULT hr = CoInitialize( nullptr );
	IWICImagingFactory2 *pFactory = nullptr;
	CoCreateInstance(
		CLSID_WICImagingFactory,
		nullptr,
		CLSCTX_INPROC_SERVER,
		IID_PPV_ARGS( &pFactory )
	);

	if( hr != S_OK ) CoUninitialize();
	return pFactory;
}

TextureData::TextureData( const std::wstring &Filename )
	:
	m_pFactory( WICCreateImagingFactory() )
{	
	ComPtr<IWICBitmapDecoder> pDecoder;
	HRESULT hr = m_pFactory->CreateDecoderFromFilename( Filename.c_str(), nullptr, GENERIC_READ, WICDecodeMetadataCacheOnDemand, &pDecoder );
	if( FAILED( hr ) ) throw hr;

	ComPtr<IWICBitmapFrameDecode> pFrame;
	hr = pDecoder->GetFrame( 0, &pFrame );
	if( FAILED( hr ) ) throw hr;

	ComPtr<IWICFormatConverter> pConverter;
	hr = m_pFactory->CreateFormatConverter( &pConverter );
	hr = pConverter->Initialize(
		pFrame.Get(), GUID_WICPixelFormat32bppPBGRA, WICBitmapDitherTypeNone, 
		nullptr, 1.f, WICBitmapPaletteTypeCustom );
	if( FAILED( hr ) ) throw hr;

	pConverter->GetSize( &m_width, &m_height );
	const unsigned stride = m_width * 4;
	const unsigned buffersize = stride * m_height;
	m_pPixelData = std::make_unique<unsigned char[]>( buffersize );
	WICRect wr = { 0,0,int(m_width), int(m_height) };

	hr = pConverter->CopyPixels( &wr, stride, buffersize, m_pPixelData.get() );
	if( FAILED( hr ) ) throw hr;
}

TextureData::TextureData( unsigned Width, unsigned Height )
	:
	m_width( Width ),
	m_height( Height ),
	m_pPixelData( std::make_unique<unsigned char[]>( Width * Height * 4u ) )
{
	memset( m_pPixelData.get(), 0, Width * Height * sizeof( unsigned int ) );
}

TextureData::TextureData( TextureData &&Src )
	:
	m_pPixelData( std::move( Src.m_pPixelData ) ),
	m_pFactory( std::move( Src.m_pFactory ) ),
	m_width( Src.m_width ),
	m_height( Src.m_height )
{}

TextureData &TextureData::operator=( TextureData &&Src )
{
	m_pPixelData = std::move( Src.m_pPixelData );
	m_pFactory = std::move( Src.m_pFactory );
	m_width = Src.m_width;
	m_height = Src.m_height;

	Src.m_width = 0u;
	Src.m_height = 0u;

	return *this;
}

unsigned TextureData::GetWidth()const
{
	return m_width;
}
unsigned TextureData::GetHeight()const
{
	return m_height;
}
unsigned char *const TextureData::GetPixels()const
{
	return m_pPixelData.get();
}
unsigned char *TextureData::GetPixels()
{
	return m_pPixelData.get();
}
/***********************************Texture class ***************************/
Texture2D::Texture2D( TextureData &&TexData )
	:
	m_texData( std::move( TexData ) )
{}

Texture2D::Texture2D( Texture2D &&Src )
	:
	m_pBitmap( std::move( Src.m_pBitmap ) ),
	m_pShaderResource( std::move( Src.m_pShaderResource ) ),
	m_texData( std::move( Src.m_texData ) )
{}
Texture2D::Texture2D( TextureData &&TexData, const Direct2D &D2D )
	:
	Texture2D( std::move( TexData ) )
{
	auto size = D2D1_SIZE_U{ m_texData.GetWidth(), m_texData.GetHeight() };
	auto pitch = UINT32( size.width * 4u );
	auto bitmapProperties = D2D1::BitmapProperties1();
	auto pContext = D2D.GetContext();
	auto bitmap = m_pBitmap.Get();
	auto sourceData = reinterpret_cast< void* >( m_texData.GetPixels() );
	
	HRESULT hr = pContext->CreateBitmap( size, sourceData, pitch, &bitmapProperties, &bitmap );
	if( FAILED( hr ) )
	{
		bitmapProperties.pixelFormat.format = DXGI_FORMAT_B8G8R8A8_UNORM;
		bitmapProperties.pixelFormat.alphaMode = D2D1_ALPHA_MODE_PREMULTIPLIED;
		hr = pContext->CreateBitmap( size, sourceData, pitch, &bitmapProperties, &bitmap );
		if( FAILED( hr ) ) throw hr;
	}

}
Texture2D::Texture2D( TextureData &&TexData, const Direct3D &D3D )
	:
	Texture2D( std::move( TexData ) )
{
	D3D11_TEXTURE2D_DESC td{};
	td.Width = m_texData.GetWidth();
	td.Height = m_texData.GetHeight();
	td.MipLevels = 1;
	td.ArraySize = 1;
	td.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	td.SampleDesc.Count = 1;
	td.Usage = D3D11_USAGE_DEFAULT;
	td.BindFlags = D3D11_BIND_SHADER_RESOURCE;

	auto pDevice = D3D.GetDevice();
	ComPtr<ID3D11Texture2D> pTex;

	D3D11_SUBRESOURCE_DATA sd{};
	sd.pSysMem = m_texData.GetPixels();
	sd.SysMemPitch = m_texData.GetWidth() * 4;
	sd.SysMemSlicePitch = m_texData.GetHeight() * sd.SysMemPitch;

	pDevice->CreateTexture2D( &td, &sd, &pTex );

	D3D11_SHADER_RESOURCE_VIEW_DESC srvd{};
	srvd.Format = td.Format;
	srvd.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvd.Texture2D = { 0,1 };

	pDevice->CreateShaderResourceView( pTex.Get(), &srvd, &m_pShaderResource );
}

Texture2D &Texture2D::operator=( Texture2D &&Src )
{
	m_pBitmap = std::move( Src.m_pBitmap );
	m_pShaderResource = std::move( Src.m_pShaderResource );
	m_texData = std::move( Src.m_texData );

	return *this;
}

unsigned char const *const Texture2D::GetPixels()const
{
	return m_texData.GetPixels();
}

unsigned Texture2D::GetWidth()const
{
	return m_texData.GetWidth();
}
unsigned Texture2D::GetHeight()const
{
	return m_texData.GetHeight();
}

ID3D11ShaderResourceView *Texture2D::AsD3DTexture( const Direct3D &D3D )
{
	if( !m_pShaderResource )
	{
		*this = Texture2D( std::move( m_texData ), D3D );
	}	
	return m_pShaderResource.Get();
}
ID2D1Bitmap1 *Texture2D::AsD2DTexture( const Direct2D &D2D )
{
	if( !m_pBitmap )
	{
		*this = Texture2D( std::move( m_texData ), D2D );
	}
	return m_pBitmap.Get();
}

