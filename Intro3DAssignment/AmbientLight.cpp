#include "StdAfx.h"
#include "AmbientLight.h"

// Constructors

AmbientLight::AmbientLight(void)
{
}

AmbientLight::AmbientLight(Gdiplus::Color colour)
{
	_redIntensity = colour.GetRed();
	_greenIntensity = colour.GetGreen();
	_blueIntensity = colour.GetBlue();
}

AmbientLight::~AmbientLight(void)
{
}


// Accessors and Mutators

float AmbientLight::GetRedIntensity(void) const
{
	return _redIntensity;
}

void AmbientLight::SetRedIntensity(float redIntensity)
{
	_redIntensity = redIntensity;
}

float AmbientLight::GetGreenIntensity(void) const
{
	return _greenIntensity;
}

void AmbientLight::SetGreenIntensity(float greenIntensity)
{
	_greenIntensity = greenIntensity;
}

float AmbientLight::GetBlueIntensity(void) const
{
	return _blueIntensity;
}

void AmbientLight::SetBlueIntensity(float blueIntensity)
{
	_blueIntensity = blueIntensity; 
}
