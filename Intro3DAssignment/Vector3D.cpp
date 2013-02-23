#include "StdAfx.h"
#include "Vector3D.h"


// Constructors

Vector3D::Vector3D(void)
{
	Init(0.0f, 0.0f, 0.0f);
	Vector3D::SetLength();
}

Vector3D::Vector3D(float x, float y, float z)
{
	Init(x, y, z);
	Vector3D::SetLength();
}

Vector3D::~Vector3D(void)
{
}

// Accessors and Mutators

float Vector3D::GetX(void) const
{
	return _x;
}

void Vector3D::SetX(float x)
{
	_x = x;
}

float Vector3D::GetY(void) const
{
	return _y;
}

void Vector3D::SetY(float y)
{
	_y = y;
}

float Vector3D::GetZ(void) const
{
	return _z;
}

void Vector3D::SetZ(float z)
{
	_z = z;
}

float Vector3D::GetLength(void)
{
	return _Length;
}

void Vector3D::SetLength()
{
	float xSqr = _x * _x;
	float ySqr = _y * _y;
	float zSqr = _z * _z;

	_Length = sqrt((xSqr + ySqr +zSqr)); 
}

// Operator Overloads

const Vector3D Vector3D::operator+(const Vector3D &other) const
{
	Vector3D result;

	result.SetX(_x + other.GetX());
	result.SetY(_y + other.GetY());
	result.SetZ(_z + other.GetZ());
	result.SetLength();

	return result;
}

Vector3D& Vector3D::operator= (const Vector3D &rhs)
{
	if (this != &rhs)
	{
		// Only copy if we are not assigning to ourselves. 
		// (remember that a = b is the same as a.operator=(b))
		Copy(rhs);
	}
	return *this;
}

// Private Methods

void Vector3D::Init(float x, float y, float z)
{
	_x = x;
	_y = y;
	_z = z;
}

void Vector3D::Copy(const Vector3D& v)
{
	_x = v.GetX();
	_y = v.GetY();
	_z = v.GetZ();
	Vector3D::SetLength();
}

// Static Methods

Vector3D Vector3D::Normalise(Vector3D &vector)
{
	Vector3D normVect;

	normVect.SetX(vector.GetX() / vector.GetLength());
	normVect.SetY(vector.GetY() / vector.GetLength());
	normVect.SetZ(vector.GetZ() / vector.GetLength());
	normVect.SetLength();

	return normVect;
}

float Vector3D::DotProduct(Vector3D &vect1, Vector3D &vect2)
{
	float dotProduct;
	
	float dotX = ((vect1.GetX() * vect2.GetX()));
	float dotY = ((vect1.GetY() * vect2.GetY()));
	float dotZ = ((vect1.GetZ() * vect2.GetZ()));

	dotProduct = dotX + dotY + dotZ;

	return dotProduct;
}

Vector3D Vector3D::CrossProduct(Vector3D &vect1, Vector3D &vect2)
{
	Vector3D crossProduct;

	float crossX = ((vect1.GetY() * vect2.GetZ()) - (vect1.GetZ() * vect2.GetY()));
	float crossY = ((vect1.GetZ() * vect2.GetX()) - (vect1.GetX() * vect2.GetZ()));
	float crossZ = ((vect1.GetX() * vect2.GetY()) - (vect1.GetY() * vect2.GetX()));

	crossProduct.SetX(crossX);
	crossProduct.SetY(crossY);
	crossProduct.SetZ(crossZ);
	crossProduct.SetLength();

	return crossProduct;
}
