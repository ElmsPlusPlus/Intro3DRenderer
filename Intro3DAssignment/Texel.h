#pragma once
class Texel
{
public:
	Texel(void);
	Texel(float,float);
	~Texel(void);

	float GetU(void) const;
	void SetU(float);
	float GetV(void) const;
	void SetV(float);

private:

	float _u;
	float _v;
};

