#pragma once
#include <SimpleMath.h>
#include <d3d11.h>
#include <wrl/client.h>
using DirectX::SimpleMath::Matrix;

typedef const Matrix& MatrixRef;
typedef Microsoft::WRL::ComPtr<ID3D11Device> D3DDevice;
typedef Microsoft::WRL::ComPtr<ID3D11DeviceContext> D3DDeviceContext;
