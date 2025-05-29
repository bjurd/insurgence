#pragma once

#include "vector.h"
#include "matrix.h"
#include "mathfunc.h"

struct cplane_t
{
	Vector	normal;
	float	dist;
	char	type;
	char	signbits;
	char	pad[2];
};

#define CPLANE_NORMAL_X			0
#define CPLANE_NORMAL_Y			4
#define CPLANE_NORMAL_Z			8
#define CPLANE_DIST				12
#define CPLANE_TYPE				16
#define CPLANE_SIGNBITS			17
#define CPLANE_PAD0				18
#define CPLANE_PAD1				19

#define	PLANE_X			0
#define	PLANE_Y			1
#define	PLANE_Z			2

#define	PLANE_ANYX		3
#define	PLANE_ANYY		4
#define	PLANE_ANYZ		5

void CrossProduct(const Vector& A, const Vector& B, Vector& Cross);
float __fastcall VectorNormalize(Vector& Target);

float DotProduct(const Vector& A, const Vector& B);
void VectorVectors(const Vector& Forward, Vector& Right, Vector& Up);
void VectorAngles(const Vector& Forward, Angle& Angles);
void VectorRotate(const Vector& Target, const matrix3x4_t& Rotation, Vector& Output);
void VectorRotate(const Vector& Target, const Angle& Rotation, Vector& Output);

void AngleVectors(const Angle& Angles, Vector& Forward);
void AngleVectors(const Angle& Angles, Vector& Forward, Vector& Right, Vector& Up);
void AngleMatrix(const Angle& Angles, matrix3x4_t& Matrix);

int FrustumTransform(const VMatrix& WorldToSurface, const Vector& Point, Vector& Screen);
int ScreenTransform(const Vector& Point, Vector& Screen);