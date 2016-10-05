#pragma once

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "dxgi.lib")

#include "Typedefs.h"
#include <DXGI.h>
#include <memory>
#include <wrl/client.h>

using std::shared_ptr;
using DirectX::SimpleMath::Matrix;
using DirectX::SimpleMath::Color;
using DirectX::XM_PIDIV4;

class NimbleD3D
{
public:
	NimbleD3D() = delete;
	NimbleD3D(int screenWidth, int screenHeight, bool vsync, HWND hwnd, bool fullscreen, float screenDepth, float screenNear);
	~NimbleD3D();

	D3DDebug GetDebugLayer() const;
	void BeginScene(DirectX::XMVECTORF32 clearColor) const;
	void EndScene() const;
	void CreateRenderTarget(int screenWidth, int screenHeight);
	void TranslateFOV(float dx, int screenWidth, int screenHeight);
	void ReleaseRenderTarget();
	void GetVideoCardInfo(char* cardName, int& memory) const;
	D3DDevice GetDevice() const;
	D3DDeviceContext GetDeviceContext() const;
	MatrixRef GetProjectionMatrix() const;
	MatrixRef GetWorldMatrix() const;
	DXGI_SWAP_CHAIN_DESC GetSwapChainDesc() const;
	auto GetSwapChain() const -> DxgiSwapChain;
	void UpdateProjectionMatrix(int width, int height, float screenNear, float screenFar);

private:
	bool Init(int screenWidth, int screenHeight, bool vsync, HWND hwnd, bool fullscreen, float screenDepth, float screenNear);

private:
	bool m_vsync_enabled;
	int m_videoCardMemory;
	char m_videoCardDescription[128];
	HWND m_hwnd;
	float fieldOfView;

	DxgiSwapChain m_swapChain;
	DXGI_SWAP_CHAIN_DESC m_swapChainDesc;

	D3DDevice m_device;
	D3DDeviceContext m_deviceContext;
	D3DRenderTargetView m_renderTargetView;
	D3DTexture2D m_depthStencilBuffer;
	D3DDepthStencilState m_depthStencilState;
	D3DDepthStencilView m_depthStencilView;
	D3DRasterizerState m_rasterState;
	D3DDebug m_debugLayer;

	Matrix m_projectionMatrix;
	Matrix m_worldMatrix;
};