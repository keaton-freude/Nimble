#include "Graphics.h"
#include <vector>
#include "VertexBuffer.h"
#include "ShaderManager.h"
#include <SimpleMath.h>
#include <stdio.h>
#include <time.h>
#include <chrono>
#include "ParticleVertex.h"
#include <DirectXColors.h>

using namespace DirectX::Colors;
using DirectX::SimpleMath::Ray;

using namespace std;

Graphics::Graphics()
	: _dt(0.0f), _D3D(0), _camera(0), particleEngine(std::make_shared<ParticleEngine>())
{
	this->_light0 = shared_ptr<Light>(new Light());
	LOG_INFO("Graphics constructed.");
	LOG_INFO("SizeOf: ", ParticleVertex::GetSize());
}

Graphics::~Graphics()
{
	//_CrtDumpMemoryLeaks();
	LOG_INFO("Graphics destruct!");
	ShaderManager::GetInstance().Shutdown();
	
}

// User has clicked at (mouseX, mouseY)
// Radius: All vertices within the radius should be affected
// Intensity: How much to add to each vertex
void Graphics::HeightmapAdd(Vector3 location, float radius, float intensity)
{
	terrain->HeightmapAdd(location, radius, _D3D->GetDeviceContext(), _D3D->GetDevice());
}

RayHit Graphics::IsRayIntersectingTerrain(Ray r)
{
	return terrain->IsRayIntersectingTerrain(r);
}

void Graphics::SetAmbientLight(float r, float g, float b)
{
	_light0->SetAmbientLight(Vector4(r, g, b, 1.0f));
}

void Graphics::SetDiffuseColor(float r, float g, float b)
{
	_light0->SetDiffuseColor(Vector4(r, g, b, 1.0f));
}

bool Graphics::Init(int screenWidth, int screenHeight, HWND hwnd, bool fullScreen, bool vSyncEnabled, float screenDepth, float screenNear)
{
	bool result;

	srand(time(0));

	_D3D = make_shared<NimbleD3D>();
	result = _D3D->Init(screenWidth, screenHeight, vSyncEnabled, hwnd, fullScreen, screenDepth, screenNear);

	if (!result)
	{
		LOG_ERROR("Failed to init D3D in Graphics::Init.");
		return false;
	}

	LOG_INFO("D3D Initialized successfully.");

	// Create the camera object at <5, 10, 5> with rotation: x:.78f, y:.78f, z:0.0f
	_camera = make_shared<Camera>(Vector3(0.0f, 5.0f, -5.0f), Vector3(0.78f, 0.0f, 0.0f));

	LOG_INFO("Camera created successfully.");
	 
	ShaderManager::GetInstance().Load(_D3D->GetDevice().Get());

	LOG_INFO("Shaders compiled.");

	// Set Initial Line
	_line = make_shared<LineDrawable>();
	SetDebugLine(Vector3(0, 0, 0), Vector3(0, 100, 0));

	terrain = make_shared<Terrain>(_D3D->GetDevice(), _D3D->GetDeviceContext(), 4, 4, "..\\..\\Assets\\Textures\\grass5.tga", "..\\..\\Assets\\Textures\\slope.tga", "..\\..\\Assets\\Textures\\stone1.tga");

	LOG_INFO("Graphics initialization complete.");

	_frustum = make_shared<Frustum>();

	return true;
}

bool Graphics::ResizeBuffers(UINT width, UINT height)
{
	shared_ptr<DXGI_SWAP_CHAIN_DESC> format;
	_D3D->GetSwapChain()->GetDesc(format.get());

	_D3D->GetSwapChain()->ResizeBuffers(1, width, height, format->BufferDesc.Format, 0);
	
	//delete format;
	return true;
}

void Graphics::Shutdown()
{
	UnloadResources();
}

