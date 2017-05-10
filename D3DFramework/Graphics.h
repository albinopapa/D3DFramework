#pragma once

#include <d3d11.h>
#include <wrl/client.h>
#include <memory>
#include "Direct3D.h"
#include "Color.h"
#include "Mat3.h"
#include "Model2D.h"
#include "Point2D.h"
#include "Rect.h"
#include "Vertex2D.h"


using Microsoft::WRL::ComPtr;

class Texture2D;
class Window;

constexpr float g_backgroundcolor[]{ .2f, .15f, .1f, 1.f };
namespace IA = Pipeline::InputAssembler;
namespace RS = Pipeline::Rasterizer;
class Graphics
{
public:
	Graphics(Window &Win, Direct3D &D3D);
	~Graphics();

	void BeginFrame( Color FillColor );
	void EndFrame( );

	void PutPixel( int X, int Y, Color Color );
	void PutPixel( const Point2D &P, Color C );
	
	void DrawLine( int X0, int Y0, int X1, int Y1, Color Color );
	void DrawLine( const Point2D &Start, const Point2D &End, Color C );

	void DrawBoxGradient( const Recti &Box, const float Value[4] );
	void DrawBoxFilled( const Recti &Box, Color Color );
	void DrawCircleFilled( int X, int Y, int Radius, Color Color );
	void DrawCircleFilled( const Point2D &Center, int Radius, Color C );

	void DrawBoxOutline( const Recti &Box, Color Color );
	void DrawCircleOutline( int X, int Y, int InnerRadius, int OuterRadius, Color Color );
	void DrawCircleOutline( const Point2D &Center, int InnerRadius, int OuterRadius, Color C );
		
	void DrawTriangle( const Vertex2D &p0, const Vertex2D &p1, const Vertex2D &p2 );
	void DrawTriangle( const Vertex2D &p0, const Vertex2D &p1, const Vertex2D &p2, const Texture2D &Tex );

	void DrawModel2D( const Model2D &Mod, const Mat3 &Transform );
	void DrawModel2D( const Model2D &Mod, const Texture2D &Tex, const Mat3 &Transform );

private:
	void PrepFlatTopTriangle( const Vertex2D &p0, const Vertex2D& p1, const Vertex2D& p2 );
	void PrepFlatTopTriangle( const Vertex2D &p0, const Vertex2D& p1, const Vertex2D& p2, const Texture2D &Tex );
	void PrepFlatBottomTriangle( const Vertex2D &p0, const Vertex2D& p1, const Vertex2D& p2 );
	void PrepFlatBottomTriangle( const Vertex2D &p0, const Vertex2D& p1, const Vertex2D& p2, const Texture2D &Tex );

	void DrawFlatTriangle( const Vertex2D &TopLeft, const Vertex2D &TopRight, const Vertex2D &EdgeStepL, const Vertex2D &EdgeStepR, const Range2D &StartEnd, const float Slopes[2] );
	void DrawFlatTriangle( const Vertex2D &TopLeft, const Vertex2D &TopRight, const Vertex2D &EdgeStepL, const Vertex2D &EdgeStepR, const Range2D &StartEnd, const Texture2D &Tex, const float Slopes[2] );
private:
	Direct3D &m_Direct3D;

	ComPtr<ID3D11Texture2D>					m_pSysTexture;
	ComPtr<ID3D11ShaderResourceView>		m_pShaderResourceView;
	ComPtr<ID3D11SamplerState>				m_pSamplerState;

	std::unique_ptr<Color[]> m_pSysbuffer;
	
	IA::PrimitiveTopology m_topology;
	IA::VertexBuffers m_vertexBuffers;
	IA::IndexBuffer m_indexBuffer;
	IA::InputLayout m_inputLayout;
	Pipeline::VertexShader m_vertexShader;
	Pipeline::PixelShader m_pixelShader;
	RS::Viewports m_viewport;
public:
	static int m_screenWidth;
	static int m_screenHeight;
};

