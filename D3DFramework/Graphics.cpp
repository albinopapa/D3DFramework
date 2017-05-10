#define NOMINMAX
#include <assert.h>
#include "Graphics.h"
#include "Direct3D.h"
#include <pmmintrin.h>
#include "SimpleModel.h"
#include <sstream>
#include "Texture.h"
#include "Utilities.h"
#include <vector>
#include "Window.h"



int Graphics::m_screenWidth = g_winrect.GetWidth();
int Graphics::m_screenHeight= g_winrect.GetHeight();

Graphics::Graphics( Window &Win, Direct3D &D3D )
	:
	m_Direct3D( D3D ),
	m_topology( D3D.GetContext(), IA::PrimitiveTopology::TriangleList ),
	m_vertexBuffers( D3D ),
	m_indexBuffer( D3D ),
	m_inputLayout( D3D ),
	m_vertexShader( D3D ),
	m_pixelShader( D3D ),
	m_viewport( D3D )
{
	m_screenWidth = Win.GetWidth();
	m_screenHeight = Win.GetHeight();
	const auto pDevice = m_Direct3D.GetDevice();

	// Create texture and shader resource view
	{
		D3D11_TEXTURE2D_DESC td{
			static_cast<UINT>( m_screenWidth ),
			static_cast<UINT>( m_screenHeight ),
			1u,
			1u,
			DXGI_FORMAT_B8G8R8A8_UNORM,
			{ 1u, 0u },
			D3D11_USAGE_DYNAMIC,
			D3D11_BIND_SHADER_RESOURCE,
			D3D11_CPU_ACCESS_WRITE,
			0u
		};

		// Create texture
		{
			const auto hr = pDevice->CreateTexture2D( &td, nullptr, &m_pSysTexture );
			if( FAILED( hr ) )
			{
				MessageBox( nullptr, L"Failed to create system texture.",
							L"System texture creation error.", MB_OK );
				throw( hr );
			}
		}

		// Create shader resource view
		{
			D3D11_SHADER_RESOURCE_VIEW_DESC srv{};
			srv.Format = td.Format;
			srv.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
			srv.Texture2D = { 0, 1 };

			const auto hr = pDevice->CreateShaderResourceView( m_pSysTexture.Get(), &srv, &m_pShaderResourceView );
			if( FAILED( hr ) )
			{
				MessageBox( nullptr, L"Failed to create shader resource view for the system texture.", L"Shader resource view for the system texture creation error.", MB_OK );
				throw( hr );
			}
		}
	}

	// Create sampler state
	{
		FLOAT borderColor[] = { 0.f, 0.f, 0.f, 0.f };
		D3D11_SAMPLER_DESC sampDesc{
			D3D11_FILTER::D3D11_FILTER_MIN_MAG_MIP_POINT,
			D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP,
			D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP,
			D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP,
			0.f,
			1,
			D3D11_COMPARISON_FUNC::D3D11_COMPARISON_NEVER,
			borderColor[ 0 ],
			borderColor[ 1 ],
			borderColor[ 2 ],
			borderColor[ 3 ],
			0.f,
			D3D11_FLOAT32_MAX
		};
		const auto hr = pDevice->CreateSamplerState( &sampDesc, &m_pSamplerState );
		if ( FAILED( hr ) )
		{
			MessageBox( nullptr, L"Failed to create sampler state.", 
						L"Sampler state creation error.", MB_OK );
			throw( hr );
		}
	}

	// Create vertex buffer
	{
		HRESULT hr = m_vertexBuffers.Create(
			std::vector<SimpleModel::VertexBuffer>{
				{ -1.0f, 1.0f, 0.5f, 0.0f, 0.0f },
				{ 1.0f, 1.0f, 0.5f, 1.0f, 0.0f },
				{ -1.0f, -1.0f, 0.5f, 0.0f, 1.0f },
				{ 1.0f, -1.0f, 0.5f, 1.0f, 1.0f }
		} );
		if ( FAILED( hr ) )
		{
			MessageBox( nullptr, L"Failed to create vertex buffer.", 
						L"Vertex buffer creation error.", MB_OK );
			throw( hr );
		}
	}

	// Create index buffer
	{
		const auto hr = m_indexBuffer.Create( { 0, 1, 2, 3, 2, 1 } );
		if ( FAILED( hr ) )
		{
			MessageBox( nullptr, L"Failed to create index buffer.", 
						L"Index buffer creation error.", MB_OK );
			throw( hr );
		}
	}

	// Create vertex shader, pixel shader, and input layout
	{
		ComPtr<ID3DBlob> vsBlob;
		// Create vertex shader
		{
			const auto hr = m_vertexShader.Create( L"backbuffer_vs.cso", &vsBlob );
			if( FAILED( hr ) )
			{
				MessageBox( nullptr, L"Failed to create vertex shader.",
							L"Vertex shader creation error.", MB_OK );
				throw( hr );
			}
		}

		// Create pixel shader
		{
			const auto hr = m_pixelShader.Create( L"backbuffer_ps.cso" );
			if( FAILED( hr ) )
			{
				MessageBox( nullptr, L"Failed to create pixel shader.",
							L"Pixel shader creation error.", MB_OK );
				throw( hr );
			}
		}

		// Create input layout
		{
			using Pipeline::InputAssembler::InputLayout;
			std::vector<InputLayout::InputElementDesc> ied{
				{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0 },
				{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12 }
			};
			const auto hr = m_inputLayout.Create( vsBlob.Get(), std::move( ied ) );
			if( FAILED( hr ) )
			{
				MessageBox( nullptr, L"Failed to create input layout.",
							L"Input layout creation error.", MB_OK );
				throw( hr );
			}
		}
	}

	// Create view port
	{
		m_viewport.Create(
			0.f, 0.f,
			static_cast< float >( m_screenWidth ),
			static_cast< float >( m_screenHeight ),
			0.f, 1.f );
	}

	// Create intermediate system draw buffer
	m_pSysbuffer = std::make_unique<Color[]>( m_screenWidth * m_screenHeight );
}


