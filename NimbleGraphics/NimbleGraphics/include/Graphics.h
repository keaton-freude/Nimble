#pragma once
#define _CRTDBG_MAP_ALLOC
#define NOMINMAX
#include <stdlib.h>
#include <Windows.h>
#include <memory>
#include <wrl/client.h>

#include "NimbleD3D.h"
#include "RayHit.h"
#include "Frustum.h"
#include "Camera.h"
#include "SimpleMath.h"
#include "ParticleSystem.h"
#include "Terrain.h"
#include "Light.h"
#include "LineDrawable.h"
#include "Logger.h"
#include "ParticleEngine.h"
#include <CommonStates.h>
#include "ShaderManager.h"
#include <DirectXColors.h>
#include "DebugLineManager.h"

using DirectX::CommonStates;
using DirectX::SimpleMath::Vector3;
using DirectX::SimpleMath::Color;
using DirectX::SimpleMath::Matrix;
using std::shared_ptr;
using std::string;
using Microsoft::WRL::ComPtr;

enum RENDER_MODE
{
	Viewport = 0,
	Minimap = 1
};

class Graphics
{
public:
	Graphics(): _dt(0), _currentTime(0)
	{
		LOG_INFO("Graphics constructed.");
		LOG_INFO("SizeOf: ", ParticleVertex::GetSize());
	}

	~Graphics()
	{
		//_CrtDumpMemoryLeaks();
		LOG_INFO("Graphics destruct!");
		ShaderManager::GetInstance().Shutdown();
	}

	void HeightmapAdd(Vector3 location, float radius, float intensity) const
	{
		terrain->SmoothHeightmapAdd(location, radius, intensity, _D3D->GetDeviceContext(), _D3D->GetDevice());
	}

	bool Init(int screenWidth, int screenHeight, HWND hwnd, bool fullscreen,
		bool vSyncEnabled, float screenDepth, float screenNear)
	{
		bool result;

		srand(time(nullptr));

		_D3D = make_shared<NimbleD3D>();
		result = _D3D->Init(screenWidth, screenHeight, vSyncEnabled, hwnd, fullscreen, screenDepth, screenNear);

		if (!result)
		{
			LOG_ERROR("Failed to init D3D in Graphics::Init.");
			return false;
		}

		LOG_INFO("D3D Initialized successfully.");

		// Create the camera object at <5, 10, 5> with rotation: x:.78f, y:.78f, z:0.0f
		_camera = Camera(Vector3(0.0f, 5.0f, 0.0f), Vector3(0.78f, 0.0f, 0.0f));

		LOG_INFO("Camera created successfully.");

		ShaderManager::GetInstance().Load(_D3D->GetDevice().Get());
		DebugLineManager::GetInstance().Load(_D3D->GetDevice());

		LOG_INFO("Shaders compiled.");

		// Set Initial Line
		SetDebugLine(Vector3(0, 0, 0), Vector3(0, 100, 0));

		terrain = make_shared<Terrain>(_D3D->GetDevice(), _D3D->GetDeviceContext(), 1, 1, "..\\..\\Assets\\Textures\\grass8.tga", "..\\..\\Assets\\Textures\\slope.tga", "..\\..\\Assets\\Textures\\stone1.tga");

		LOG_INFO("Graphics initialization complete.");

		return true;
	}

	void Shutdown()
	{
		UnloadResources();
	}

	static void UnloadResources()
	{

	}

	bool ResizeBuffers(UINT width, UINT height) const
	{
		shared_ptr<DXGI_SWAP_CHAIN_DESC> format;
		_D3D->GetSwapChain()->GetDesc(format.get());

		_D3D->GetSwapChain()->ResizeBuffers(1, width, height, format->BufferDesc.Format, 0);

		//delete format;
		return true;
	}

	auto GetSwapChain() const ->ComPtr<IDXGISwapChain>
	{
		return _D3D->GetSwapChain();
	}

	Camera& getCamera()
	{
		return this->_camera;
	}

	DXGI_SWAP_CHAIN_DESC GetSwapChainDesc() const
	{
		return _D3D->GetSwapChainDesc();
	}

	shared_ptr<NimbleD3D> GetD3D() const
	{
		return _D3D;
	}

	void SetDT(float new_dt)
	{
		this->_dt = new_dt;
	}

	float GetDT() const
	{
		return _dt;
	}

	shared_ptr<Terrain> GetTerrain() const
	{
		return this->terrain;
	}

	const Light& GetLight() const
	{
		return _light0;
	}

	void SetDebugLine(Vector3 p1, Vector3 p2)
	{
		_line.SetLine(_D3D->GetDevice().Get(), p1, p2);
	}

	RayHit IsRayIntersectingTerrain(Ray r) const
	{
		return terrain->IsRayIntersectingTerrain(r);
	}

	void PrintDebugObjects() const
	{
		_D3D->GetDebugLayer()->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
	}

	static string GetStatistics()
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

	void SetLightDirection(float x, float y, float z)
	{
		_light0.SetLightDirection(Vector3(x, y, z));
	}

	void SetAmbientLight(float r, float g, float b)
	{
		_light0.SetAmbientLight(Vector4(r, g, b, 1.0f));
	}

	void SetDiffuseColor(float r, float g, float b)
	{
		_light0.SetDiffuseColor(Vector4(r, g, b, 1.0f));
	}

	bool Draw(RENDER_MODE mode, float dt)
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

	ParticleEngine& GetParticleEngine()
	{
		return particleEngine;
	}

private:
	bool RenderMinimap() const
	{
		_D3D->BeginScene(DirectX::Colors::Black);

		_D3D->EndScene();
		return true;
	}

	bool RenderViewport()
	{
		// Clear the buffers to begin the scene.
		_D3D->BeginScene(DirectX::Colors::Black);

		_camera.Update();

		auto projectionMatrix = _D3D->GetProjectionMatrix();
		auto viewMatrix = _camera.GetViewMatrix();

		_frustum.ConstructFrustum(1000.0f, projectionMatrix, viewMatrix);
		//_line->Draw(_D3D->GetDeviceContext().Get(), viewMatrix, projectionMatrix);

		terrain->Draw(_D3D->GetDevice(), _D3D->GetDeviceContext(), viewMatrix, projectionMatrix, _light0, _frustum);

		// draw every line
		auto& lines = DebugLineManager::GetInstance().GetLines();

		for (auto& line : lines)
		{
			line.Draw(_D3D->GetDeviceContext().Get(), viewMatrix, projectionMatrix);
		}

		//particleSystem->Update(*viewMatrix, *projectionMatrix, _dt);
		//particleSystem->Draw(_D3D->GetDevice(), _D3D->GetDeviceContext());
		//particleEngine->Update(*viewMatrix, *projectionMatrix, _dt);
		//particleEngine->Draw(_D3D->GetDevice(), _D3D->GetDeviceContext());


		//StartDebugTimer();
		_D3D->EndScene();
		//EndDebugTimer("PS: ");

		return true;
	}

	void CompileShaders() const
	{
		ShaderManager::GetInstance().Load(_D3D->GetDevice().Get());
	}

	static void LoadVertexBuffers()
	{
		
	}

private:
	shared_ptr<NimbleD3D> _D3D;
	Camera _camera;
	shared_ptr<Terrain> terrain;
	Light _light0;
	Frustum _frustum;
	LineDrawable _line;
	ParticleEngine particleEngine;

	// How much time has passed since the last frame, used for smooth interpolation
	float _dt;
	float _currentTime;
};