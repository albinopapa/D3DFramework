#include "Direct3D.h"
#include "Window.h"

#include <string>
#include <sstream>


Direct3D::Direct3D( Window &Win )
{
	const auto screenWidth = static_cast<UINT>( Win.GetWidth() );
	const auto screenHeight = static_cast<UINT>( Win.GetHeight() );

	// Pre-setup
	D3D11_CREATE_DEVICE_FLAG flags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
#ifdef DEBUG
	flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	const auto bufferDesc = DXGI_MODE_DESC{
		screenWidth,							// Width
		screenHeight,							// Height
		DXGI_RATIONAL{ 60, 1 },					// Refresh rate
		DXGI_FORMAT_R8G8B8A8_UNORM,				// Buffer format
		DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED,	// Scanline order
		DXGI_MODE_SCALING_UNSPECIFIED };		// Scaling

	DXGI_SWAP_CHAIN_DESC scd{
		bufferDesc,								// Buffer description
		DXGI_SAMPLE_DESC{ 1, 0 },				// Count, Quality
		DXGI_USAGE_RENDER_TARGET_OUTPUT,		// Resource usage
		1,										// buffer count
		Win.GetHandle(),						// Window handle
		TRUE,									// Windowed?
		DXGI_SWAP_EFFECT_DISCARD,				// Swap effect
		DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH	// Flags
	};

	HRESULT hr = D3D11CreateDeviceAndSwapChain(
		nullptr,								// IDXGIAdapter *
		D3D_DRIVER_TYPE_HARDWARE,				// Driver type
		nullptr,								// Software driver implementation
		flags,									// Creation flags (BGRA support and Debug are defaults)
		nullptr,								// Feature level list, nullptr for default array
		0,										// Feature level list count
		D3D11_SDK_VERSION,						// SDK version
		&scd,									// Address of swapchain description
		&m_pSwapchain,							// Address of swapchain interface
		&m_pDevice,								// Address of D3D11 device interface
		&m_featurelevel,						// Address of returned feature level
		&m_pContext								// Address of D3D11 device context interface
	);

	if ( FAILED( hr ) )
	{
		MessageBox( nullptr, L"Failed to create device, context and/or swapchain.", L"Device creation error.", MB_OK );
		throw( hr );
	}

	ComPtr<ID3D11Texture2D>	pBackbuffer;
	// Initialize the back buffer
	hr = m_pSwapchain->GetBuffer( 0, IID_PPV_ARGS( &pBackbuffer ) );
	if ( FAILED( hr ) )
	{
		MessageBox( nullptr, L"Failed to get the back buffer.", L"Back buffer initialization error.", MB_OK );
		throw( hr );
	}

	// Create and set the render target using the back buffer
	hr = m_pDevice->CreateRenderTargetView( pBackbuffer.Get(), nullptr, &m_pRenderTarget );
	if ( FAILED( hr ) )
	{
		MessageBox( nullptr, L"Failed to create render target.", L"RenderTargetView creation error.", MB_OK );
		throw( hr );
	}
}

Direct3D::~Direct3D()
{}

ID3D11Device * Direct3D::GetDevice() const
{
	return m_pDevice.Get();
}

ID3D11DeviceContext * Direct3D::GetContext() const
{	
	return m_pContext.Get();
}

ID3D11RenderTargetView * Direct3D::GetRenderTarget() const
{
	return m_pRenderTarget.Get();
}

IDXGISwapChain * Direct3D::GetSwapchain() const
{
	return m_pSwapchain.Get();
}

D3D_FEATURE_LEVEL Direct3D::GetFeatureLevel() const
{
	return m_featurelevel;
}

// Render pipeline functions

Pipeline::InputAssembler::PrimitiveTopology::PrimitiveTopology( ID3D11DeviceContext * pContext, D3D_PRIMITIVE_TOPOLOGY Topology )
	:
	m_pContext( pContext ),
	m_topology( Topology )
{}

void Pipeline::InputAssembler::PrimitiveTopology::Set()
{	
	m_pContext->IASetPrimitiveTopology( m_topology );
}

void Pipeline::InputAssembler::VertexBuffers::AddBuffer( ID3D11Buffer * pBuffer )
{
	using Microsoft::WRL::ComPtr;
	m_ppBuffers.push_back( ComPtr<ID3D11Buffer>(pBuffer) );
}

Pipeline::InputAssembler::VertexBuffers::VertexBuffers( const Direct3D & D3D )
	:
	m_pDevice( D3D.GetDevice() ),
	m_pContext( D3D.GetContext() )
{}

void Pipeline::InputAssembler::VertexBuffers::Set()
{
	const UINT offset = 0u;
	const auto buffer_size = static_cast< UINT >( m_ppBuffers.size() );
	m_pContext->IASetVertexBuffers( 0, buffer_size, 
									m_ppBuffers.data()->GetAddressOf(), 
									&m_stride, &offset );
}

void Pipeline::InputAssembler::IndexBuffer::Set( UINT Offset, DXGI_FORMAT Format )
{
	m_pContext->IASetIndexBuffer( m_pBuffer.Get(), Format, Offset );
}

Pipeline::InputAssembler::IndexBuffer::IndexBuffer( const Direct3D & D3D )
	:
	m_pDevice( D3D.GetDevice() ),
	m_pContext( D3D.GetContext() )
{}

