#pragma once

#include <vector>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <wrl\client.h>
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")

// Shorthand for using namespace
using Microsoft::WRL::ComPtr;

class Direct3D
{
public:
	Direct3D( class Window &Win );
	~Direct3D();

	ID3D11Device *GetDevice()const;
	ID3D11DeviceContext *GetContext()const;
	ID3D11RenderTargetView *GetRenderTarget()const;
	IDXGISwapChain *GetSwapchain()const;
	D3D_FEATURE_LEVEL GetFeatureLevel()const;

private:

	// D3D11 interfaces
	ComPtr<ID3D11Device>					m_pDevice;
	ComPtr<ID3D11DeviceContext>				m_pContext;
	ComPtr<IDXGISwapChain>					m_pSwapchain;
	ComPtr<ID3D11RenderTargetView>			m_pRenderTarget;
	D3D_FEATURE_LEVEL						m_featurelevel{};
};

namespace Pipeline
{
	namespace InputAssembler
	{
		class PrimitiveTopology
		{
			ID3D11DeviceContext		*m_pContext;
			D3D_PRIMITIVE_TOPOLOGY	 m_topology;
		public:
			PrimitiveTopology( ID3D11DeviceContext *pContext, D3D_PRIMITIVE_TOPOLOGY Topology );
			void Set();
			static constexpr D3D11_PRIMITIVE_TOPOLOGY Undefined = D3D_PRIMITIVE_TOPOLOGY_UNDEFINED;

			// Tells the pipeline to render the list of points as points
			static constexpr D3D11_PRIMITIVE_TOPOLOGY PointList = D3D_PRIMITIVE_TOPOLOGY_POINTLIST;
			// Tells the pipeline to render the list of points as lines
			static constexpr D3D11_PRIMITIVE_TOPOLOGY LineList = D3D_PRIMITIVE_TOPOLOGY_LINELIST;
			// Tells the pipeline to render the list of points as connected lines
			static constexpr D3D11_PRIMITIVE_TOPOLOGY LineStrip = D3D_PRIMITIVE_TOPOLOGY_LINESTRIP;
			// Tells the pipeline to render the list of points as triangles
			static constexpr D3D11_PRIMITIVE_TOPOLOGY TriangleList = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
			// Tells the pipeline to render the list of points as connected triangles
			static constexpr D3D11_PRIMITIVE_TOPOLOGY TriangleStrip = D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;

			// Adjacent points are only accessible from geometry shaders

			// Tells the pipeline to render the list of points as lines
			static constexpr D3D11_PRIMITIVE_TOPOLOGY LineList_Adj = D3D_PRIMITIVE_TOPOLOGY_LINELIST_ADJ;
			// Tells the pipeline to render the list of points as connected lines
			static constexpr D3D11_PRIMITIVE_TOPOLOGY LineStrip_Adj = D3D_PRIMITIVE_TOPOLOGY_LINESTRIP_ADJ;
			// Tells the pipeline to render the list of points as triangles
			static constexpr D3D11_PRIMITIVE_TOPOLOGY TriangleList_Adj = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST_ADJ;
			// Tells the pipeline to render the list of points as connected triangles
			static constexpr D3D11_PRIMITIVE_TOPOLOGY TriangleStrip_Adj = D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP_ADJ;

			// TODO: Verify
			// These 32 control point patch lists are for use with domain shaders

