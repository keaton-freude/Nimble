#include "TileShader.h"
#include "SplatMap.h"
#include "Helper.h"

TileShader::TileShader()
{
}

bool TileShader::Load()
{
	// Do basic load work
	if (!BaseShader::Load())
	{
		cout << "IShader failed to load correctly in TerrainShader. " << endl;
		return false;
	}

	return true;
}