Graphics::~Graphics()
{}

void Graphics::BeginFrame( Color FillColor )
{
	/*for( int y = 0; y < m_screenHeight; ++y )
	{
		for( int x = 0; x < m_screenWidth; ++x )
		{
			PutPixel( { x,y }, FillColor );
		}
	}*/

	memset( m_pSysbuffer.get(), 0x20, m_screenWidth * m_screenHeight * sizeof( unsigned ) );
}

void Graphics::EndFrame( )
{
	// Update texture
	{
		using TexResource = Pipeline::Utils::MappedResource<ID3D11Texture2D>;
		TexResource texResource( m_Direct3D, m_pSysTexture.Get() );
		const auto* pPixels = reinterpret_cast< BYTE* >( m_pSysbuffer.get() );
		auto hr = texResource.Update( pPixels, m_screenWidth, m_screenHeight );

		if ( FAILED( hr ) )
		{
			std::wstringstream ss;
			ss << hr << L": Failed to update the current frame.";
			MessageBox( nullptr, ss.str().c_str(), L"Map error.", MB_OK );
			throw( hr );
		}
	}

	// Set pipeline state
	{
		const auto pContext = m_Direct3D.GetContext();
		const auto pRenderTarget = m_Direct3D.GetRenderTarget();

		m_topology.Set();
		m_vertexBuffers.Set();
		m_indexBuffer.Set();
		m_inputLayout.Set();

		pContext->OMSetRenderTargets( 1u, &pRenderTarget, nullptr );
		pContext->PSSetSamplers( 0u, 1u, m_pSamplerState.GetAddressOf() );
		pContext->PSSetShaderResources( 0u, 1u, m_pShaderResourceView.GetAddressOf() );
		m_vertexShader.Set();
		m_pixelShader.Set();
		m_viewport.Set();

		pContext->DrawIndexed( 6u, 0u, 0u );
	}

	// Present the frame
	{
		const auto hr = m_Direct3D.GetSwapchain()->Present( 0u, 0u );
		if ( FAILED( hr ) )
		{
			if ( hr == DXGI_ERROR_DEVICE_REMOVED )
			{
				throw hr;
			}
			MessageBox( nullptr, L"Failed to present the current frame.", L"Present frame error.", MB_OK );
		}
	}
}

