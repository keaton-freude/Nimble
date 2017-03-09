#pragma once
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
	TerrainCell(TerrainVertex& upperLeft, TerrainVertex& upperRight, TerrainVertex& bottomLeft, TerrainVertex& bottomRight);

	/* This face is upperLeft, upperRight1, bottomLeft1 */
	void UpdateFaceNormal1();

	/* This face is bottomLeft, upperRight2, bottomRight2*/
	void UpdateFaceNormal2();

	void ClearNormals();

	void SetNormal();

	void NormalizeNormals();

	const Vector3& GetFaceNormal1() const;

	const Vector3& GetFaceNormal2() const;

	Vector3 GetAveragePosition() const;
};