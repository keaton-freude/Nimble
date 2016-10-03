#pragma once

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "dxgi.lib")

#include <DXGI.h>
#include <memory>
#include <D3Dcommon.h>
#include <D3D11.h>
#include <SimpleMath.h>
#include <wrl\client.h>
#include "Logger.h"
#include "Helper.h"

using Microsoft::WRL::ComPtr;
using std::shared_ptr;
using DirectX::SimpleMath::Matrix;
using DirectX::SimpleMath::Color;
using DirectX::XM_PIDIV4;

class NimbleD3D
{
private:
	bool m_vsync_enabled;
	int m_videoCardMemory;
	char m_videoCardDescription[128];
	HWND m_hwnd;
	float fieldOfView;

	ComPtr<IDXGISwapChain> m_swapChain;
	DXGI_SWAP_CHAIN_DESC m_swapChainDesc;

	ComPtr<ID3D11Device> m_device;
	ComPtr<ID3D11DeviceContext> m_deviceContext;
	ComPtr<ID3D11RenderTargetView> m_renderTargetView;
	ComPtr<ID3D11Texture2D> m_depthStencilBuffer;
	ComPtr<ID3D11DepthStencilState> m_depthStencilState;
	ComPtr<ID3D11DepthStencilView> m_depthStencilView;
	ComPtr<ID3D11RasterizerState> m_rasterState;
	ComPtr<ID3D11Debug> m_debugLayer;

	Matrix m_projectionMatrix;
	Matrix m_worldMatrix;
public:
	NimbleD3D()
		: m_vsync_enabled(false), m_videoCardMemory(0), m_hwnd(nullptr), m_projectionMatrix(), m_worldMatrix()
	{
		fieldOfView = XM_PIDIV4;
	}

	~NimbleD3D()
	{
		// Before shutting down set to windowed mode or when you release the swap chain it will throw an exception.
		m_swapChain->SetFullscreenState(false, nullptr);

		LOG_INFO("NimbleD3D Destruct!");
	}

	bool Init(int screenWidth, int screenHeight, bool vsync, HWND hwnd, 
		bool fullscreen, float screenDepth, float screenNear)
	{
		HRESULT result;
		D3D_FEATURE_LEVEL featureLevel;
		unsigned int numModes, i, numerator = 0, denominator = 1;
		int error;
		float fieldOfView, screenAspect;
		size_t stringLength;

		ComPtr<IDXGIFactory> factory;
		ComPtr<IDXGIAdapter> adapter;
		ComPtr<IDXGIOutput> adapterOutput;
		shared_ptr<DXGI_MODE_DESC> displayModeList;
		ComPtr<ID3D11Texture2D> backBufferPtr;

		DXGI_ADAPTER_DESC adapterDesc;
		D3D11_TEXTURE2D_DESC depthBufferDesc;
		D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
		D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
		D3D11_RASTERIZER_DESC rasterDesc;
		D3D11_VIEWPORT viewport;



		// Store the vsync setting.
		m_vsync_enabled = vsync;

		// Create a DirectX graphics interface factory.
		result = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory);

		if (FAILED(result))
		{
			return false;
		}

		// Use the factory to create an adapter for the primary graphics interface (video card).
		result = factory->EnumAdapters(0, adapter.GetAddressOf());
		if (FAILED(result))
		{
			return false;
		}

		// Enumerate the primary adapter output (monitor).
		result = adapter->EnumOutputs(0, adapterOutput.GetAddressOf());
		if (FAILED(result))
		{
			return false;
		}

