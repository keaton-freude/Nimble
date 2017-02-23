#include "DiffuseMaterial.h"

DiffuseMaterial::DiffuseMaterial()
	: Material(ShaderManager::GetInstance().GetShader<DiffuseShader>(SHADER::DIFFUSE)), _texture()
{
	_shader = ShaderManager::GetInstance().GetShader<DiffuseShader>(SHADER::DIFFUSE);
}

DiffuseMaterial::DiffuseMaterial(D3DDevice device, D3DDeviceContext deviceContext, shared_ptr<Texture> texture)
	: Material(ShaderManager::GetInstance().GetShader<DiffuseShader>(SHADER::DIFFUSE)), _texture(texture)
{
	_shader = ShaderManager::GetInstance().GetShader<DiffuseShader>(SHADER::DIFFUSE);
}


void DiffuseMaterial::Apply(D3DDeviceContext deviceContext)
{
	_shader.lock()->SetDiffuseTexture(_texture);
	_shader.lock()->SetShaderParameters();
}
