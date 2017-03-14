#pragma once
#include "TileCell.h"

TileCell::TileCell(TileVertex& upperLeft, TileVertex& upperRight, TileVertex& bottomLeft, TileVertex& bottomRight)
	: data(upperLeft, upperRight, bottomLeft, bottomRight)
{
}

/* This face is upperLeft, upperRight1, bottomLeft1 */
void TileCell::UpdateFaceNormal1()
{
	Vector3 P1 = data.upperLeft.position;
	Vector3 P2 = data.upperRight.position;
	Vector3 P3 = data.bottomLeft.position;

	Vector3 U = P2 - P1;
	Vector3 V = P3 - P1;

	FaceNormal1 = U.Cross(V);
}

/* This face is bottomLeft, upperRight2, bottomRight2*/
void TileCell::UpdateFaceNormal2()
{
	Vector3 P1 = data.bottomLeft.position;
	Vector3 P2 = data.upperRight.position;
	Vector3 P3 = data.bottomRight.position;

	Vector3 U = P2 - P1;
	Vector3 V = P3 - P1;

	FaceNormal2 = U.Cross(V);
}

void TileCell::ClearNormals()
{
	data.upperLeft.normal = Vector3::Zero;
	data.upperRight.normal = Vector3::Zero;
	data.bottomRight.normal = Vector3::Zero;
	data.bottomLeft.normal = Vector3::Zero;
}

void TileCell::SetNormal()
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

void TileCell::NormalizeNormals()
{
	data.upperLeft.normal.Normalize();
	data.upperRight.normal.Normalize();
	data.bottomLeft.normal.Normalize();
	data.bottomRight.normal.Normalize();
}

const Vector3& TileCell::GetFaceNormal1() const
{
	return FaceNormal1;
}

const Vector3& TileCell::GetFaceNormal2() const
{
	return FaceNormal2;
}

Vector3 TileCell::GetAveragePosition() const
{
	return data.GetAveragePosition();
}
