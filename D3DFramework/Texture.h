#pragma once

#include <memory>
#include <string>
#include <wrl.h>
#include <wincodec.h>
#include "Direct2D.h"
#include "Direct3D.h"

#pragma comment(lib, "windowscodecs.lib")

Microsoft::WRL::ComPtr<IWICImagingFactory2> WICCreateImagingFactory();


class TextureData
{
public:
	TextureData( const std::wstring &Filename );
	TextureData( unsigned Width, unsigned Height );
	TextureData( TextureData &&Src );

	TextureData &operator=( TextureData &&Src );

	unsigned GetWidth()const;
	unsigned GetHeight()const;
	unsigned char *const GetPixels()const;
	unsigned char *GetPixels();
private:
	unsigned m_width = 0, m_height = 0;
	std::unique_ptr<unsigned char[]> m_pPixelData;
	Microsoft::WRL::ComPtr<IWICImagingFactory2> m_pFactory;
};

class Texture2D
{
public:
	Texture2D( TextureData &&TexData );
	Texture2D( Texture2D &&Src );
	Texture2D( TextureData &&TexData, const Direct2D &D2D );
	Texture2D( TextureData &&TexData, const Direct3D &D3D );


	Texture2D &operator=( Texture2D &&Src );

	unsigned char const*const GetPixels()const;
	unsigned GetWidth()const;
	unsigned GetHeight()const;

	ID3D11ShaderResourceView *AsD3DTexture( const Direct3D &D3D );
	ID2D1Bitmap1 *AsD2DTexture( const Direct2D &D2D );
	
private:
	TextureData m_texData;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_pShaderResource;
	Microsoft::WRL::ComPtr<ID2D1Bitmap1> m_pBitmap;
};