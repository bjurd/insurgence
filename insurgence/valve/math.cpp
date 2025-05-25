#include "math.h"

#include "../globals.h"
#include <cmath>
#include <cfloat>

void CrossProduct(const Vector& A, const Vector& B, Vector& Cross)
{
	Cross.x = A.y * B.z - A.z * B.y;
	Cross.y = A.z * B.x - A.x * B.z;
	Cross.z = A.x * B.y - A.y * B.z;
}

float __fastcall VectorNormalize(Vector& Target)
{
	float Radius = sqrtf(Target.x * Target.x + Target.y * Target.y + Target.z * Target.z);
	float iRadius = 1.f / (Radius + FLT_EPSILON);

	Target.x *= iRadius;
	Target.y *= iRadius;
	Target.z *= iRadius;

	return Radius;
}

void VectorVectors(const Vector& Forward, Vector& Right, Vector& Up)
{
	Vector Temp;

	if (Forward.x == 0.f && Forward.y == 0.f)
	{
		Right.x = 0.f;
		Right.y = -1.f;
		Right.z = 0.f;

		Up.x = -Forward.z;
		Up.y = 0.f;
		Up.z = 0.f;
	}
	else
	{
		Temp.x = 0.f; Temp.y = 0.f; Temp.z = 1.f;

		CrossProduct(Forward, Temp, Right);
		VectorNormalize(Right);

		CrossProduct(Right, Forward, Up);
		VectorNormalize(Up);
	}
}

void VectorAngles(const Vector& Forward, Angle& Angles)
{
	float Tmp, Yaw, Pitch;

	if (Forward.x == 0.f && Forward.y == 0.f)
	{
		Yaw = 0.f;

		if (Forward.z > 0.f)
			Pitch = 270.f;
		else
			Pitch = 90.f;
	}
	else
	{
		Yaw = RAD2DEG(atan2f(Forward.y, Forward.x));

		if (Yaw < 0.f)
			Yaw += 360.f;

		Tmp = sqrtf(Forward.x * Forward.x + Forward.y * Forward.y);
		Pitch = RAD2DEG(atan2f(-Forward.z, Tmp));

		if (Pitch < 0.f)
			Pitch += 360.f;
	}

	Angles.p = Pitch;
	Angles.y = Yaw;
	Angles.r = 0.f;
}

void AngleVectors(const Angle& Angles, Vector& Forward)
{
	float RadPitch = DEG2RAD(Angles.p);
	float RadYaw = DEG2RAD(Angles.y);

	float Sp = sinf(RadPitch);
	float Cp = cosf(RadPitch);
	float Sy = sinf(RadYaw);
	float Cy = cosf(RadYaw);

	Forward.x = Cp * Cy;
	Forward.y = Cp * Sy;
	Forward.z = -Sp;
}

void AngleVectors(const Angle& Angles, Vector& Forward, Vector& Right, Vector& Up)
{
	float RadPitch = DEG2RAD(Angles.p);
	float RadYaw = DEG2RAD(Angles.y);
	float RadRoll = DEG2RAD(Angles.r);

	float Sp = sinf(RadPitch);
	float Cp = cosf(RadPitch);
	float Sy = sinf(RadYaw);
	float Cy = cosf(RadYaw);
	float Sr = sinf(RadRoll);
	float Cr = cosf(RadRoll);

	Forward.x = Cp * Cy;
	Forward.y = Cp * Sy;
	Forward.z = -Sp;

	Right.x = -Sr * Sp * Cy + Cr * Sy;
	Right.y = -Sr * Sp * Sy - Cr * Cy;
	Right.z = -Sr * Cp;

	Up.x = Cr * Sp * Cy + Sr * Sy;
	Up.y = Cr * Sp * Sy - Sr * Cy;
	Up.z = Cr * Cp;
}

int FrustumTransform(const VMatrix& WorldToSurface, const Vector& Point, Vector& Screen)
{
	Screen.y = WorldToSurface[1][0] * Point.x + WorldToSurface[1][1] * Point.y + WorldToSurface[1][2] * Point.z + WorldToSurface[1][3];
	Screen.x = WorldToSurface[0][0] * Point.x + WorldToSurface[0][1] * Point.y + WorldToSurface[0][2] * Point.z + WorldToSurface[0][3];
	Screen.z = 0.f;

	float Width = WorldToSurface[3][0] * Point.x + WorldToSurface[3][1] * Point.y + WorldToSurface[3][2] * Point.z + WorldToSurface[3][3];

	bool Hidden = Width < 0.001f;

	if (Hidden)
	{
		Screen.x *= 100000;
		Screen.y *= 100000;
	}
	else
	{
		float iWidth = 1.f / Width;

		Screen.x *= iWidth;
		Screen.y *= iWidth;
	}

	int ScreenWidth, ScreenHeight;
	Globals->PointersManager->Client->GetScreenSize(ScreenWidth, ScreenHeight);

	Screen.x = (ScreenWidth * 0.5f) + (Screen.x * 0.5f * ScreenWidth);
	Screen.y = (ScreenHeight * 0.5f) - (Screen.y * 0.5f * ScreenHeight);

	return Hidden;
}

int ScreenTransform(const Vector& Point, Vector& Screen)
{
	return FrustumTransform(Globals->PointersManager->Client->WorldToScreenMatrix(), Point, Screen);
}