#pragma once
#define _CRTDBG_MAP_ALLOC
#define NOMINMAX
#include <stdlib.h>
#include <crtdbg.h>
#include <Windows.h>
#include <memory>
#include <wrl\client.h>
#include "..\..\..\Externals\Bitmap\bitmap_image.hpp"

#include "NimbleD3D.h"
#include "ColorShader.h"
#include "RayHit.h"
#include "Frustum.h"
#include "Camera.h"
#include "SimpleMath.h"
#include "ColorDrawable.h"
#include "ParticleSystem.h"
#include "Terrain.h"
#include "Light.h"
#include "LineDrawable.h"
#include "Logger.h"
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
	~Graphics();

	void HeightmapAdd(Vector3 location, float radius, float intensity);

	bool Init(int screenWidth, int screenHeight, HWND hwnd, bool fullscreen,
		bool vSyncEnabled, float screenDepth, float screenNear);
	
	void Shutdown();
	void UnloadResources();
	bool ResizeBuffers(UINT width, UINT height);
	auto GetSwapChain()-> ComPtr<IDXGISwapChain>;
	shared_ptr<Camera> getCamera();
	DXGI_SWAP_CHAIN_DESC GetSwapChainDesc();
	shared_ptr<NimbleD3D> GetD3D();
	void SetDT(float new_dt);
	float GetDT();
	shared_ptr<Terrain> GetTerrain();
	shared_ptr<Light> GetLight();
	void SetDebugLine(Vector3 p1, Vector3 p2);
	RayHit IsRayIntersectingTerrain(Ray r);
	void PrintDebugObjects();
	string GetStatistics();
	void SetLightDirection(float x, float y, float z);
	void SetAmbientLight(float r, float g, float b);
	void SetDiffuseColor(float r, float g, float b);
	bool Draw(RENDER_MODE mode, float dt);
	shared_ptr<ParticleEngine> GetParticleEngine();

private:
	bool RenderMinimap();
	bool RenderViewport();
	void CompileShaders();
	

	void LoadVertexBuffers();

private:
	shared_ptr<NimbleD3D> _D3D;
	shared_ptr<Camera> _camera;
	shared_ptr<Terrain> terrain;
	shared_ptr<Light> _light0;
	shared_ptr<Frustum> _frustum;
	shared_ptr<LineDrawable> _line;
	shared_ptr<ParticleEngine> particleEngine;

	// How much time has passed since the last frame, used for smooth interpolation
	float _dt;
	float _currentTime;
};