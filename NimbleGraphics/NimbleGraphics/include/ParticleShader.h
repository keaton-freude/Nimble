//#pragma once
//#include "IShader.h"
//#include <d3d11.h>
//#include "Structs.h"
//#include <memory>
//#include "Texture.h"
//#include "ParticleSettings.h"
//
//using Microsoft::WRL::ComPtr;
//
//class ParticleShader : public BaseShader
//{
//public:
//	ParticleShader();
//
//	virtual ~ParticleShader();
//
//	ParticleShader(ComPtr<ID3D11Device> device, D3DDeviceContext deviceContext, ParticleSettings settings);
//
//	bool Load() override;
//
//	// sets the values on our local copy of the shader that never change
//	void CreateShaderConstants(ParticleSettings settings);
//
//	void Update(const Matrix& worldMatrix, const Matrix& viewMatrix, const Matrix& projectionMatrix) override;
//
//	bool Draw(ComPtr<ID3D11DeviceContext> deviceContext, int indexCount, int startIndexLocation, int baseVertexLocation,
//	          Texture* texture, Vector2 viewport, float current_time, bool update = true);
//
//	void Draw(int indexCount) override;
//
//
//	void RenderShader(ComPtr<ID3D11DeviceContext> deviceContext, int indexCount, int startIndexLocation, int baseVertexLocation);
//	
//	virtual bool SetShaderParameters() override;
//
//	bool SetShaderParameters(ComPtr<ID3D11DeviceContext> deviceContext,
//	                         ComPtr<ID3D11ShaderResourceView> texture, Vector2 viewport, float current_time, bool update = true);
//private:
//	// Inherited via IShader
//	virtual void GetPolygonLayout(shared_ptr<D3D11_INPUT_ELEMENT_DESC>& desc, unsigned int& numElements) override;
//
//	void UpdatePerFrameBuffer(ComPtr<ID3D11DeviceContext> deviceContext, Vector2 viewport_scale, float current_time) const;
//
//protected:
//	std::vector<shared_ptr<IShaderComponent>>& GetComponents() override;
//
//	void SetComponents() override;
//private:
//	ComPtr<ID3D11Buffer> perFrameParticleParameters;
//	ComPtr<ID3D11Buffer> constantParticleParameters;
//
//
//};