		// Get the number of modes that fit the DXGI_FORMAT_R8G8B8A8_UNORM display format for the adapter output (monitor).
		result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, NULL);
		if (FAILED(result))
		{
			return false;
		}

		// Create a list to hold all the possible display modes for this monitor/video card combination.
		displayModeList = shared_ptr<DXGI_MODE_DESC>(new DXGI_MODE_DESC[numModes]);

		// Now fill the display mode list structures.
		result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, displayModeList.get());
		if (FAILED(result))
		{
			return false;
		}

		// Now go through all the display modes and find the one that matches the screen width and height.
		// When a match is found store the numerator and denominator of the refresh rate for that monitor.
		for (i = 0; i < numModes; i++)
		{
			if (displayModeList.get()[i].Width == (unsigned int)screenWidth)
			{
				if (displayModeList.get()[i].Height == (unsigned int)screenHeight)
				{
					numerator = displayModeList.get()[i].RefreshRate.Numerator;
					denominator = displayModeList.get()[i].RefreshRate.Denominator;
				}
			}
		}

		// Get the adapter (video card) description.
		result = adapter->GetDesc(&adapterDesc);
		if (FAILED(result))
		{
			return false;
		}

		// Store the dedicated video card memory in megabytes.
		m_videoCardMemory = (int)(adapterDesc.DedicatedVideoMemory / 1024 / 1024);

		// Convert the name of the video card to a character array and store it.
		error = wcstombs_s(&stringLength, m_videoCardDescription, 128, adapterDesc.Description, 128);
		if (error != 0)
		{
			return false;
		}

		// Initialize the swap chain description.
		ZeroMemory(&m_swapChainDesc, sizeof(m_swapChainDesc));

		// Set to a single back buffer.
		m_swapChainDesc.BufferCount = 1;

		// Set the width and height of the back buffer.
		m_swapChainDesc.BufferDesc.Width = screenWidth;
		m_swapChainDesc.BufferDesc.Height = screenHeight;

		// Set regular 32-bit surface for the back buffer.
		m_swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

		// Set the refresh rate of the back buffer.
		if (m_vsync_enabled)
		{
			m_swapChainDesc.BufferDesc.RefreshRate.Numerator = numerator;
			m_swapChainDesc.BufferDesc.RefreshRate.Denominator = denominator;
		}
		else
		{
			m_swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
			m_swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
		}

		// Set the usage of the back buffer.
		m_swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

		// Set the handle for the window to render to.
		m_swapChainDesc.OutputWindow = hwnd;

		// Turn multisampling off.
		m_swapChainDesc.SampleDesc.Count = 1;
		m_swapChainDesc.SampleDesc.Quality = 0;

		// Set to full screen or windowed mode.
		if (fullscreen)
		{
			m_swapChainDesc.Windowed = false;
		}
		else
		{
			m_swapChainDesc.Windowed = true;
		}

		// Set the scan line ordering and scaling to unspecified.
		m_swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		m_swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

		// Discard the back buffer contents after presenting.
		m_swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;


		// Don't set the advanced flags.
		m_swapChainDesc.Flags = 0;

		// Set the feature level to DirectX 11.
		featureLevel = D3D_FEATURE_LEVEL_11_0;

		// Create the swap chain, Direct3D device, and Direct3D device context.
		result = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, NULL, &featureLevel, 1,
			D3D11_SDK_VERSION, &m_swapChainDesc, m_swapChain.GetAddressOf(), m_device.GetAddressOf(), NULL, m_deviceContext.GetAddressOf());
		if (FAILED(result))
		{
			return false;
		}

		m_device.Get()->QueryInterface(__uuidof(ID3D11Debug),
			reinterpret_cast<void**>(m_debugLayer.GetAddressOf()));

		// Get the pointer to the back buffer.
		result = m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)backBufferPtr.GetAddressOf());
		if (FAILED(result))
		{
			return false;
		}

		// Create the render target view with the back buffer pointer.
		result = m_device->CreateRenderTargetView(backBufferPtr.Get(), NULL, m_renderTargetView.GetAddressOf());
		if (FAILED(result))
		{
			return false;
		}

		// Initialize the description of the depth buffer.
		ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));

		// Set up the description of the depth buffer.
		depthBufferDesc.Width = screenWidth;
		depthBufferDesc.Height = screenHeight;
		depthBufferDesc.MipLevels = 1;
		depthBufferDesc.ArraySize = 1;
		depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthBufferDesc.SampleDesc.Count = 1;
		depthBufferDesc.SampleDesc.Quality = 0;
		depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		depthBufferDesc.CPUAccessFlags = 0;
		depthBufferDesc.MiscFlags = 0;

		// Create the texture for the depth buffer using the filled out description.
		result = m_device->CreateTexture2D(&depthBufferDesc, NULL, m_depthStencilBuffer.GetAddressOf());
		if (FAILED(result))
		{
			return false;
		}

		SetD3DDebugName(m_depthStencilBuffer.Get(), "NimbleD3D::m_depthStencilBuffer");

		// Initialize the description of the stencil state.
		ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

		// Set up the description of the stencil state.
		depthStencilDesc.DepthEnable = true;
		depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

		depthStencilDesc.StencilEnable = true;
		depthStencilDesc.StencilReadMask = 0xFF;
		depthStencilDesc.StencilWriteMask = 0xFF;

		// Stencil operations if pixel is front-facing.
		depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
		depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

		// Stencil operations if pixel is back-facing.
		depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
		depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

		// Create the depth stencil state.
		result = m_device->CreateDepthStencilState(&depthStencilDesc, m_depthStencilState.GetAddressOf());
		if (FAILED(result))
		{
			return false;
		}

		// Set the depth stencil state.
		m_deviceContext->OMSetDepthStencilState(m_depthStencilState.Get(), 1);

		// Initailze the depth stencil view.
		ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

		// Set up the depth stencil view description.
		depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		depthStencilViewDesc.Texture2D.MipSlice = 0;

		// Create the depth stencil view.
		result = m_device->CreateDepthStencilView(m_depthStencilBuffer.Get(),
			&depthStencilViewDesc, m_depthStencilView.GetAddressOf());
		if (FAILED(result))
		{
			return false;
		}
		LOG_INFO("Depth Stencil view created.");
		SetD3DDebugName(m_depthStencilView.Get(), "NimbleD3D::m_depthStencilView");

		// Bind the render target view and depth stencil buffer to the output render pipeline.
		m_deviceContext->OMSetRenderTargets(1, m_renderTargetView.GetAddressOf(), m_depthStencilView.Get());

		// Setup the raster description which will determine how and what polygons will be drawn.
		rasterDesc.AntialiasedLineEnable = false;
		rasterDesc.CullMode = D3D11_CULL_BACK;
		rasterDesc.DepthBias = 0;
		rasterDesc.DepthBiasClamp = 0.0f;
		rasterDesc.DepthClipEnable = true;
		rasterDesc.FillMode = D3D11_FILL_SOLID;
		rasterDesc.FrontCounterClockwise = false;
		rasterDesc.MultisampleEnable = false;
		rasterDesc.ScissorEnable = false;
		rasterDesc.SlopeScaledDepthBias = 0.0f;

		// Create the rasterizer state from the description we just filled out.
		result = m_device->CreateRasterizerState(&rasterDesc, m_rasterState.GetAddressOf());
		if (FAILED(result))
		{
			return false;
		}

		// Now set the rasterizer state.
		m_deviceContext->RSSetState(m_rasterState.Get());

		// Setup the viewport for rendering.
		viewport.Width = (float)screenWidth;
		viewport.Height = (float)screenHeight;
		viewport.MinDepth = 0.0f;
		viewport.MaxDepth = 1.0f;
		viewport.TopLeftX = 0.0f;
		viewport.TopLeftY = 0.0f;

		// Create the viewport.
		m_deviceContext->RSSetViewports(1, &viewport);

		// Setup the projection matrix.
		fieldOfView = XM_PIDIV4;
		screenAspect = (float)screenWidth / (float)screenHeight;

		//m_projectionMatrix = Matrix::CreatePerspectiveFieldOfView(fieldOfView, screenAspect, screenNear, screenDepth);

		m_projectionMatrix = DirectX::XMMatrixPerspectiveFovLH(fieldOfView, screenAspect, screenNear, screenDepth);

		m_worldMatrix = Matrix::Identity;

		StatesHelper::GetInstance().Load(m_device);

		return true;
	}

	ComPtr<ID3D11Debug> GetDebugLayer()
	{
		return m_debugLayer;
	}

	void BeginScene(DirectX::XMVECTORF32 clearColor)
	{

		float color[4];

		// Setup the color to clear the buffer to.
		color[0] = clearColor.f[0];
		color[1] = clearColor.f[1];
		color[2] = clearColor.f[2];
		color[3] = clearColor.f[3];

		// Clear the back buffer.
		m_deviceContext->ClearRenderTargetView(m_renderTargetView.Get(), color);

		// Clear the depth buffer.
		m_deviceContext->ClearDepthStencilView(m_depthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);
	}

	void EndScene()
	{
		// Present the back buffer to the screen since rendering is complete.
		if (m_vsync_enabled)
		{
			// Lock to screen refresh rate.
			m_swapChain->Present(1, 0);
		}
		else
		{
			// Present as fast as possible.
			m_swapChain->Present(0, 0);
		}
	}

	void CreateRenderTarget(int screenWidth, int screenHeight)
	{
		ComPtr<ID3D11Texture2D> backBufferPtr;
		HRESULT result;
		D3D11_VIEWPORT viewport;
		D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
		D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
		D3D11_TEXTURE2D_DESC depthBufferDesc;

		// Get the pointer to the back buffer.
		result = m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)backBufferPtr.GetAddressOf());
		if (FAILED(result))
		{
			cout << "Failed to get buffer." << endl;
			return;
		}

		// Create the render target view with the back buffer pointer.
		result = m_device->CreateRenderTargetView(backBufferPtr.Get(), NULL, m_renderTargetView.GetAddressOf());
		if (FAILED(result))
		{
			cout << "Failed to create render target view." << endl;
			return;
		}

		// Initialize the description of the depth buffer.
		ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));

		// Set up the description of the depth buffer.
		depthBufferDesc.Width = screenWidth;
		depthBufferDesc.Height = screenHeight;
		depthBufferDesc.MipLevels = 1;
		depthBufferDesc.ArraySize = 1;
		depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthBufferDesc.SampleDesc.Count = 1;
		depthBufferDesc.SampleDesc.Quality = 0;
		depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		depthBufferDesc.CPUAccessFlags = 0;
		depthBufferDesc.MiscFlags = 0;

		// Create the texture for the depth buffer using the filled out description.
		result = m_device->CreateTexture2D(&depthBufferDesc, NULL, m_depthStencilBuffer.GetAddressOf());
		if (FAILED(result))
		{
			cout << "Failed to create texture 2d for depth stencil buffer" << endl;
			return;
		}

		// Initialize the description of the stencil state.
		ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

		// Set up the description of the stencil state.
		depthStencilDesc.DepthEnable = true;
		depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

		depthStencilDesc.StencilEnable = true;
		depthStencilDesc.StencilReadMask = 0xFF;
		depthStencilDesc.StencilWriteMask = 0xFF;

		// Stencil operations if pixel is front-facing.
		depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
		depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

		// Stencil operations if pixel is back-facing.
		depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
		depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

		// Initailze the depth stencil view.
		ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

		// Set up the depth stencil view description.
		depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		depthStencilViewDesc.Texture2D.MipSlice = 0;

		// Create the depth stencil view.
		m_device->CreateDepthStencilView(m_depthStencilBuffer.Get(), &depthStencilViewDesc, m_depthStencilView.GetAddressOf());

		// Bind the render target view and depth stencil buffer to the output render pipeline.
		m_deviceContext->OMSetRenderTargets(1, m_renderTargetView.GetAddressOf(), m_depthStencilView.Get());

		ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));
		viewport.MinDepth = 0.0f;
		viewport.MaxDepth = 1.0f;
		viewport.TopLeftX = 0;
		viewport.TopLeftY = 0;
		viewport.Width = static_cast<float>(screenWidth);
		viewport.Height = static_cast<float>(screenHeight);

		// Create the viewport.
		m_deviceContext->RSSetViewports(1, &viewport);

		auto screenAspect = static_cast<float>(screenWidth) / static_cast<float>(screenHeight);

		// Create the projection matrix for 3D rendering.
		m_projectionMatrix = DirectX::XMMatrixPerspectiveFovLH(XM_PIDIV4, screenAspect, 0.1f, 1000.0f);
	}

	void TranslateFOV(float dx, int screenWidth, int screenHeight)
	{
		fieldOfView += dx;
		float screenAspect = static_cast<float>(screenWidth) / static_cast<float>(screenHeight);
		m_projectionMatrix = DirectX::XMMatrixPerspectiveFovLH(XM_PIDIV4, screenAspect, 0.1f, 1000.0f);
	}

	void ReleaseRenderTarget()
	{
		this->m_renderTargetView.Reset();
		this->m_depthStencilView.Reset();
		this->m_depthStencilBuffer.Reset();
	}

	void GetVideoCardInfo(char* cardName, int& memory) const
	{
		strcpy_s(cardName, 128, m_videoCardDescription);
		memory = m_videoCardMemory;
	}

	ComPtr<ID3D11Device> GetDevice() const
	{
		return m_device;
	}

	ComPtr<ID3D11DeviceContext> GetDeviceContext() const
	{
		return m_deviceContext;
	}

	const Matrix& GetProjectionMatrix() const
	{
		return m_projectionMatrix;
	}

	const Matrix& GetWorldMatrix() const
	{
		return m_worldMatrix;
	}

	DXGI_SWAP_CHAIN_DESC GetSwapChainDesc() const
	{
		return m_swapChainDesc;
	}

	auto GetSwapChain() const -> ComPtr<IDXGISwapChain>
	{
		return m_swapChain;
	}

	void UpdateProjectionMatrix(int width, int height, float screenNear, float screenFar)
	{
		auto screenAspect = static_cast<float>(width) / static_cast<float>(height);

		m_projectionMatrix = DirectX::XMMatrixPerspectiveFovLH(XM_PIDIV4, screenAspect, screenNear, screenFar);
	}
};