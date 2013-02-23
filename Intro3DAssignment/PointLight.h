#pragma once
class PointLight
{
public:
	// Constructors

	PointLight(void);
	PointLight(Gdiplus::Color, Vertex, float, float, float);
	~PointLight(void);

	// Accessors and Mutators

	float GetRedIntensity(void) const;
	void SetRedIntensity(float);
	float GetGreenIntensity(void) const;
	void SetGreenIntensity(float);
	float GetBlueIntensity(void) const;
	void SetBlueIntensity(float);
	float GetAttenuationA(void) const;
	void SetAttenuationA(float);
	float GetAttenuationB(void) const;
	void SetAttenuationB(float);
	float GetAttenuationC(void) const;
	void SetAttenuationC(float);
	Vertex GetLightLocation(void) const;
	void SetLightLocation(Vertex);

private:
	// RGB values for the point light

	float _redIntensity;
	float _greenIntensity;
	float _blueIntensity;

	// a,b,c values for the point light calculations

	float _attenuationA;
	float _attenuationB;
	float _attenuationC;

	// Location of the point light

	Vertex _lightLocation;
};

