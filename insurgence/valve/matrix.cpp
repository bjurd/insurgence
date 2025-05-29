#include "matrix.h"

#include "vector.h"
#include "math.h"

void VMatrix::SetIdentity()
{
	m[0][0] = 1.0f; m[0][1] = 0.0f; m[0][2] = 0.0f; m[0][3] = 0.0f;
	m[1][0] = 0.0f; m[1][1] = 1.0f; m[1][2] = 0.0f; m[1][3] = 0.0f;
	m[2][0] = 0.0f; m[2][1] = 0.0f; m[2][2] = 1.0f; m[2][3] = 0.0f;
	m[3][0] = 0.0f; m[3][1] = 0.0f; m[3][2] = 0.0f; m[3][3] = 1.0f;
}

void VMatrix::SetForward(const Vector& Forward)
{
	m[0][0] = Forward.x;
	m[1][0] = Forward.y;
	m[2][0] = Forward.z;
}

void VMatrix::SetLeft(const Vector& Left)
{
	m[0][1] = Left.x;
	m[1][1] = Left.y;
	m[2][1] = Left.z;
}

void VMatrix::SetUp(const Vector& Up)
{
	m[0][2] = Up.x;
	m[1][2] = Up.y;
	m[2][2] = Up.z;
}

void VMatrix::GetBasisVectors(Vector& Forward, Vector& Left, Vector& Up)
{
	Forward.x = m[0][0];
	Forward.y = m[1][0];
	Forward.z = m[2][0];

	Left.x = m[0][1];
	Left.y = m[1][1];
	Left.z = m[2][1];

	Up.x = m[0][2];
	Up.y = m[1][2];
	Up.z = m[2][2];
}

void VMatrix::SetBasisVectors(const Vector& Forward, const Vector& Left, const Vector& Up)
{
	SetForward(Forward);
	SetLeft(Left);
	SetUp(Up);
}

void VMatrix::GetTranslation(Vector& Translation)
{
	Translation.x = m[0][3];
	Translation.y = m[1][3];
	Translation.z = m[2][3];
}

void VMatrix::SetTranslation(const Vector& Translation)
{
	m[0][3] = Translation.x;
	m[1][3] = Translation.y;
	m[2][3] = Translation.z;
}

void VMatrix::GetAngles(Angle& Angles)
{
	Vector Forward, Left, Up;
	this->GetBasisVectors(Forward, Left, Up);

	VectorAngles(Forward, Angles);

	float d1 = DotProduct(Left, Up);
	float d2 = DotProduct(Up, Up);

	Angles.r = RAD2DEG(atan2f(d1, d2));
}

D3DMATRIX VMatrix::ToDirectX()
{
	D3DMATRIX DirectX;
	memcpy(&DirectX, this->m, sizeof(DirectX));

	return DirectX;
}
