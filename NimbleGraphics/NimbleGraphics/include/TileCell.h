#pragma once
#include "TileVertex.h"


struct TerrainData
{
	TileVertex& upperLeft;
	TileVertex& upperRight;
	TileVertex& bottomLeft;
	TileVertex& bottomRight;
	Vector3 m_averagePosition;

	TerrainData(TileVertex& upper_left, TileVertex& upper_right, TileVertex& bottom_left, 
		TileVertex& bottom_right)
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

class TileCell
{
public:
	TerrainData data;
	Vector3 FaceNormal1;
	Vector3 FaceNormal2;

	// TerrainData must be initialized with TileVertex, so we can't allow default construction
	TileCell() = delete;
	TileCell(TileVertex& upperLeft, TileVertex& upperRight, TileVertex& bottomLeft, TileVertex& bottomRight);

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