			static constexpr D3D11_PRIMITIVE_TOPOLOGY ControlPointPatchList_1 = D3D_PRIMITIVE_TOPOLOGY_1_CONTROL_POINT_PATCHLIST;
			static constexpr D3D11_PRIMITIVE_TOPOLOGY ControlPointPatchList_2 = D3D_PRIMITIVE_TOPOLOGY_2_CONTROL_POINT_PATCHLIST;
			static constexpr D3D11_PRIMITIVE_TOPOLOGY ControlPointPatchList_3 = D3D_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST;
			static constexpr D3D11_PRIMITIVE_TOPOLOGY ControlPointPatchList_4 = D3D_PRIMITIVE_TOPOLOGY_4_CONTROL_POINT_PATCHLIST;
			static constexpr D3D11_PRIMITIVE_TOPOLOGY ControlPointPatchList_5 = D3D_PRIMITIVE_TOPOLOGY_5_CONTROL_POINT_PATCHLIST;
			static constexpr D3D11_PRIMITIVE_TOPOLOGY ControlPointPatchList_6 = D3D_PRIMITIVE_TOPOLOGY_6_CONTROL_POINT_PATCHLIST;
			static constexpr D3D11_PRIMITIVE_TOPOLOGY ControlPointPatchList_7 = D3D_PRIMITIVE_TOPOLOGY_7_CONTROL_POINT_PATCHLIST;
			static constexpr D3D11_PRIMITIVE_TOPOLOGY ControlPointPatchList_8 = D3D_PRIMITIVE_TOPOLOGY_8_CONTROL_POINT_PATCHLIST;
			static constexpr D3D11_PRIMITIVE_TOPOLOGY ControlPointPatchList_9 = D3D_PRIMITIVE_TOPOLOGY_9_CONTROL_POINT_PATCHLIST;
			static constexpr D3D11_PRIMITIVE_TOPOLOGY ControlPointPatchList_10 = D3D_PRIMITIVE_TOPOLOGY_10_CONTROL_POINT_PATCHLIST;
			static constexpr D3D11_PRIMITIVE_TOPOLOGY ControlPointPatchList_11 = D3D_PRIMITIVE_TOPOLOGY_11_CONTROL_POINT_PATCHLIST;
			static constexpr D3D11_PRIMITIVE_TOPOLOGY ControlPointPatchList_12 = D3D_PRIMITIVE_TOPOLOGY_12_CONTROL_POINT_PATCHLIST;
			static constexpr D3D11_PRIMITIVE_TOPOLOGY ControlPointPatchList_13 = D3D_PRIMITIVE_TOPOLOGY_13_CONTROL_POINT_PATCHLIST;
			static constexpr D3D11_PRIMITIVE_TOPOLOGY ControlPointPatchList_14 = D3D_PRIMITIVE_TOPOLOGY_14_CONTROL_POINT_PATCHLIST;
			static constexpr D3D11_PRIMITIVE_TOPOLOGY ControlPointPatchList_15 = D3D_PRIMITIVE_TOPOLOGY_15_CONTROL_POINT_PATCHLIST;
			static constexpr D3D11_PRIMITIVE_TOPOLOGY ControlPointPatchList_16 = D3D_PRIMITIVE_TOPOLOGY_16_CONTROL_POINT_PATCHLIST;
			static constexpr D3D11_PRIMITIVE_TOPOLOGY ControlPointPatchList_17 = D3D_PRIMITIVE_TOPOLOGY_17_CONTROL_POINT_PATCHLIST;
			static constexpr D3D11_PRIMITIVE_TOPOLOGY ControlPointPatchList_18 = D3D_PRIMITIVE_TOPOLOGY_18_CONTROL_POINT_PATCHLIST;
			static constexpr D3D11_PRIMITIVE_TOPOLOGY ControlPointPatchList_19 = D3D_PRIMITIVE_TOPOLOGY_19_CONTROL_POINT_PATCHLIST;
			static constexpr D3D11_PRIMITIVE_TOPOLOGY ControlPointPatchList_20 = D3D_PRIMITIVE_TOPOLOGY_20_CONTROL_POINT_PATCHLIST;
			static constexpr D3D11_PRIMITIVE_TOPOLOGY ControlPointPatchList_21 = D3D_PRIMITIVE_TOPOLOGY_21_CONTROL_POINT_PATCHLIST;
			static constexpr D3D11_PRIMITIVE_TOPOLOGY ControlPointPatchList_22 = D3D_PRIMITIVE_TOPOLOGY_22_CONTROL_POINT_PATCHLIST;
			static constexpr D3D11_PRIMITIVE_TOPOLOGY ControlPointPatchList_23 = D3D_PRIMITIVE_TOPOLOGY_23_CONTROL_POINT_PATCHLIST;
			static constexpr D3D11_PRIMITIVE_TOPOLOGY ControlPointPatchList_24 = D3D_PRIMITIVE_TOPOLOGY_24_CONTROL_POINT_PATCHLIST;
			static constexpr D3D11_PRIMITIVE_TOPOLOGY ControlPointPatchList_25 = D3D_PRIMITIVE_TOPOLOGY_25_CONTROL_POINT_PATCHLIST;
			static constexpr D3D11_PRIMITIVE_TOPOLOGY ControlPointPatchList_26 = D3D_PRIMITIVE_TOPOLOGY_26_CONTROL_POINT_PATCHLIST;
			static constexpr D3D11_PRIMITIVE_TOPOLOGY ControlPointPatchList_27 = D3D_PRIMITIVE_TOPOLOGY_27_CONTROL_POINT_PATCHLIST;
			static constexpr D3D11_PRIMITIVE_TOPOLOGY ControlPointPatchList_28 = D3D_PRIMITIVE_TOPOLOGY_28_CONTROL_POINT_PATCHLIST;
			static constexpr D3D11_PRIMITIVE_TOPOLOGY ControlPointPatchList_29 = D3D_PRIMITIVE_TOPOLOGY_29_CONTROL_POINT_PATCHLIST;
			static constexpr D3D11_PRIMITIVE_TOPOLOGY ControlPointPatchList_30 = D3D_PRIMITIVE_TOPOLOGY_30_CONTROL_POINT_PATCHLIST;
			static constexpr D3D11_PRIMITIVE_TOPOLOGY ControlPointPatchList_31 = D3D_PRIMITIVE_TOPOLOGY_31_CONTROL_POINT_PATCHLIST;
			static constexpr D3D11_PRIMITIVE_TOPOLOGY ControlPointPatchList_32 = D3D_PRIMITIVE_TOPOLOGY_32_CONTROL_POINT_PATCHLIST;

		};
		class VertexBuffers
		{
			void AddBuffer( ID3D11Buffer *pBuffer );

