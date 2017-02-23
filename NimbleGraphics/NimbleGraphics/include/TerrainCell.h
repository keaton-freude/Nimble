#pragma once
#include "Typedefs.h"
#include "TerrainVertex.h"


struct TerrainData
{
	TerrainVertex& upperLeft;
	TerrainVertex& upperRight;
	TerrainVertex& bottomLeft;
	TerrainVertex& bottomRight;
	Vector3 m_averagePosition;

	TerrainData(TerrainVertex& upper_left, TerrainVertex& upper_right, TerrainVertex& bottom_left, 
		TerrainVertex& bottom_right)
		: upperLeft(upper_left), upperRight(upper_right), bottomLeft(bottom_left), bottomRight(bottom_right)
	{
		auto vectorSum = upperLeft.position + upperRight.position + bottomLeft.position + bottomRight.position;

		m_averagePosition =  vectorSum / 4.0f;
	}

	Vector3 GetAveragePosition() const
	{
		return m_averagePosition;
	}


};

class TerrainCell
{
public:
	TerrainData data;
	Vector3 FaceNormal1;
	Vector3 FaceNormal2;

	// TerrainData must be initialized with TerrainVertex, so we can't allow default construction
	TerrainCell() = delete;
	TerrainCell(TerrainVertex& upperLeft, TerrainVertex& upperRight, TerrainVertex& bottomLeft, TerrainVertex& bottomRight)
		: data(upperLeft, upperRight, bottomLeft, bottomRight)
	{
	}

	/* This face is upperLeft, upperRight1, bottomLeft1 */
	void UpdateFaceNormal1()
	{
		Vector3 P1 = data.upperLeft.position;
		Vector3 P2 = data.upperRight.position;
		Vector3 P3 = data.bottomLeft.position;

		Vector3 U = P2 - P1;
		Vector3 V = P3 - P1;

		FaceNormal1 = U.Cross(V);
	}

	/* This face is bottomLeft, upperRight2, bottomRight2*/
	void UpdateFaceNormal2()
	{
		Vector3 P1 = data.bottomLeft.position;
		Vector3 P2 = data.upperRight.position;
		Vector3 P3 = data.bottomRight.position;

		Vector3 U = P2 - P1;
		Vector3 V = P3 - P1;

		FaceNormal2 = U.Cross(V);
	}

	void ClearNormals()
	{
		data.upperLeft.normal = Vector3::Zero;
		data.upperRight.normal = Vector3::Zero;
		data.bottomRight.normal = Vector3::Zero;
		data.bottomLeft.normal = Vector3::Zero;
	}

	void SetNormal()
	{
		// Need to set the normals for all 4 of our Vertices
		UpdateFaceNormal1();
		UpdateFaceNormal2();

		data.upperLeft.normal = FaceNormal1;
		data.upperRight.normal = (FaceNormal1 + FaceNormal2);
		data.upperRight.normal.Normalize();
		data.bottomLeft.normal = (FaceNormal1 + FaceNormal2);
		data.bottomLeft.normal.Normalize();
		data.bottomRight.normal = FaceNormal2;
	}

	void NormalizeNormals()
	{
		data.upperLeft.normal.Normalize();
		data.upperRight.normal.Normalize();
		data.bottomLeft.normal.Normalize();
		data.bottomRight.normal.Normalize();
	}

	const Vector3& GetFaceNormal1() const
	{
		return FaceNormal1;
	}

	const Vector3& GetFaceNormal2() const
	{
		return FaceNormal2;
	}

	Vector3 GetAveragePosition() const
	{
		return data.GetAveragePosition();
	}
};