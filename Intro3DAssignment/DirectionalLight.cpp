#include "StdAfx.h"
#include "DirectionalLight.h"

// Constructors

DirectionalLight::DirectionalLight(void)
{
}

DirectionalLight::DirectionalLight(Gdiplus::Color colour, Vector3D direction)
{
	_redIntensity = colour.GetRed();
	_greenIntensity = colour.GetGreen();
	_blueIntensity = colour.GetBlue();
	_lightDirection = direction;
}

DirectionalLight::~DirectionalLight(void)
{
}


// Accessors and Mutators

float DirectionalLight::GetRedIntensity(void) const
{
	return _redIntensity;
}

void DirectionalLight::SetRedIntensity(float redIntensity)
{
	_redIntensity = redIntensity;
}

float DirectionalLight::GetGreenIntensity(void) const
{
	return _greenIntensity;
}

void DirectionalLight::SetGreenIntensity(float greenIntensity)
{
	_greenIntensity = greenIntensity;
}

float DirectionalLight::GetBlueIntensity(void) const
{
	return _blueIntensity;
}

void DirectionalLight::SetBlueIntensity(float blueIntensity)
{
	_blueIntensity = blueIntensity; 
}

Vector3D DirectionalLight::GetLightDirection(void) const
{
	return _lightDirection;
}

void DirectionalLight::SetLightDirection(Vector3D lightDirection)
{
	_lightDirection = lightDirection;
}