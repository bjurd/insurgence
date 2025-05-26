#pragma once

#include "vector.h"
#include <d3d9.h>

typedef float matrix3x4_t[3][4];

class VMatrix
{
public:
	float m[4][4];
	
public:
	VMatrix()
	{
		
	}

	VMatrix(const matrix3x4_t& Matrix)
	{
		m[0][0] = Matrix[0][0];
		m[0][1] = Matrix[0][1];
		m[0][2] = Matrix[0][2];
		m[0][3] = Matrix[0][3];

		m[1][0] = Matrix[1][0];
		m[1][1] = Matrix[1][1];
		m[1][2] = Matrix[1][2];
		m[1][3] = Matrix[1][3];

		m[2][0] = Matrix[2][0];
		m[2][1] = Matrix[2][1];
		m[2][2] = Matrix[2][2];
		m[2][3] = Matrix[2][3];

		m[3][0] = 0.f;
		m[3][1] = 0.f;
		m[3][2] = 0.f;
		m[3][3] = 1.f;
	}

	VMatrix(float* Matrix)
	{
		m[0][0] = Matrix[0];
		m[0][1] = Matrix[1];
		m[0][2] = Matrix[2];
		m[0][3] = Matrix[3];
		m[1][0] = Matrix[4];
		m[1][1] = Matrix[5];
		m[1][2] = Matrix[6];
		m[1][3] = Matrix[7];
		m[2][0] = Matrix[8];
		m[2][1] = Matrix[9];
		m[2][2] = Matrix[10];
		m[2][3] = Matrix[11];
		m[3][0] = Matrix[12];
		m[3][1] = Matrix[13];
		m[3][2] = Matrix[14];
		m[3][3] = Matrix[15];
	}

	inline float* operator[](int i)
	{
		return m[i];
	}

	inline const float* operator[](int i) const
	{
		return m[i];
	}

	inline float* Base()
	{
		return &m[0][0];
	}

	inline const float* Base() const
	{
		return &m[0][0];
	}

	void SetIdentity();

	void SetForward(const Vector& Forward);
	void SetLeft(const Vector& Left);
	void SetUp(const Vector& Up);
		 
	void GetBasisVectors(Vector& Forward, Vector& Left, Vector& Up);
	void SetBasisVectors(const Vector& Forward, const Vector& Left, const Vector& Up);
		 
	void GetTranslation(Vector& Translation);
	void SetTranslation(const Vector& Translation);

	void GetAngles(Angle& Angles);

	D3DMATRIX ToDirectX();
};