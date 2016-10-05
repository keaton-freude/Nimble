#pragma once
#include <d3d11.h>
#include <SimpleMath.h>
#include <wrl/client.h>
using DirectX::SimpleMath::Matrix;

using MatrixRef = const Matrix&;
using D3DDevice = Microsoft::WRL::ComPtr<ID3D11Device>;
using D3DDeviceContext = Microsoft::WRL::ComPtr<ID3D11DeviceContext>;
using D3DDebug = Microsoft::WRL::ComPtr<ID3D11Debug>;
using DxgiSwapChain = Microsoft::WRL::ComPtr<IDXGISwapChain>;
using D3DRasterizerState = Microsoft::WRL::ComPtr<ID3D11RasterizerState>;
using D3DDepthStencilView = Microsoft::WRL::ComPtr<ID3D11DepthStencilView>;
using D3DDepthStencilState = Microsoft::WRL::ComPtr <ID3D11DepthStencilState>;
using D3DTexture2D = Microsoft::WRL::ComPtr<ID3D11Texture2D>;
using D3DRenderTargetView = Microsoft::WRL::ComPtr<ID3D11RenderTargetView>;
using D3DShaderResourceView = Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>;
using D3DBuffer = Microsoft::WRL::ComPtr<ID3D11Buffer>;