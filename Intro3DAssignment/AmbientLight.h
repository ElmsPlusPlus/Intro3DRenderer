#pragma once
class AmbientLight
{
public:
	// Constructors

	AmbientLight(void);
	AmbientLight(Gdiplus::Color);
	~AmbientLight(void);

	// Accessors and Mutators

	float GetRedIntensity(void) const;
	void SetRedIntensity(float);
	float GetGreenIntensity(void) const;
	void SetGreenIntensity(float);
	float GetBlueIntensity(void) const;
	void SetBlueIntensity(float);

private:
	// RGB values for the ambient light

	float _redIntensity;
	float _greenIntensity;
	float _blueIntensity;
};