void Graphics::PutPixel( int X, int Y, Color Color )
{
	assert( X >= 0 );
	assert( Y >= 0 );
	assert( X < g_winrect.GetWidth() );
	assert( Y < g_winrect.GetHeight() );

	const int idx = X + ( Y * m_screenWidth );
	m_pSysbuffer[ idx ] = Color;
}

void Graphics::PutPixel( const Point2D & P, Color C )
{
	PutPixel( P.x, P.y, C );
}

void Graphics::DrawLine( int X0, int Y0, int X1, int Y1, Color C )
{
	const float dx = static_cast<float>(X1 - X0);
	const float dy = static_cast<float>(Y1 - Y0);

	if ( dy == 0.0f && dx == 0.0f )
	{
		PutPixel( X0, Y0, C );
	}
	else if ( abs( dy ) > abs( dx ) )
	{
		if ( dy < 0.0f )
		{
			std::swap( X0, X1 );
			std::swap( Y0, Y1 );
		}

		const float m = dx / dy;		
		int lastIntY = Y1;
		const float fx0 = static_cast<float>( X0 );
		const float fy0 = static_cast<float>( Y0 );
		const float fx1 = static_cast<float>( X1 );
		const float fy1 = static_cast<float>( Y1 );

		for ( float x = fx0, y = fy0; y < fy1; y += 1.0f, x += m )
		{
			lastIntY = static_cast< int >( y );
			PutPixel( static_cast< int >( x ), lastIntY, C );
		}
		if ( Y1 > lastIntY )
		{
			PutPixel( X1, Y1, C );
		}
	}
	else
	{
		if ( dx < 0.0f )
		{
			std::swap( X0, X1 );
			std::swap( Y0, Y1 );
		}

		const float m = dy / dx;
		
		int lastIntX = X1;
		const float fx0 = static_cast<float>( X0 );
		const float fy0 = static_cast<float>( Y0 );
		const float fx1 = static_cast<float>( X1 );
		const float fy1 = static_cast<float>( Y1 );

		for ( float y = fy0, x = fx0; x < fx1; x += 1.0f, y += m )
		{
			lastIntX = static_cast< int >( x );
			PutPixel( lastIntX, static_cast< int >( y ), C );
		}
		if ( X1 > lastIntX )
		{
			PutPixel( X1, Y1, C );
		}
	}
}

void Graphics::DrawLine( const Point2D & Start, const Point2D & End, Color C )
{
	DrawLine( Start.x, Start.y, End.x, End.y, C );
}

void Graphics::DrawBoxGradient( const Recti & Box, const float Value[ 4 ] )
{
	const auto left = std::max<int>( -Box.left, 0 );
	const auto top = std::max<int>( -Box.top, 0 );
	const auto right = std::min<int>( m_screenWidth - 1 - Box.left, Box.GetWidth() );
	const auto bottom = std::min<int>( m_screenHeight - 1 - Box.top, Box.GetHeight() );

	const float rec255 = 1.f / 255.f;
	// Top left block
	const float tlBlock = Value[ 0 ] * rec255;
	// Top right block
	const float trBlock = Value[ 1 ] * rec255;
	// Bottom left block 
	const float blBlock = Value[ 2 ] * rec255;
	// Bottom right block
	const float brBlock = Value[ 3 ] * rec255;

	const float size = 20.f;
	const float step = 1.f / size;
	float yStep = 0.f;

	for( int y = top; y < bottom; ++y, yStep += step )
	{
		float xStep = 0.f;
		const float inv_yStep = 1.f - yStep;
		for( int x = left; x < right; ++x, xStep += step )
		{
			const float inv_xStep = 1.f - xStep;

			const float ul = inv_xStep * inv_yStep;
			const float ll = inv_xStep * yStep;
			const float ur = xStep * inv_yStep;
			const float lr = xStep * yStep;

			const float value =
				( ul * tlBlock ) +
				( ll * blBlock ) +
				( ur * trBlock ) +
				( lr * brBlock );
			const unsigned colorLevel = static_cast< unsigned >( value * 255.f );
			const Color color = Color( colorLevel, colorLevel, colorLevel );
			PutPixel( x + Box.left, y + Box.top, color );
		}
	}
}

