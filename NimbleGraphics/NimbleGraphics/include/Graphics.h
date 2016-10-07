#pragma once
#define _CRTDBG_MAP_ALLOC
#define NOMINMAX
#include <Windows.h>
#include <memory>
#include <wrl/client.h>

#include "NimbleD3D.h"
#include "RayHit.h"
#include "Frustum.h"
#include "Camera.h"
#include "SimpleMath.h"
#include "Terrain.h"
#include "Light.h"
#include "ParticleEngine.h"
#include <CommonStates.h>

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
	Graphics();
	Graphics(int screenWidth, int screenHeight, HWND hwnd, bool fullscreen, bool vSyncEnabled,
		float screenDepth, float screenNear);
	~Graphics();

	void HeightmapAdd(Vector3 location, float radius, float intensity);

	bool ResizeBuffers(UINT width, UINT height) const;

	auto GetSwapChain() const -> ComPtr<IDXGISwapChain>;
	Camera& GetCamera();
	DXGI_SWAP_CHAIN_DESC GetSwapChainDesc() const;
	NimbleD3D& GetD3D();
	void SetDT(float new_dt);
	float GetDT() const;
	shared_ptr<Terrain> GetTerrain() const;
	const Light& GetLight() const;

	RayHit IsRayIntersectingTerrain(Ray r) const;

	void PrintDebugObjects() const;
	static string GetStatistics();

	void SetLightDirection(float x, float y, float z);
	void SetAmbientLight(float r, float g, float b);
	void SetDiffuseColor(float r, float g, float b);
	bool Draw(RENDER_MODE mode, float dt);
	ParticleEngine& GetParticleEngine();

private:
	bool RenderMinimap() const;
	bool RenderViewport();
	bool Init(int screenWidth, int screenHeight, HWND hwnd, bool fullscreen,
		bool vSyncEnabled, float screenDepth, float screenNear);

private:
	NimbleD3D _D3D;
	Camera _camera;
	shared_ptr<Terrain> terrain;
	Light _light0;
	Frustum _frustum;
	ParticleEngine particleEngine;

	// How much time has passed since the last frame, used for smooth interpolation
	float _dt;
	float _currentTime;
};