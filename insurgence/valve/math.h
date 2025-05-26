#pragma once

#include "vector.h"
#include "matrix.h"
#include "mathfunc.h"

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