void Graphics::DrawBoxFilled( const Recti & Box, Color Color )
{
	const auto left = std::max<int>( -Box.left, 0 );
	const auto top = std::max<int>( -Box.top, 0 );
	const auto right = std::min<int>( m_screenWidth - 1 - Box.left, Box.GetWidth() );
	const auto bottom = std::min<int>( m_screenHeight - 1 - Box.top, Box.GetHeight() );

	const auto x = Box.left;
	const auto y = Box.top;

	for ( int iy = top; iy < bottom; ++iy )
	{
		DrawLine( x + left, y + iy, x + right, y + iy, Color );
	}
}
void Graphics::DrawCircleFilled( int X, int Y, int Radius, Color Color )
{
	const auto _2Rad = Radius << 1;
	const auto _left = X - Radius;
	const auto _top = Y - Radius;

	const auto left =   std::max<int>( -_left, 0 ) - Radius;
	const auto top =    std::max<int>( -_top, 0 ) - Radius;
	const auto right =  std::min<int>( m_screenWidth - 1 - _left, _2Rad ) - Radius;
	const auto bottom = std::min<int>( m_screenHeight - 1 - _top, _2Rad ) - Radius;

	const auto sqRad = Radius * Radius;
	for ( int y = top; y < bottom; ++y )
	{
		for ( int x = left; x < right; ++x )
		{
			const auto sqDist = ( ( x*x ) + ( y*y ) );
			if ( sqDist <= sqRad )
			{
				PutPixel( x + X, y + Y, Color );
			}
		}
	}
}
void Graphics::DrawCircleFilled( const Point2D &Center, int Radius, Color C )
{
	DrawCircleFilled( Center.x, Center.y, Radius, C );
}
void Graphics::DrawBoxOutline( const Recti & Box, Color Color )
{
	const auto bottom = Box.bottom - 1l;

	DrawLine( Box.left, Box.top, Box.right, Box.top, Color );
	DrawLine( Box.left, bottom, Box.right, bottom, Color );
	DrawLine( Box.left, Box.top, Box.left, bottom, Color );
	DrawLine( Box.right, Box.top, Box.right, bottom, Color );
}
void Graphics::DrawCircleOutline( int X, int Y, int InnerRadius, int OuterRadius, Color Color )
{
	if( InnerRadius < OuterRadius )
	{
		const auto sqRadius = OuterRadius * OuterRadius;
		const auto sqInnerRadius = InnerRadius * InnerRadius;

		for( int y = -OuterRadius; y < OuterRadius; ++y )
		{
			for( int x = -OuterRadius; x < OuterRadius; ++x )
			{
				const auto dist = ( x*x ) + ( y*y );

				if( dist <= sqRadius && dist > sqInnerRadius )
				{
					PutPixel( x + X, y + Y, Color );
				}
			}
		}
	}
}

void Graphics::DrawCircleOutline( const Point2D & Center, int InnerRadius, int OuterRadius, Color C )
{
	DrawCircleOutline( Center.x, Center.y, InnerRadius, OuterRadius, C );
}

