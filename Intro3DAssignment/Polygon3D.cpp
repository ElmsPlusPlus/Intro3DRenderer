#include "StdAfx.h"
#include "Polygon3D.h"



//Constructors
Polygon3D::Polygon3D(void)
{

}

Polygon3D::Polygon3D(int indices[3])
{
	for (int i = 0; i < 3; i++)
	{
		_indices[3] = indices[3];
	}
}

Polygon3D::~Polygon3D(void)
{
	
}

//Accessors and Mutators

int Polygon3D::GetI(int i) const
{
	return _indices[i];
}

void Polygon3D::SetI(int i, int j)
{
	_indices[i] = j;
}

Texel Polygon3D::GetTexel(int i) const
{
	return _texels[i];
}

void Polygon3D::SetTexel(int i, float u, float v)
{
	_texels[i].SetU(u);
	_texels[i].SetV(v);
}

bool Polygon3D::GetCulled(void) const
{
	return _culled;
}

void Polygon3D::SetCulled(bool culled)
{
	_culled = culled;
}

float Polygon3D::GetAverageZ(void) const
{
	return _averageZ;
}

void Polygon3D::SetAverageZ(float averageZ)
{
	_averageZ = averageZ;
}

Gdiplus::Color Polygon3D::GetColor(void) const
{
	return _colour;
}

void Polygon3D::SetColor(Gdiplus::Color colour)
{
	_colour = colour;
}

Vector3D Polygon3D::GetNormal(void) const
{
	return _normal;
}

void Polygon3D::SetNormal(Vector3D normal)
{
	_normal = normal;
}

const bool Polygon3D::operator<(const Polygon3D &other) const
{
	if (_averageZ > other.GetAverageZ())
	{
		return true;
	}
	else
	{
		return false;
	}
}
