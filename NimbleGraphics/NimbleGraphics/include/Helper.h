#pragma once
#include <iostream>
#include <CommonStates.h>
#include <SimpleMath.h>
#include <chrono>
#include "Logger.h"
#include "Singleton.h"

using DirectX::SimpleMath::Ray;
using DirectX::CommonStates;
using std::unique_ptr;
using DirectX::SimpleMath::Vector3;
using DirectX::SimpleMath::Color;


inline void SetD3DDebugName(ID3D11DeviceChild* child, const std::string& name)
{
	if (child != nullptr)
	{
		child->SetPrivateData(WKPDID_D3DDebugObjectName, static_cast<UINT>(name.size()), name.c_str());
	}
}

inline float lerp(float value1, float value2, float amount)
{
	return value1 + ((value2 - value1) * amount);
}

inline float frand()
{
	float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	return r;
}

inline float RandomFloat(float a, float b) 
{
	float random = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);

	float diff = b - a;
	float r = random * diff;
	return a + r;
}

// Return a Vector3 whose components are randomly chosen
// based on the components of a and b
// NewVector(Random(a.x, b.x), Random(a.y, b.y), Random(a.z, b.z))
inline Vector3 RandomVector3(const Vector3& a, const Vector3& b)
{
	return Vector3(RandomFloat(a.x, b.x), RandomFloat(a.y, b.y), RandomFloat(a.z, b.z));
}

// Return a Color whos components are randomly chosen
// based on the components of a and b
inline Color RandomColor(const Color& a, const Color& b)
{
	return Color(RandomFloat(a.x, b.x), RandomFloat(a.y, b.y), 
		RandomFloat(a.z, b.z), RandomFloat(a.w, b.w));
}

class StatesHelper: public Singleton<StatesHelper>
{
public:
	inline CommonStates* GetStates() const
	{
		return states.get();
	}

	inline void Load(ComPtr<ID3D11Device> device)
	{
		states.reset(new CommonStates(device.Get()));
	}

private:
	unique_ptr<CommonStates> states;
};

static std::chrono::time_point<std::chrono::system_clock> g_start;
static std::chrono::time_point<std::chrono::system_clock> g_end;

inline static void StartDebugTimer()
{
	g_start = std::chrono::system_clock::now();
}

inline static void EndDebugTimer(const std::string& label = "")
{
	g_end = std::chrono::system_clock::now();

	std::chrono::duration<double> elapsed_seconds = g_end - g_start;
	LOG_INFO(label, elapsed_seconds.count());
}