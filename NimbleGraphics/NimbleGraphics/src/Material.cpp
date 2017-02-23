#include "..\include\Material.h"

Material::Material()
{
	
}

Material::Material(shared_ptr<IShader> shader)
	: shader(shader)
{
}

Material::~Material()
{
}

IShader* Material::GetShader()
{
	return shader.get();
}

void Material::Apply(D3DDeviceContext deviceContext)
{

}
