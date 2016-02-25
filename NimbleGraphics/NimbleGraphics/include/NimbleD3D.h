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

using Microsoft::WRL::ComPtr;
using std::shared_ptr;
using DirectX::SimpleMath::Matrix;
using DirectX::SimpleMath::Color;

class NimbleD3D
{
public:
	NimbleD3D();
	NimbleD3D(const NimbleD3D&);
	~NimbleD3D();

	bool Init(int screenWidth, int screenHeight, bool vsync, HWND hwnd, 
		bool fullscreen, float screenDepth, float screenNear);

	void BeginScene(DirectX::XMVECTORF32 clearColor);
	void EndScene();

	void CreateRenderTarget(int screenWidth, int screenHeight);
	void TranslateFOV(float dx, int screenWidth, int screenHeight);
	void ReleaseRenderTarget();
	void GetVideoCardInfo(char*, int&);

	HWND getHwnd();
	ComPtr<ID3D11Device> GetDevice();
	ComPtr<ID3D11DeviceContext> GetDeviceContext();
	ComPtr<ID3D11Debug> GetDebugLayer();
	shared_ptr<Matrix> GetProjectionMatrix();
	shared_ptr<Matrix> GetWorldMatrix();
	DXGI_SWAP_CHAIN_DESC GetSwapChainDesc();
	auto GetSwapChain() -> ComPtr<IDXGISwapChain>;

	void UpdateProjectionMatrix(int width, int height, float screenNear, float screenFar);

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

	shared_ptr<Matrix> m_projectionMatrix;
	shared_ptr<Matrix> m_worldMatrix;

};