			ID3D11Device *m_pDevice;
			ID3D11DeviceContext *m_pContext;
			std::vector<Microsoft::WRL::ComPtr<ID3D11Buffer>> m_ppBuffers;
			UINT m_stride = 0;
		public:
			VertexBuffers( const Direct3D &D3D );

			// Creates a new vertex buffer and adds it to the list
			template<class ContType>
			HRESULT Create( 
				const std::vector<ContType> &VertexData, 
				D3D11_USAGE Usage = D3D11_USAGE_DEFAULT, 
				D3D11_BIND_FLAG Flags = D3D11_BIND_VERTEX_BUFFER, 
				UINT CpuAccess = 0U, UINT MiscFlags = 0U, 
				UINT StructureByteStride = 0U )
			{
				m_stride = sizeof( ContType );
				D3D11_BUFFER_DESC bd{
					m_stride * static_cast<UINT>( VertexData.size() ),
					Usage,
					static_cast<UINT>( Flags ),
					CpuAccess,
					MiscFlags,
					StructureByteStride
				};

				D3D11_SUBRESOURCE_DATA data;
				D3D11_SUBRESOURCE_DATA *pData = [ &VertexData, &data ]()
				{
					if( VertexData.empty() )
						return reinterpret_cast< D3D11_SUBRESOURCE_DATA* >( nullptr );
					else
						data = { VertexData.data(), 0u, 0u };
					return &data;
				}( );
				ID3D11Buffer *pBuffer = nullptr;
				HRESULT hr = m_pDevice->CreateBuffer( &bd, pData, &pBuffer );
				if( FAILED( hr ) ) return hr;

				AddBuffer( pBuffer );
				return hr;
			}
			void Set();
		};
		class IndexBuffer
		{
			ID3D11Device *m_pDevice;
			ID3D11DeviceContext *m_pContext;
			Microsoft::WRL::ComPtr<ID3D11Buffer> m_pBuffer;
		public:
			IndexBuffer( const Direct3D &D3D );
			HRESULT Create( 
				const std::vector<UINT> &IndexData, 
				D3D11_USAGE Usage = D3D11_USAGE_DEFAULT, 
				D3D11_BIND_FLAG Flags = D3D11_BIND_VERTEX_BUFFER, 
				UINT CpuAccess = 0U, UINT MiscFlags = 0U, 
				UINT StructureByteStride = 0U );
			void Set( UINT Offset = 0u, DXGI_FORMAT Format = DXGI_FORMAT_R32_UINT );
		};
		class InputLayout
		{
			ID3D11Device *m_pDevice;
			ID3D11DeviceContext *m_pContext;
			Microsoft::WRL::ComPtr<ID3D11InputLayout> m_pLayout;
		public:
			struct InputElementDesc:D3D11_INPUT_ELEMENT_DESC
			{
				InputElementDesc(
					LPCSTR SemanticName,
					UINT SemanticIndex,
					DXGI_FORMAT Format,
					UINT InputSlot,
					UINT AlignedByteOffset
				);
				InputElementDesc(
					LPCSTR SemanticName,
					UINT SemanticIndex,
					DXGI_FORMAT Format,
					UINT InputSlot,
					UINT AlignedByteOffset,
					UINT InstanceDataStepRate
				);
				operator D3D11_INPUT_ELEMENT_DESC();
			};
			InputLayout( const Direct3D &D3D );
			HRESULT Create( ID3DBlob *pVShaderBlob, 
							std::vector<InputElementDesc> &&InputElementDesc );
			void Set();
		};
	}
	class VertexShader
	{
		ID3D11Device *m_pDevice;
		ID3D11DeviceContext *m_pContext;
		Microsoft::WRL::ComPtr<ID3D11VertexShader> m_pShader;
	public:
		VertexShader( const Direct3D &D3D );
		HRESULT Create( const std::wstring &Filename, ID3DBlob **ppShaderBlob );
		void Set();
	};
	class GeometryShader
	{

	};
	class DomainShader
	{

	};
	class ComputeShader
	{

	};
	class PixelShader
	{
		ID3D11Device *m_pDevice;
		ID3D11DeviceContext *m_pContext;
		Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pShader;
	public:
		PixelShader( const Direct3D &D3D );
		HRESULT Create( const std::wstring &Filename );
		void Set();
	};
	namespace OutputMerger
	{
		class RenderTargetViews
		{
			ID3D11Device *m_pDevice;
			ID3D11DeviceContext *m_pContext;
			Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_pRenderTargets;
		public:
			RenderTargetViews( const Direct3D &D3D );
			HRESULT Create();
			void Set();
		};
	}
	namespace Rasterizer
	{
		class Viewports
		{
			ID3D11DeviceContext *m_pContext;
			D3D11_VIEWPORT m_viewport;
		public:
			Viewports( const Direct3D &D3D );
			void Create( float Left, float Top, float Width, float Height, float MinDepth, float MaxDepth );
			void Set();
		};
	}
	namespace StreamOutput
	{

	}

