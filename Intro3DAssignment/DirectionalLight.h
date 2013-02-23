#pragma once
#include "Vector3D.h"
class DirectionalLight
{
public:
	// Constructors

	DirectionalLight(void);
	DirectionalLight(Gdiplus::Color, Vector3D);
	~DirectionalLight(void);

	// Accessors and Mutators

	float GetRedIntensity(void) const;
	void SetRedIntensity(float);
	float GetGreenIntensity(void) const;
	void SetGreenIntensity(float);
	float GetBlueIntensity(void) const;
	void SetBlueIntensity(float);
	Vector3D GetLightDirection(void) const;
	void SetLightDirection(Vector3D);

private:
	// RGB values of the directional light

	float _redIntensity;
	float _greenIntensity;
	float _blueIntensity;

	// Direction of the light (in vector form)

	Vector3D _lightDirection;
};

