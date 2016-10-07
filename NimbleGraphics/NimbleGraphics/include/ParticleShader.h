#pragma once
#include "IShader.h"
#include <d3d11.h>
#include "Structs.h"
#include <memory>
#include "Logger.h"
#include "Texture.h"
#include "Mesh.h"
#include "ParticleSettings.h"
#include "ParticleVertex.h"
#include "Helper.h"

using Microsoft::WRL::ComPtr;

class ParticleShader : public IShader
{
public:
	ParticleShader();

	virtual ~ParticleShader();

	ParticleShader(ComPtr<ID3D11Device> device, ParticleSettings settings);

	bool Load(ComPtr<ID3D11Device> device) override;

	// sets the values on our local copy of the shader that never change
	void CreateShaderConstants(ComPtr<ID3D11Device> device, ParticleSettings settings);

	void Update(const Matrix& worldMatrix, const Matrix& viewMatrix, const Matrix& projectionMatrix);

	bool Draw(ComPtr<ID3D11DeviceContext> deviceContext, int indexCount, int startIndexLocation, int baseVertexLocation,
	          Texture* texture, Vector2 viewport, float current_time, bool update = true);


	void RenderShader(ComPtr<ID3D11DeviceContext> deviceContext, int indexCount, int startIndexLocation, int baseVertexLocation);

	bool SetShaderParameters(ComPtr<ID3D11DeviceContext> deviceContext,
	                         ComPtr<ID3D11ShaderResourceView> texture, Vector2 viewport, float current_time, bool update = true);
private:
	// Inherited via IShader
	void GetPolygonLayout(shared_ptr<D3D11_INPUT_ELEMENT_DESC>& desc, unsigned int& numElements) override;

	void UpdatePerFrameBuffer(ComPtr<ID3D11DeviceContext> deviceContext, Vector2 viewport_scale, float current_time) const;

	ComPtr<ID3D11Buffer> perFrameParticleParameters;
	ComPtr<ID3D11Buffer> constantParticleParameters;


};