	namespace Utils
	{
		template<class ResourceType>
		class MappedResource 
		{
		};
		template<>class MappedResource<ID3D11Texture2D>
		{
			ID3D11DeviceContext *m_pContext;
			ID3D11Texture2D *m_pTexture;
			D3D11_MAPPED_SUBRESOURCE ms;
			HRESULT hr;
		public:
			MappedResource( const Direct3D &D3D, ID3D11Texture2D *pResource, UINT SubResource = 0u, D3D11_MAP RWAccess = D3D11_MAP_WRITE_DISCARD, UINT MapFlag = 0u )
				:
				m_pContext( D3D.GetContext() ),
				m_pTexture( pResource ),
				hr( m_pContext->Map( pResource, SubResource, RWAccess, MapFlag, &ms ) )
			{}
			HRESULT Update( const BYTE *pPixelData, UINT Width, UINT Height )
			{
				if( SUCCEEDED( hr ) )
				{
					const UINT pitch = Width * sizeof( unsigned );
					BYTE *vidMem = reinterpret_cast< BYTE* >( ms.pData );

					for( auto y = 0u; y < Height; ++y )
					{
						const unsigned int index = y * pitch;
						CopyMemory( &vidMem[ index ], &pPixelData[ index ], pitch );
					}
				}

				return hr;
			}
			~MappedResource()
			{
				m_pContext->Unmap( m_pTexture, 0 );
			}
		};
		template<>class MappedResource<ID3D11Buffer>
		{
			ID3D11DeviceContext *m_pContext;
			ID3D11Buffer *m_pBuffer;
			D3D11_MAPPED_SUBRESOURCE ms;
			HRESULT hr;
		public:
			MappedResource( const Direct3D &D3D, ID3D11Buffer *pResource, UINT SubResource = 0u, D3D11_MAP RWAccess = D3D11_MAP_WRITE_DISCARD, UINT MapFlag = 0u )
				:
				m_pContext( D3D.GetContext() ),
				m_pBuffer( pResource ),
				hr( m_pContext->Map( pResource, SubResource, RWAccess, MapFlag, &ms ) )
			{}

			template<class DataType>
			HRESULT Update( DataType *pData )
			{
				if( SUCCEEDED( hr ) )
				{
					const unsigned size = sizeof( DataType );
					CopyMemory( ms.pData, pData, size );
				}

				return hr;
			}
			~MappedResource()
			{
				m_pContext->Unmap( m_pBuffer, 0 );
			}
		};
	}
}