HRESULT Pipeline::InputAssembler::IndexBuffer::Create(
	const std::vector<UINT>& IndexData, D3D11_USAGE Usage,
	D3D11_BIND_FLAG Flags, UINT CpuAccess, UINT MiscFlags,
	UINT StructureByteStride )
{
	D3D11_BUFFER_DESC bd{
		sizeof( UINT ) * static_cast<UINT>( IndexData.size() ),
		Usage,
		static_cast<UINT>( Flags ),
		CpuAccess,
		MiscFlags,
		StructureByteStride
	};
	D3D11_SUBRESOURCE_DATA data{ IndexData.data(),0u,0u };
	D3D11_SUBRESOURCE_DATA *pData = IndexData.empty() ? nullptr : &data;

	return m_pDevice->CreateBuffer( &bd, pData, &m_pBuffer );	
}

Pipeline::InputAssembler::InputLayout::InputLayout( const Direct3D & D3D )
	:
	m_pDevice( D3D.GetDevice() ),
	m_pContext( D3D.GetContext() )
{}

HRESULT Pipeline::InputAssembler::InputLayout::Create( 
	ID3DBlob *pVShaderBlob, std::vector<InputElementDesc> &&InputElementDesc )
{
	HRESULT hr = S_OK;
	
	hr = m_pDevice->CreateInputLayout( 
		InputElementDesc.data(), 
		static_cast<UINT>( InputElementDesc.size() ),
		pVShaderBlob->GetBufferPointer(), 
		pVShaderBlob->GetBufferSize(),
		&m_pLayout );

	return hr;
}

void Pipeline::InputAssembler::InputLayout::Set()
{
	m_pContext->IASetInputLayout( m_pLayout.Get() );
}

Pipeline::InputAssembler::InputLayout::InputElementDesc::InputElementDesc(
	LPCSTR SemanticName, UINT SemanticIndex, DXGI_FORMAT Format,
	UINT InputSlot, UINT AlignedByteOffset )
{
	this->SemanticName = SemanticName;
	this->SemanticIndex = SemanticIndex;
	this->Format = Format;
	this->InputSlot = InputSlot;
	this->AlignedByteOffset = AlignedByteOffset;
	this->InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	this->InstanceDataStepRate = 0u;
}

Pipeline::InputAssembler::InputLayout::InputElementDesc::InputElementDesc( 
	LPCSTR SemanticName, UINT SemanticIndex, DXGI_FORMAT Format,
	UINT InputSlot, UINT AlignedByteOffset, UINT InstanceDataStepRate )
{
	this->SemanticName = SemanticName;
	this->SemanticIndex = SemanticIndex;
	this->Format = Format;
	this->InputSlot = InputSlot;
	this->AlignedByteOffset = AlignedByteOffset;
	this->InputSlotClass = D3D11_INPUT_PER_INSTANCE_DATA;
	this->InstanceDataStepRate = InstanceDataStepRate;
}

Pipeline::InputAssembler::InputLayout::InputElementDesc::operator D3D11_INPUT_ELEMENT_DESC()
{
	return static_cast< D3D11_INPUT_ELEMENT_DESC >( *this );
}

Pipeline::VertexShader::VertexShader( const Direct3D & D3D )
	:
	m_pDevice( D3D.GetDevice() ),
	m_pContext( D3D.GetContext() )
{}

HRESULT Pipeline::VertexShader::Create( const std::wstring & Filename, ID3DBlob ** ppShaderBlob )
{
	ID3DBlob *pBlob = nullptr;
	HRESULT hr = D3DReadFileToBlob( Filename.c_str(), &pBlob );
	if( FAILED( hr ) ) return hr;

	hr = m_pDevice->CreateVertexShader(
		pBlob->GetBufferPointer(),
		pBlob->GetBufferSize(),
		nullptr,
		&m_pShader
	);

	*ppShaderBlob = pBlob;
	return hr;
}

void Pipeline::VertexShader::Set()
{
	m_pContext->VSSetShader( m_pShader.Get(), nullptr, 0 );
}

Pipeline::PixelShader::PixelShader( const Direct3D & D3D )
	:
	m_pDevice( D3D.GetDevice() ),
	m_pContext( D3D.GetContext() )
{}

HRESULT Pipeline::PixelShader::Create( const std::wstring & Filename )
{
	ID3DBlob *pBlob = nullptr;
	HRESULT hr = D3DReadFileToBlob( Filename.c_str(), &pBlob );
	if( FAILED( hr ) ) return hr;

	hr = m_pDevice->CreatePixelShader(
		pBlob->GetBufferPointer(),
		pBlob->GetBufferSize(),
		nullptr,
		&m_pShader
	);

	return hr;
}

void Pipeline::PixelShader::Set()
{
	m_pContext->PSSetShader( m_pShader.Get(), nullptr, 0 );
}

Pipeline::Rasterizer::Viewports::Viewports(
	const Direct3D &D3D )
	:
	m_pContext( D3D.GetContext() )
{}

void Pipeline::Rasterizer::Viewports::Create( 
	float Left, float Top, float Width, float Height, 
	float MinDepth, float MaxDepth )
{
	m_viewport = {
		Left, Top,
		Width, Height,
		MinDepth, MaxDepth
	};
}

void Pipeline::Rasterizer::Viewports::Set()
{
	m_pContext->RSSetViewports( 1u, &m_viewport );
}