void Graphics::DrawTriangle( const Vertex2D &P0, const Vertex2D& P1, const Vertex2D& P2 )
{
	Vertex2D p0 = P0;
	Vertex2D p1 = P1;
	Vertex2D p2 = P2;
	if( p1.position.y < p0.position.y ) std::swap( p1, p0 );
	if( p2.position.y < p1.position.y ) std::swap( p2, p1 );
	if( p1.position.y < p0.position.y ) std::swap( p1, p0 );

	if( p0.position.y == p1.position.y )
	{
		if( p1.position.x < p0.position.x ) std::swap( p1, p0 );
		PrepFlatTopTriangle( p0, p1, p2 );
	}
	else if( p1.position.y == p2.position.y )
	{
		if( p2.position.x < p1.position.x ) std::swap( p2, p1 );
		PrepFlatBottomTriangle( p0, p1, p2 );
	}
	else
	{
		// Calculate intermediate vertex point on major segment
		const float lHeight = p2.position.y - p0.position.y;
		const float sHeight = p1.position.y - p0.position.y;
		const float heightRatio = sHeight / lHeight;
		const Vec2f v = { ( 
			( p2.position.x - p0.position.x ) / lHeight ) * sHeight + p0.position.x, p1.position.y };

		// Calculate uv coordinates for intermediate vertex point
		const Vec2f t = p0.texcoord + ( p2.texcoord - p0.texcoord ) * heightRatio;

		// Calculate color for intermediate vertex point
		ColorF c = p0.tint + ( p2.tint - p0.tint ) * heightRatio;

		// Compose intermediate vertex
		Vertex2D p3 = { v, t, c };

		// If major right
		if( p1.position.x > p3.position.x ) std::swap( p1, p3 );

		PrepFlatBottomTriangle( p0, p1, p3 );
		PrepFlatTopTriangle( p1, p3, p2 );
	}
}
void Graphics::DrawTriangle( const Vertex2D &P0, const Vertex2D& P1, const Vertex2D& P2, const Texture2D &Tex )
{
	Vertex2D p0 = P0;
	Vertex2D p1 = P1;
	Vertex2D p2 = P2;
	if( p1.position.y < p0.position.y ) std::swap( p1, p0 );
	if( p2.position.y < p1.position.y ) std::swap( p2, p1 );
	if( p1.position.y < p0.position.y ) std::swap( p1, p0 );

	if( p0.position.y == p1.position.y )
	{
		if( p1.position.x < p0.position.x ) std::swap( p1, p0 );
		PrepFlatTopTriangle( p0, p1, p2, Tex );
	}
	else if( p1.position.y == p2.position.y )
	{
		if( p2.position.x < p1.position.x ) std::swap( p2, p1 );
		PrepFlatBottomTriangle( p0, p1, p2, Tex );
	}
	else
	{
		const float recLHeight = 1.f / ( p2.position.y - p0.position.y );
		const float sHeight = p1.position.y - p0.position.y;
		const float heightRatio = sHeight * recLHeight;

		// Calculate intermediate vertex point on major segment
		const Vec2f v = { (
			( p2.position.x - p0.position.x ) * recLHeight ) * sHeight + p0.position.x, p1.position.y };

		// Calculate uv coordinates for intermediate vertex point
		const Vec2f t = p0.texcoord + ( p2.texcoord - p0.texcoord ) * heightRatio;

		// Calculate color for intermediate vertex point
		ColorF c = p0.tint + ( p2.tint - p0.tint ) * heightRatio;

		// Compose intermediate vertex
		Vertex2D p3 = { v, t, c };

		// If major right
		if( p1.position.x > p3.position.x ) std::swap( p1, p3 );

		PrepFlatBottomTriangle( p0, p1, p3, Tex );
		PrepFlatTopTriangle( p1, p3, p2, Tex );
	}
}

void Graphics::DrawModel2D( const Model2D & Mod, const Mat3 &Transform )
{
	const auto &vert = Mod.GetVertices();
	const auto count = vert.size();
	for( int i = 0; i < count; i += 3 )
	{
		Vertex2D v0, v1, v2;
		v0.position = Transform * vert[ i + 0 ].position;
		v0.texcoord = vert[ i + 0 ].texcoord;
		v0.tint = vert[ i + 0 ].tint;

		v1.position = Transform * vert[ i + 1 ].position;
		v1.texcoord = vert[ i + 1 ].texcoord;
		v1.tint = vert[ i + 1 ].tint;

		v2.position = Transform * vert[ i + 2 ].position;
		v2.texcoord = vert[ i + 2 ].texcoord;
		v2.tint = vert[ i + 2 ].tint;

		DrawTriangle( v0, v1, v2 );
	}
}

void Graphics::DrawModel2D( const Model2D &Mod, const Texture2D &Tex, const Mat3 &Transform )
{
	const auto &vert = Mod.GetVertices();
	const auto count = vert.size();
	for( int i = 0; i < count; i += 3 )
	{
		Vertex2D v0, v1, v2;
		v0.position = Transform * vert[ i + 0 ].position;
		v0.texcoord = vert[ i + 0 ].texcoord;
		v0.tint = vert[ i + 0 ].tint;

		v1.position = Transform * vert[ i + 1 ].position;
		v1.texcoord = vert[ i + 1 ].texcoord;
		v1.tint = vert[ i + 1 ].tint;

		v2.position = Transform * vert[ i + 2 ].position;
		v2.texcoord = vert[ i + 2 ].texcoord;
		v2.tint = vert[ i + 2 ].tint;

		DrawTriangle( v0, v1, v2, Tex );
	}
}


