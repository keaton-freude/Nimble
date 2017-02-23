//#pragma once
//
//#include <d3d11.h>
//#include "ShaderManager.h"
//#include "Mesh.h"
//#include "Transform.h"
//#include "TerrainVertex.h"
//
//using std::shared_ptr;
//using Microsoft::WRL::ComPtr;
//
//class TerrainMesh
//{
//public:
//    TerrainMesh() = default;
//    TerrainMesh(D3DDevice device,
//                vector<TerrainVertex>& vertices,
//                vector<unsigned long>& indices);
//
//    TerrainMesh(const TerrainMesh& other) {}
//
//    // TerrainMesh& operator=(const TerrainMesh& rhs)
//    //{
//    //	return *this;
//    //}
//
//    TerrainMesh& operator=(TerrainMesh&& rhs)
//    {
//        //this->mesh = move(rhs.mesh);
//		mesh.swap(rhs.mesh);
//        this->transform = rhs.transform;
//        this->terrain_shader = rhs.terrain_shader;
//
//		return *this;
//    }
//
//    ~TerrainMesh();
//
//    void Draw(D3DDeviceContext deviceContext, int indexCount);
//
//    Mesh* GetVertexBuffer()
//    {
//        return mesh.get();
//    }
//
//private:
//    unique_ptr<Mesh> mesh;
//    Transform transform;
//    shared_ptr<TerrainShader> terrain_shader;
//};