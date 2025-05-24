#pragma once

#include "mathfunc.h"

class Vector
{
public:
	float x, y, z;

public:
	Vector()
	{
		this->x = 0.f;
		this->y = 0.f;
		this->z = 0.f;
	}

	Vector(float x, float y, float z)
	{
		this->x = x;
		this->y = y;
		this->z = z;
	}

	Vector(const Vector& Other)
	{
		this->x = Other.x;
		this->y = Other.y;
		this->z = Other.z;
	}
};

class Angle
{
public:
	float p, y, r;

public:
	Angle()
	{
		this->p = 0.f;
		this->y = 0.f;
		this->r = 0.f;
	}

	Angle(float p, float y, float r)
	{
		this->p = p;
		this->y = y;
		this->r = r;
	}

	Angle(const Angle& Other)
	{
		this->p = Other.p;
		this->y = Other.y;
		this->r = Other.r;
	}
};

class Quaternion
{
public:
	float x, y, z, w;
};

class RadianEuler
{
public:
	float x, y, z;

public:
	Angle ToAngle()
	{
		return Angle(RAD2DEG(x), RAD2DEG(y), RAD2DEG(z));
	}
};