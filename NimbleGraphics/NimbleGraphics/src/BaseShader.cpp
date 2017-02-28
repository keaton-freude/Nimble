//#include "BaseShader.h"
//#include "Structs.h"
//
//BaseShader::BaseShader()
//{
//}
//
//BaseShader::BaseShader(D3DDevice device, D3DDeviceContext deviceContext)
//	: IShader(device, deviceContext)
//{
//
//}
//
//void BaseShader::SetWorldMatrix(const Matrix& worldMatrix)
//{
//	this->_worldMatrix = worldMatrix;
//}
//
//void BaseShader::SetViewMatrix(const Matrix& viewMatrix)
//{
//	this->_viewMatrix = viewMatrix;
//}
//
//void BaseShader::SetProjectionMatrix(const Matrix& projectionMatrix)
//{
//	this->_projectionMatrix = projectionMatrix;
//}
//
//bool BaseShader::SetShaderParameters()
//{
//	return true;
//}
//
//bool BaseShader::Load()
//{
//	return true;
//}
//
//void BaseShader::Draw(int indexCount)
//{
//}
//
//void BaseShader::Update(MatrixRef world, MatrixRef view, MatrixRef projection)
//{
//	_worldMatrix = world;
//	_viewMatrix = view;
//	_projectionMatrix = projection;
//}
//
//D3D11_BUFFER_DESC BaseShader::GetMatrixBufferDescription()
//{
//	D3D11_BUFFER_DESC matrixBufferDesc;
//
//	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
//	matrixBufferDesc.ByteWidth = sizeof(MatrixBuffer);
//	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
//	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
//	matrixBufferDesc.MiscFlags = 0;
//	matrixBufferDesc.StructureByteStride = 0;
//
//	return matrixBufferDesc;
//}
//
//void BaseShader::GetPolygonLayout(shared_ptr<D3D11_INPUT_ELEMENT_DESC>& desc, unsigned int& numElements)
//{
//}
