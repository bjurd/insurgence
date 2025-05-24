#pragma once

#include "vector.h"
#include "matrix.h"
#include "mathfunc.h"

void CrossProduct(const Vector& A, const Vector& B, Vector& Cross);
float __fastcall VectorNormalize(Vector& Target);

void VectorVectors(const Vector& Forward, Vector& Right, Vector& Up);
void VectorAngles(const Vector& Forward, Angle& Angles);

void AngleVectors(const Angle& Angles, Vector& Forward);
void AngleVectors(const Angle& Angles, Vector& Forward, Vector& Right, Vector& Up);

int FrustumTransform(const VMatrix& WorldToSurface, const Vector& Point, Vector& Screen);
int ScreenTransform(const Vector& Point, Vector& Screen);