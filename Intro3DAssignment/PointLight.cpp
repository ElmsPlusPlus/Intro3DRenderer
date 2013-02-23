#include "StdAfx.h"
#include "PointLight.h"

// Constructors

PointLight::PointLight(void)
{
}

PointLight::PointLight(Gdiplus::Color colour, Vertex location, float atCoA, float atCoB, float atCoC)
{
	_redIntensity = colour.GetRed();
	_greenIntensity = colour.GetGreen();
	_blueIntensity = colour.GetBlue();
	_attenuationA = atCoA;
	_attenuationB = atCoB;
	_attenuationC = atCoC;
	_lightLocation = location;
}

PointLight::~PointLight(void)
{
}


// Accessors and Mutators

float PointLight::GetRedIntensity(void) const
{
	return _redIntensity;
}

void PointLight::SetRedIntensity(float redIntensity)
{
	_redIntensity = redIntensity;
}

float PointLight::GetGreenIntensity(void) const
{
	return _greenIntensity;
}

void PointLight::SetGreenIntensity(float greenIntensity)
{
	_greenIntensity = greenIntensity;
}

float PointLight::GetBlueIntensity(void) const
{
	return _blueIntensity;
}

void PointLight::SetBlueIntensity(float blueIntensity)
{
	_blueIntensity = blueIntensity; 
}

float PointLight::GetAttenuationA(void) const
{
	return _attenuationA;
}

void PointLight::SetAttenuationA(float atCoA)
{
	_attenuationA = atCoA;
}

float PointLight::GetAttenuationB(void) const
{
	return _attenuationB;
}

void PointLight::SetAttenuationB(float atCoB)
{
	_attenuationB = atCoB;
}

float PointLight::GetAttenuationC(void) const
{
	return _attenuationC;
}

void PointLight::SetAttenuationC(float atCoC)
{
	_attenuationC = atCoC;
}

Vertex PointLight::GetLightLocation(void) const
{
	return _lightLocation;
}

void PointLight::SetLightLocation(Vertex location)
{
	_lightLocation = location;
}
