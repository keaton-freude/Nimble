#include "TileShader.h"
#include "SplatMap.h"
#include "Helper.h"
#include "DiffuseShaderComponent.h"
#include "SamplerStateShaderComponent.h"
#include "WVPShaderComponent.h"
#include "LightShaderComponent.h"

void TileShader::SetComponents()
{
	_components = std::vector<shared_ptr<IShaderComponent>>
	({
		make_shared<WVPShaderComponent>(),
		make_shared<SamplerStateShaderComponent>(StatesHelper::GetInstance().GetStates()->LinearWrap()),
		make_shared<DiffuseShaderComponent>(),
		make_shared<LightShaderComponent>()
	});
}

void TileShader::GetPolygonLayout(shared_ptr<D3D11_INPUT_ELEMENT_DESC>& desc, unsigned& numElements)
{
}
