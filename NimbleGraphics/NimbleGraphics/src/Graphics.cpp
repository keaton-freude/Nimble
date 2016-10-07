#include "Graphics.h"

Graphics::Graphics()
	: _dt(0), _currentTime(0)
{
}

Graphics::Graphics(int screenWidth, int screenHeight, HWND hwnd, bool fullscreen, bool vSyncEnabled, float screenDepth, float screenNear)
{
	Init(screenWidth, screenHeight, hwnd, fullscreen, vSyncEnabled, screenDepth, screenNear);
}

Graphics::~Graphics()
{
	//_CrtDumpMemoryLeaks();
	LOG_INFO("Graphics destruct!");
	ShaderManager::GetInstance().Shutdown();
}

void Graphics::HeightmapAdd(Vector3 location, float radius, float intensity)
{
	terrain->SmoothHeightmapAdd(location, radius, intensity, _D3D.GetDeviceContext(), _D3D.GetDevice());
}

bool Graphics::Init(int screenWidth, int screenHeight, HWND hwnd, bool fullscreen, bool vSyncEnabled, float screenDepth, float screenNear)
{
	srand(time(nullptr));

	_D3D = NimbleD3D(screenWidth, screenHeight, vSyncEnabled, hwnd, fullscreen, screenDepth, screenNear);
	_camera = Camera(Vector3(0.0f, 5.0f, 0.0f), Vector3(0.78f, 0.0f, 0.0f));

	ShaderManager::GetInstance().Load(_D3D.GetDevice().Get());
	DebugLineManager::GetInstance().Load(_D3D.GetDevice());

	terrain = make_shared<Terrain>(_D3D.GetDevice(), _D3D.GetDeviceContext(), 1, 1, "..\\..\\Assets\\Textures\\grass8.tga", 
		"..\\..\\Assets\\Textures\\weird_texture.tga", "..\\..\\Assets\\Textures\\grass4.tga", "..\\..\\Assets\\Textures\\splat1.tga");

	LOG_INFO("Graphics initialization complete.");

	return true;
}

bool Graphics::ResizeBuffers(UINT width, UINT height) const
{
	unique_ptr<DXGI_SWAP_CHAIN_DESC> format;

	_D3D.GetSwapChain()->GetDesc(format.get());
	_D3D.GetSwapChain()->ResizeBuffers(1, width, height, format->BufferDesc.Format, 0);

	return true;
}

auto Graphics::GetSwapChain() const -> ComPtr<IDXGISwapChain>
{
	return _D3D.GetSwapChain();
}

Camera& Graphics::GetCamera()
{
	return this->_camera;
}

DXGI_SWAP_CHAIN_DESC Graphics::GetSwapChainDesc() const
{
	return _D3D.GetSwapChainDesc();
}

NimbleD3D& Graphics::GetD3D()
{
	return _D3D;
}

void Graphics::SetDT(float new_dt)
{
	this->_dt = new_dt;
}

float Graphics::GetDT() const
{
	return _dt;
}

shared_ptr<Terrain> Graphics::GetTerrain() const
{
	return this->terrain;
}

const Light& Graphics::GetLight() const
{
	return _light0;
}

RayHit Graphics::IsRayIntersectingTerrain(Ray r) const
{
	return terrain->IsRayIntersectingTerrain(r);
}

void Graphics::PrintDebugObjects() const
{
	_D3D.GetDebugLayer()->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
}

string Graphics::GetStatistics()
{
	string stats = "";
	return stats;
}

void Graphics::SetLightDirection(float x, float y, float z)
{
	_light0.SetLightDirection(Vector3(x, y, z));
}

void Graphics::SetAmbientLight(float r, float g, float b)
{
	_light0.SetAmbientLight(Vector4(r, g, b, 1.0f));
}

void Graphics::SetDiffuseColor(float r, float g, float b)
{
	_light0.SetDiffuseColor(Vector4(r, g, b, 1.0f));
}

bool Graphics::Draw(RENDER_MODE mode, float dt)
{
	_dt = dt;
	_currentTime += dt;
	if (mode == RENDER_MODE::Viewport)
	{
		// Need to calculate how much time has passed since the last frame.
		return RenderViewport();
	}
	else if (mode == RENDER_MODE::Minimap)
	{
		return RenderMinimap();
	}
	else
	{
		LOG_ERROR("RENDER MODE SELECTED IS INVALID OR CODE PATH NOT CREATED.");
		return false;
	}
}

ParticleEngine& Graphics::GetParticleEngine()
{
	return particleEngine;
}

bool Graphics::RenderMinimap() const
{
	_D3D.BeginScene(DirectX::Colors::Black);

	_D3D.EndScene();
	return true;
}

bool Graphics::RenderViewport()
{
	// Clear the buffers to begin the scene.
	_D3D.BeginScene(DirectX::Colors::Black);

	_camera.Update();

	auto projectionMatrix = _D3D.GetProjectionMatrix();
	auto viewMatrix = _camera.GetViewMatrix();

	_frustum.ConstructFrustum(1000.0f, projectionMatrix, viewMatrix);
	terrain->Draw(_D3D.GetDevice(), _D3D.GetDeviceContext(), viewMatrix, projectionMatrix, _light0, _frustum);

	_D3D.EndScene();

	return true;
}