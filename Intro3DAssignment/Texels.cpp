#include "StdAfx.h"
#include "Texel.h"


Texel::Texel(void)
{
	_u = 0.0f;
	_v = 0.0f;
}

Texel::Texel(float u, float v)
{
	_u = u;
	_v = v;
}

Texel::~Texel(void)
{
}

float Texel::GetU(void) const
{
	return _u;
}

void Texel::SetU(float u)
{
	_u = u;
}

float Texel::GetV(void) const
{
	return _v;
}

void Texel::SetV(float v)
{
	_v = v;
}