// Private functions
void Graphics::PrepFlatTopTriangle( 
	const Vertex2D &p0, const Vertex2D& p1, const Vertex2D& p2 )
{
	// To do: Replace v2 - v0 with precalculated height
	const float recHeight = 1.f / ( p2.position.y - p0.position.y );

	// calulcate slopes in screen space
	float m[] = {
		{ ( p2.position.x - p0.position.x ) * recHeight },
		{ ( p2.position.x - p1.position.x ) * recHeight }
	};

	// calculate start and end scanlines
	Range2D yRange{
		std::max( ( int )ceilf( p0.position.y ), 0 ),
		std::min( ( int )ceilf( p2.position.y ) - 1, m_screenHeight - 1 )
	};

	Vertex2D edgeStepL{
		{ p0.position },
		{ ( p2.texcoord - p0.texcoord ) * recHeight },
		{ ( p2.tint - p0.tint ) * recHeight }
	};
	Vertex2D edgeStepR{
		{ p1.position },
		{ ( p2.texcoord - p1.texcoord ) * recHeight },
		{ ( p2.tint - p1.tint ) * recHeight }
	};

	DrawFlatTriangle( p0, p1, edgeStepL, edgeStepR, yRange, m );	
}
void Graphics::PrepFlatTopTriangle( 
	const Vertex2D &p0, const Vertex2D& p1, const Vertex2D& p2, const Texture2D &Tex )
{
	// To do: Replace v2 - v0 with precalculated height
	const float recHeight = 1.f / ( p2.position.y - p0.position.y );

	// calulcate slopes in screen space
	const float m[] = {
		{ ( p2.position.x - p0.position.x ) * recHeight },
		{ ( p2.position.x - p1.position.x ) * recHeight }
	};

	// calculate start and end scanlines
	Range2D yRange{
		std::max( ( int )ceilf( p0.position.y ), 0 ),
		std::min( ( int )ceilf( p2.position.y ) - 1, m_screenHeight - 1 )
	};

	Vertex2D edgeStepL{
		{ p0.position },
		{ ( p2.texcoord - p0.texcoord ) * recHeight },
		{ ( p2.tint - p0.tint ) * recHeight }
	};
	Vertex2D edgeStepR{
		{ p1.position },
		{ ( p2.texcoord - p1.texcoord ) * recHeight },
		{ ( p2.tint - p1.tint ) * recHeight }
	};
	
	DrawFlatTriangle( p0, p1, edgeStepL, edgeStepR, yRange, Tex, m );
	/*for( int y = yStart; y <= yEnd;
		 y++,
		 tEdgeL += tEdgeStepL,
		 tEdgeR += tEdgeStepR,
		 cEdgeL += cEdgeStepL,
		 cEdgeR += cEdgeStepR )
	{
		const float f_y = float( y );
		// caluclate start and end points
		const float px0 = m0 * ( f_y - p0.position.y ) + p0.position.x;
		const float px1 = m1 * ( f_y - p1.position.y ) + p1.position.x;
		const float recWidth = 1.f / ( px1 - px0 );

		// calculate start and end pixels
		const int xStart = std::max( ( int )ceilf( px0 ), 0 );
		const int xEnd = std::min( ( int )ceilf( px1 ) - 1, m_screenWidth );

		// calculate uv scanline step
		const Vec2 tScanStep = ( tEdgeR - tEdgeL ) * recWidth;

		// calculate uv point prestep
		Vec2 t = tEdgeL + tScanStep * ( float( xStart ) - px0 );

		// calculate color scan step
		ColorF cScanStep = ( cEdgeR - cEdgeL ) * recWidth;

		// calculate color gradient
		ColorF c = cEdgeL + cScanStep * ( float( xStart ) - px0 );

		auto pPixels = reinterpret_cast< Color const* const >( Tex.GetPixels() );
		const auto tWidth = Tex.GetWidth();
		const auto tHeight = Tex.GetHeight();
		for( int x = xStart; x <= xEnd; x++, t += tScanStep, c += cScanStep )
		{
			const auto u = t.x * float( tWidth );

			const auto tx = static_cast<unsigned>( ( t.x * float( tWidth - 1) ) );
			const auto ty = static_cast<unsigned>( ( t.y * float( tHeight- 1 ) ) );
			const auto tint = Color(
				static_cast< unsigned >( c.GetA() * 255.f ),
				static_cast< unsigned >( c.GetR() * 255.f ),
				static_cast< unsigned >( c.GetG() * 255.f ),
				static_cast< unsigned >( c.GetB() * 255.f ) );
			Color i_c = pPixels[ tx + ( ty * tWidth ) ] * tint;
			PutPixel( x, y, i_c );
		}
	}*/
}

