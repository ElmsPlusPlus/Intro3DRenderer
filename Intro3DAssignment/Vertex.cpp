#include "StdAfx.h"
#include "Vertex.h"

// Constructors

Vertex::Vertex(void)
{
	Init(0.0f, 0.0f, 0.0f, 1.0f);
}

Vertex::Vertex(float x, float y, float z, float w)
{
	Init(x, y, z, w);
}

Vertex::Vertex(const Vertex& v)
{
	Copy(v);
}

Vertex::~Vertex(void)
{
}

// Accessors and mutators

float Vertex::GetX(void) const
{
	return _x;
}

void Vertex::SetX(float x)
{
	_x = x;
}

inline float Vertex::GetY(void) const
{
	return _y;
}

void Vertex::SetY(float y)
{
	_y = y;
}

float Vertex::GetZ(void) const
{
	return _z;
}

void Vertex::SetZ(float z)
{
	_z = z;
}

float Vertex::GetW(void) const
{
	return _w;
}

void Vertex::SetW(float w)
{
	_w = w;
}

inline Gdiplus::Color Vertex::GetColour(void) const
{
	return _colour;
}

void Vertex::SetColour(Gdiplus::Color colour)
{
	_colour = colour;
}

Vector3D Vertex::GetNormal(void) const
{
	return _normal;
}

void Vertex::SetNormal(Vector3D normal)
{
	_normal = normal;
}

int Vertex::GetCount(void) const
{
	return _contributingCount;
}

void Vertex::SetCount(int count)
{
	_contributingCount = count;
}

int Vertex::GetUVIndex(void) const
{
	return _UVIndex;
}

void Vertex::SetUVIndex(int index)
{
	_UVIndex = index;
}

// Operator overloads

Vertex& Vertex::operator= (const Vertex &rhs)
{
	if (this != &rhs)
	{
		// Only copy if we are not assigning to ourselves. 
		// (remember that a = b is the same as a.operator=(b))
		Copy(rhs);
	}
	return *this;
}

const Vertex Vertex::operator+(const Vertex &other) const
{
	Vertex result;
	result.SetX(_x + other.GetX());
	result.SetY(_y + other.GetY());
	result.SetZ(_z + other.GetZ());
	result.SetW(_w + other.GetW());
	return result;
}

const Vertex Vertex::operator-(const Vertex &other) const
{
	Vertex result;
	result.SetX(_x - other.GetX());
	result.SetY(_y - other.GetY());
	result.SetZ(_z - other.GetZ());
	result.SetW(_w - other.GetW());
	return result;
}

// Private methods

void Vertex::Init(float x, float y, float z, float w)
{
	_x = x;
	_y = y;
	_z = z;
	_w = w;
}

void Vertex::Copy(const Vertex& v)
{
	_x = v.GetX();
	_y = v.GetY();
	_z = v.GetZ();
	_w = v.GetW();
	_colour = v.GetColour();
	_normal = v.GetNormal();
	_contributingCount = v.GetCount();
	_UVIndex = v.GetUVIndex();
}

void Vertex::Dehomogenize()
{
	_x = _x / _w;
	_y = _y / _w;
	_z = _z / _w;
	_w = _w /_w;
}

// Other Methods

Vector3D Vertex::ReturnVector(Vertex vert1, Vertex vert2)
{
	Vector3D vector;

	float x = vert1.GetX() - vert2.GetX();
	float y = vert1.GetY() - vert2.GetY();
	float z = vert1.GetZ() - vert2.GetZ();

	vector.SetX(x);
	vector.SetY(y);
	vector.SetZ(z);
	vector.SetLength();
	return vector;
}

void Vertex::IncrementCount()
{
	_contributingCount++;
}


