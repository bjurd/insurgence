#pragma once

#include "mathfunc.h"

#define INVALID_VECTOR Vector(INFINITY, INFINITY, INFINITY)
#define INVALID_ANGLE Angle(INFINITY, INFINITY, INFINITY)

#define MAKE_INVALID_VECTOR(Name) Vector Name(INFINITY, INFINITY, INFINITY)
#define MAKE_INVALID_ANGLE(Name) Angle Name(INFINITY, INFINITY, INFINITY)

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

	Vector& operator+=(const Vector& v);
	Vector& operator-=(const Vector& v);
	Vector& operator*=(const Vector& v);
	Vector& operator*=(float s);
	Vector& operator/=(const Vector& v);
	Vector& operator/=(float s);
	Vector& operator+=(float fl);
	Vector& operator-=(float fl);
	Vector	operator-(void) const;
	Vector	operator+(const Vector& v) const;
	Vector	operator-(const Vector& v) const;
	Vector	operator*(const Vector& v) const;
	Vector	operator/(const Vector& v) const;
	Vector	operator*(float fl) const;
	Vector	operator/(float fl) const;

public:
	float DistToSqr(const Vector& Other) const
	{
		Vector Delta;

		Delta.x = this->x - Other.x;
		Delta.y = this->y - Other.y;
		Delta.z = this->z - Other.z;

		return Delta.LengthSqr();
	}

	float DistTo(const Vector& Other) const
	{
		return sqrtf(this->DistToSqr(Other));
	}

	float LengthSqr() const
	{
		return (this->x * this->x) + (this->y * this->y) + (this->z * this->z);
	}

	float Length() const
	{
		return sqrtf(this->LengthSqr());
	}

	bool IsValid() const
	{
		return isfinite(this->x) && isfinite(this->y) && isfinite(this->z);
	}

	void Invalidate()
	{
		this->x = INFINITY;
		this->y = INFINITY;
		this->z = INFINITY;
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

public:
	bool IsValid() const
	{
		return isfinite(this->p) && isfinite(this->y) && isfinite(this->r);
	}

	void Invalidate()
	{
		this->p = INFINITY;
		this->y = INFINITY;
		this->r = INFINITY;
	}
};

class __declspec(align(16)) VectorAligned : public Vector
{
public:
	float w;

public:
	VectorAligned()
	{
		this->x = 0.f;
		this->y = 0.f;
		this->z = 0.f;
		this->w = 0.f;
	}

	VectorAligned(float x, float y, float z)
	{
		this->x = x;
		this->y = y;
		this->z = z;
		this->w = 0.f;
	}

	VectorAligned(float x, float y, float z, float w)
	{
		this->x = x;
		this->y = y;
		this->z = z;
		this->w = w;
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

// Operators yay!
inline Vector& Vector::operator+=(const Vector& v)
{
	x += v.x; y += v.y; z += v.z;
	return *this;
}

inline Vector& Vector::operator-=(const Vector& v)
{
	x -= v.x; y -= v.y; z -= v.z;
	return *this;
}

inline Vector& Vector::operator*=(float fl)
{
	x *= fl;
	y *= fl;
	z *= fl;

	return *this;
}

inline Vector& Vector::operator*=(const Vector& v)
{
	x *= v.x;
	y *= v.y;
	z *= v.z;

	return *this;
}

inline Vector& Vector::operator+=(float fl)
{
	x += fl;
	y += fl;
	z += fl;

	return *this;
}

inline Vector& Vector::operator-=(float fl)
{
	x -= fl;
	y -= fl;
	z -= fl;

	return *this;
}

inline Vector& Vector::operator/=(float fl)
{
	float oofl = 1.0f / fl;
	x *= oofl;
	y *= oofl;
	z *= oofl;

	return *this;
}

inline Vector& Vector::operator/=(const Vector& v)
{
	x /= v.x;
	y /= v.y;
	z /= v.z;

	return *this;
}

inline Vector Vector::operator-(void) const
{
	return Vector(-x, -y, -z);
}

inline void VectorClear(Vector& a)
{
	a.x = a.y = a.z = 0.f;
}

inline void VectorClear(VectorAligned& a)
{
	a.x = a.y = a.z = a.w = 0.f;
}

inline void VectorCopy(const Vector& Source, Vector& Destination)
{
	Destination.x = Source.x;
	Destination.y = Source.y;
	Destination.z = Source.z;
}

inline void VectorCopy(const Vector& Source, VectorAligned& Destination)
{
	Destination.x = Source.x;
	Destination.y = Source.y;
	Destination.z = Source.z;
	Destination.w = 0.f;
}

inline void VectorAdd(const Vector& a, const Vector& b, Vector& c)
{
	c.x = a.x + b.x;
	c.y = a.y + b.y;
	c.z = a.z + b.z;
}

inline void VectorSubtract(const Vector& a, const Vector& b, Vector& c)
{
	c.x = a.x - b.x;
	c.y = a.y - b.y;
	c.z = a.z - b.z;
}

inline void VectorMultiply(const Vector& a, float b, Vector& c)
{
	c.x = a.x * b;
	c.y = a.y * b;
	c.z = a.z * b;
}

inline void VectorMultiply(const Vector& a, const Vector& b, Vector& c)
{
	c.x = a.x * b.x;
	c.y = a.y * b.y;
	c.z = a.z * b.z;
}

inline void VectorDivide(const Vector& a, float b, Vector& c)
{
	float oob = 1.0f / b;
	c.x = a.x * oob;
	c.y = a.y * oob;
	c.z = a.z * oob;
}

inline void VectorDivide(const Vector& a, const Vector& b, Vector& c)
{
	c.x = a.x / b.x;
	c.y = a.y / b.y;
	c.z = a.z / b.z;
}

inline Vector Vector::operator+(const Vector& v) const
{
	Vector res;
	VectorAdd(*this, v, res);

	return res;
}

inline Vector Vector::operator-(const Vector& v) const
{
	Vector res;
	VectorSubtract(*this, v, res);

	return res;
}

inline Vector Vector::operator*(float fl) const
{
	Vector res;
	VectorMultiply(*this, fl, res);

	return res;
}

inline Vector Vector::operator*(const Vector& v) const
{
	Vector res;
	VectorMultiply(*this, v, res);

	return res;
}

inline Vector Vector::operator/(float fl) const
{
	Vector res;
	VectorDivide(*this, fl, res);

	return res;
}

inline Vector Vector::operator/(const Vector& v) const
{
	Vector res;
	VectorDivide(*this, v, res);

	return res;
}

inline Vector operator*(float fl, const Vector& v)
{
	return v * fl;
}
