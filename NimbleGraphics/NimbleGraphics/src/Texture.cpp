#include "Texture.h"
#include "Logger.h"

Texture::Texture(D3DDevice device, D3DDeviceContext deviceContext, string textureName)
{
	if (!this->Init(device, deviceContext, textureName))
	{
		LOG_ERROR("Cannot load texture:");
		LOG_ERROR(textureName);
	}
}

Texture::~Texture()
{
	LOG_INFO("Texture destruct!");
}

D3DShaderResourceView Texture::GetTexture() const
{
	return _textureView;
}

bool Texture::LoadTarga(string filename, int& height, int& width)
{
	int error, bpp, imageSize;
	FILE* filePtr;
	size_t count;
	TargaHeader targaFileHeader;


	// Open the targa file for reading in binary.
	error = fopen_s(&filePtr, filename.c_str(), "rb");
	if (error != 0)
	{
		return false;
	}

	// Read in the file header.
	count = fread(&targaFileHeader, sizeof(TargaHeader), 1, filePtr);
	if (count != 1)
	{
		return false;
	}

	// Get the important information from the header.
	height = static_cast<int>(targaFileHeader.height);
	width = static_cast<int>(targaFileHeader.width);
	bpp = static_cast<int>(targaFileHeader.bpp);

	// Check that it is 32 bit and not 24 bit.
	if (bpp != 32)
	{
		return false;
	}

	// Calculate the size of the 32 bit image data.
	imageSize = width * height * 4;

	// Allocate memory for the targa image data.
	_targaImage = unique_ptr<unsigned char>(new unsigned char[imageSize]);
	if (!_targaImage)
	{
		return false;
	}

	// Read in the targa image data.
	count = fread(_targaImage.get(), 1, imageSize, filePtr);
	if (count != imageSize)
	{
		return false;
	}

	// Close the file.
	error = fclose(filePtr);
	if (error != 0)
	{
		return false;
	}

	return true;
}

bool Texture::Init(D3DDevice device, D3DDeviceContext deviceContext, string textureName)
{
	bool result;
	int height, width;
	D3D11_TEXTURE2D_DESC textureDesc;
	HRESULT hResult;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	unsigned char* dataPtr;
	unsigned int i, j, k, rowStart, columnStart;
	D3D11_SHADER_RESOURCE_VIEW_DESC srDesc;


	// Load the targa texture data into memory.
	result = LoadTarga(textureName, height, width);
	if (!result)
	{
		return false;
	}

	// Setup the description of the texture.
	textureDesc.Height = height;
	textureDesc.Width = width;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM_SRGB;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_DYNAMIC;
	textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	textureDesc.MiscFlags = 0;

	// Create the empty texture.
	hResult = device->CreateTexture2D(&textureDesc, NULL, _texture.GetAddressOf());
	if (FAILED(hResult))
	{
		return false;
	}

	// Lock the texture so it can be written to by the CPU.
	hResult = deviceContext->Map(_texture.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(hResult))
	{
		return false;
	}

	// Get a pointer to the mapped resource data pointer and cast it into an unsigned char to match the targa data.
	dataPtr = static_cast<unsigned char*>(mappedResource.pData);

	// Initialize the index into the targa data since targa data is stored upside down.
	k = (width * height * 4) - (width * 4);

	// Load the targa data into the texture now.
	for (j = 0; j < static_cast<unsigned int>(height); j++)
	{
		// Set the beginning of the row.
		rowStart = j * mappedResource.RowPitch;

		for (i = 0; i < static_cast<unsigned int>(width); i++)
		{
			// Set the beginning of the column.
			columnStart = i * 4;

			auto r = _targaImage.get()[k + 2];
			auto g = _targaImage.get()[k + 1];
			auto b = _targaImage.get()[k + 0];
			auto a = _targaImage.get()[k + 3];

			// Copy the data in.
			dataPtr[rowStart + columnStart + 0] = _targaImage.get()[k + 2]; // Red.
			dataPtr[rowStart + columnStart + 1] = _targaImage.get()[k + 1]; // Green.
			dataPtr[rowStart + columnStart + 2] = _targaImage.get()[k + 0]; // Blue
			dataPtr[rowStart + columnStart + 3] = _targaImage.get()[k + 3]; // Alpha

			// Increment the index into the targa data.
			k += 4;
		}

		// Set the targa data index back to the preceding row at the beginning of the column since its reading it in upside down.
		k -= (width * 8);
	}

	// Unlock the texture.
	deviceContext->Unmap(_texture.Get(), 0);

	// Create the shader-resource view
	srDesc.Format = textureDesc.Format;
	srDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srDesc.Texture2D.MostDetailedMip = 0;
	srDesc.Texture2D.MipLevels = 1;

	// Create the shader resource view for the texture.
	hResult = device->CreateShaderResourceView(_texture.Get(), &srDesc, _textureView.GetAddressOf());
	if (FAILED(hResult))
	{
		return false;
	}

	return true;
}