void Graphics::PrepFlatBottomTriangle( 
	const Vertex2D &p0, const Vertex2D& p1, const Vertex2D& p2 )
{
	// To do: Replace v2 - v0 with precalculated height
	const float recHeight = 1.f / ( p2.position.y - p0.position.y );

	// calulcate slopes in screen space
	const float m[] = {
		{ ( p1.position.x - p0.position.x ) * recHeight },
		{ ( p2.position.x - p0.position.x ) * recHeight }
	};

	// calculate start and end scanlines
	const Range2D yRange{
		std::max( ( int )ceilf( p0.position.y ), 0 ),
		std::min( ( int )ceilf( p2.position.y ) - 1, m_screenHeight - 1 )
	};

	// calculate uv edge unit steps
	const Vertex2D edgeStepL{
		{p0.position},
		{ ( p1.texcoord - p0.texcoord ) * recHeight },
		{ ( p1.tint - p0.tint ) * recHeight }
	};
	const Vertex2D edgeStepR{
		{p0.position},
		{ ( p2.texcoord - p0.texcoord ) * recHeight },
		{ ( p2.tint - p0.tint ) * recHeight }
	};

	DrawFlatTriangle( p0, p0, edgeStepL, edgeStepR, yRange, m );
}
void Graphics::PrepFlatBottomTriangle(
	const Vertex2D &p0, const Vertex2D& p1, const Vertex2D& p2, const Texture2D &Tex )
{
	// To do: Replace v2 - v0 with precalculated height
	const float recHeight = 1.f / ( p2.position.y - p0.position.y );

	// calulcate slopes in screen space
	const float m[] = {
		{ ( p1.position.x - p0.position.x ) * recHeight },
		{ ( p2.position.x - p0.position.x ) * recHeight }
	};

	// calculate start and end scanlines
	const Range2D yRange{
		std::max( ( int )ceilf( p0.position.y ), 0 ),
		std::min( ( int )ceilf( p2.position.y ) - 1, m_screenHeight - 1 )
	};

	// calculate uv edge unit steps
	const Vertex2D edgeStepL{
		{ p0.position },
		{ ( p1.texcoord - p0.texcoord ) * recHeight },
		{ ( p1.tint - p0.tint ) * recHeight }
	};
	const Vertex2D edgeStepR{
		{ p0.position },
		{ ( p2.texcoord - p0.texcoord ) * recHeight },
		{ ( p2.tint - p0.tint ) * recHeight }
	};

	DrawFlatTriangle( p0, p0, edgeStepL, edgeStepR, yRange, Tex, m );
}

