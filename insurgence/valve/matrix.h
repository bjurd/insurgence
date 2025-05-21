#pragma once

#include "vector.h"

typedef float matrix3x4_t[3][4];

class VMatrix
{
public:
	float m[4][4];
	
public:
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

	void		SetLeft(const Vector& Left);
	void		SetUp(const Vector& Up);
	void		SetForward(const Vector& Forward);

	void		GetBasisVectors(Vector& Forward, Vector& Left, Vector& Up);
	void		SetBasisVectors(const Vector& Forward, const Vector& Left, const Vector& Up);

	void		GetTranslation(Vector& Translation);
	void		SetTranslation(const Vector& Translation);
};