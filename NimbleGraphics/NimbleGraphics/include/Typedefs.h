#pragma once
#include <d3d11.h>
#include <SimpleMath.h>
#include <wrl/client.h>
#include <memory>
//#include "../../packages/directxtk_desktop_2015.2016.10.6.1/build/native/include/SimpleMath.h"
using DirectX::SimpleMath::Matrix;
using DirectX::SimpleMath::Ray;
using DirectX::SimpleMath::Vector2;
using DirectX::SimpleMath::Vector3;
using DirectX::SimpleMath::Vector4;
using DirectX::SimpleMath::Plane;
using DirectX::SimpleMath::Color;
using namespace DirectX::SimpleMath;

using std::shared_ptr;
using std::unique_ptr;
using std::make_shared;
using std::make_unique;

using Microsoft::WRL::ComPtr;

using MatrixRef = const Matrix&;
using D3DDevice = ComPtr<ID3D11Device>;
using D3DDeviceContext = ComPtr<ID3D11DeviceContext>;
using D3DDebug = ComPtr<ID3D11Debug>;
using DxgiSwapChain = ComPtr<IDXGISwapChain>;
using D3DRasterizerState = ComPtr<ID3D11RasterizerState>;
using D3DDepthStencilView = ComPtr<ID3D11DepthStencilView>;
using D3DDepthStencilState = ComPtr <ID3D11DepthStencilState>;
using D3DTexture2D = ComPtr<ID3D11Texture2D>;
using D3DRenderTargetView = ComPtr<ID3D11RenderTargetView>;
using D3DShaderResourceView = ComPtr<ID3D11ShaderResourceView>;
using D3DBuffer = ComPtr<ID3D11Buffer>;
using D3DInputLayout = ComPtr<ID3D11InputLayout>;
using D3DSamplerState = ComPtr<ID3D11SamplerState>;

#define NIMBLE_TEXTURE_W(x) L"..\\Assets\\Textures\\"#x
#define NIMBLE_TEXTURE(x) "..\\Assets\\Textures\\"#x