void Graphics::DrawFlatTriangle( const Vertex2D &TopLeft, const Vertex2D &TopRight, const Vertex2D &EdgeStepL, const Vertex2D &EdgeStepR, const Range2D &yRange, const float Slopes[ 2 ] )
{
	const float yPreStep = float( yRange.start ) - TopLeft.position.y;

	Vertex2D edgeL, edgeR;
	// calculate color edge prestep
	edgeL.tint = TopLeft.tint + EdgeStepL.tint * yPreStep;
	edgeR.tint = TopRight.tint + EdgeStepR.tint * yPreStep;

	for( int y = yRange.start; y <= yRange.end;
		 y++,
		 edgeL.tint += EdgeStepL.tint,
		 edgeR.tint += EdgeStepR.tint )
	{
		const float f_y = float( y );
		// caluclate start and end points
		const float px0 = Slopes[ 0 ] * ( f_y - TopLeft.position.y ) + TopLeft.position.x;
		const float px1 = Slopes[ 1 ] * ( f_y - TopRight.position.y ) + TopRight.position.x;
		const float recWidth = 1.f / ( px1 - px0 );

		// calculate start and end pixels
		const int xStart = std::max( ( int )ceilf( px0 ), 0 );
		const int xEnd = std::min( ( int )ceilf( px1 ) - 1, m_screenWidth );

		// calculate color scan step
		ColorF cScanStep = ( edgeR.tint - edgeL.tint ) * recWidth;

		// calculate color gradient
		ColorF c = edgeL.tint + cScanStep * ( float( xStart ) - px0 );

		for( int x = xStart; x <= xEnd; x++, c += cScanStep )
		{
			const auto tint = Color(
				static_cast< unsigned >( c.GetA() * 255.f ),
				static_cast< unsigned >( c.GetR() * 255.f ),
				static_cast< unsigned >( c.GetG() * 255.f ),
				static_cast< unsigned >( c.GetB() * 255.f ) );

			PutPixel( x, y, tint );
		}
	}
}

void Graphics::DrawFlatTriangle( const Vertex2D &TopLeft, const Vertex2D &TopRight, const Vertex2D &EdgeStepL, const Vertex2D &EdgeStepR, const Range2D &yRange, const Texture2D &Tex, const float Slopes[ 2 ] )
{
	const float yPreStep = float( yRange.start ) - TopLeft.position.y;

	// calculate uv edge prestep
	Vec2f tEdgeL = TopLeft.texcoord + ( EdgeStepL.texcoord * yPreStep );
	Vec2f tEdgeR = TopRight.texcoord + ( EdgeStepR.texcoord * yPreStep );

	// calculate color edge prestep
	ColorF cEdgeL = TopLeft.tint + EdgeStepL.tint * yPreStep;
	ColorF cEdgeR = TopRight.tint + EdgeStepR.tint * yPreStep;

	auto pPixels = reinterpret_cast< Color const* const >( Tex.GetPixels() );
	const auto tWidth = Tex.GetWidth();
	const auto tHeight = Tex.GetHeight();
	const float uRange = float( tWidth - 1 );
	const float vRange = float( tHeight - 1 );
	const float height = float( yRange.end - yRange.start );

	for( int y = yRange.start; y <= yRange.end;
		 y++,
		 tEdgeL += EdgeStepL.texcoord,
		 tEdgeR += EdgeStepR.texcoord,
		 cEdgeL += EdgeStepL.tint,
		 cEdgeR += EdgeStepR.tint )
	{
		const float f_y = float( y );
		// caluclate start and end points
		const float px0 = Slopes[0] * ( f_y - TopLeft.position.y ) + TopLeft.position.x;
		const float px1 = Slopes[1] * ( f_y - TopRight.position.y ) + TopRight.position.x;
		const float width = px1 - px0;
		const float recWidth = 1.f / width;

		// calculate start and end pixels
		Range2D xRange{
			std::max( ( int )ceilf( px0 ), 0 ),
			std::min( ( int )ceilf( px1 ) - 1, m_screenWidth )
		};
		const float xPreStep = ( float( xRange.start ) - px0 );

		// calculate uv scanline step
		const Vec2f tScanStep = ( tEdgeR - tEdgeL ) * recWidth;

		// calculate color scan step
		const ColorF cScanStep = ( cEdgeR - cEdgeL ) * recWidth;

		// calculate uv point prestep
		Vec2f t = tEdgeL + tScanStep * xPreStep;

		// calculate color gradient
		ColorF c = cEdgeL + cScanStep * xPreStep;

		for( int x = xRange.start, i = 0; x <= xRange.end; x++, t += tScanStep, c += cScanStep, ++i )
		{			
			const auto tx = static_cast<unsigned>( ( t.x * uRange ) );
			const auto ty = static_cast<unsigned>( ( t.y * vRange ) );
			const Color i_c = pPixels[ tx + ( ty * tWidth ) ] * ColorsF::ToColor( c );
			PutPixel( x, y, i_c );
		}
	}
}
// End private functions