void Graphics::UnloadResources()
{
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

shared_ptr<ParticleEngine> Graphics::GetParticleEngine()
{
	return particleEngine;
}

bool Graphics::RenderMinimap()
{
	_D3D->BeginScene(Black);

	_D3D->EndScene();
	return true;
}

bool Graphics::RenderViewport()
{
	// Clear the buffers to begin the scene.
	_D3D->BeginScene(Black);

	_camera->Update();

	auto projectionMatrix = _D3D->GetProjectionMatrix();
	auto viewMatrix = _camera->GetViewMatrix();

	_frustum->ConstructFrustum(1000.0f, *projectionMatrix, *viewMatrix);
	_line->Draw(_D3D->GetDeviceContext().Get(), viewMatrix, projectionMatrix);

	//terrain->Draw(_D3D->GetDevice(), _D3D->GetDeviceContext(), viewMatrix, projectionMatrix, _light0, _frustum);

	//particleSystem->Update(*viewMatrix, *projectionMatrix, _dt);
	//particleSystem->Draw(_D3D->GetDevice(), _D3D->GetDeviceContext());
	particleEngine->Update(*viewMatrix, *projectionMatrix, _dt);
	particleEngine->Draw(_D3D->GetDevice(), _D3D->GetDeviceContext());

	
	//StartDebugTimer();
	_D3D->EndScene();
	//EndDebugTimer("PS: ");

	return true;
}

void Graphics::PrintDebugObjects()
{
	_D3D->GetDebugLayer()->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
}

string Graphics::GetStatistics()
{
	string stats = "";
	// Ask terrain to report on the following info:
	// 1. # of verts being drawn
	// 2. Draw count

	//stats = "Visible Terrain Verts: ";
	//stats += std::to_string(terrain->GetVisibleVertCount());
	//
	//stats += " | Terrain Draw Count: ";
	//stats += std::to_string(terrain->GetDrawCount());

	//int total_particles = 0;
	//if (particleSystem->first_active_particle < particleSystem->first_free_particle)
	//{
	//	total_particles = particleSystem->first_new_particle - particleSystem->first_active_particle;
	//}
	//else
	//{ 
	//	total_particles += (particleSystem->settings.max_particles - particleSystem->first_active_particle);

	//	if (particleSystem->first_free_particle > 0)
	//	{
	//		total_particles += particleSystem->first_free_particle;
	//	}
	//}


	//stats = "Active: " + std::to_string(particleSystem->first_active_particle) +
	//	"New: " + to_string(particleSystem->first_new_particle) +
	//	"Free: " + to_string(particleSystem->first_free_particle) +
	//	"Retired: " + to_string(particleSystem->first_retired_particle); +
	//	" | Total Particles: " + to_string(total_particles);

	//return "Total Particles " + to_string(total_particles);
	return stats;
}

void Graphics::SetLightDirection(float x, float y, float z)
{
	_light0->SetLightDirection(Vector3(x, y, z));
}

shared_ptr<Terrain> Graphics::GetTerrain()
{
	return this->terrain;
}

shared_ptr<Light> Graphics::GetLight()
{
	return _light0;
}

void Graphics::SetDebugLine(Vector3 p1, Vector3 p2)
{
	_line->SetLine(_D3D->GetDevice().Get(), p1, p2);
}

auto Graphics::GetSwapChain() -> ComPtr<IDXGISwapChain>
{
	return _D3D->GetSwapChain();
}

void Graphics::CompileShaders()
{
	
	ShaderManager::GetInstance().Load(_D3D->GetDevice().Get());
}

void Graphics::LoadVertexBuffers()
{
}

shared_ptr<Camera> Graphics::getCamera()
{
	return this->_camera;
}

DXGI_SWAP_CHAIN_DESC Graphics::GetSwapChainDesc()
{
	return _D3D->GetSwapChainDesc();
}

shared_ptr<NimbleD3D> Graphics::GetD3D()
{
	return _D3D;
}

void Graphics::SetDT(float new_dt)
{
    this->_dt = new_dt;
}

float Graphics::GetDT()
{
    return _dt;
}
