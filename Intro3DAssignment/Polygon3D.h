#pragma once
#include <GdiPlus.h>
#include "Texel.h"
class Polygon3D
{
public:
	Polygon3D(void);
	Polygon3D(int indices[3]);
	~Polygon3D(void);

	int GetI(int) const;
	void SetI(int, int);
	Texel GetTexel(int) const ;
	void SetTexel(int, float, float);
	float GetAverageZ(void) const;
	void SetAverageZ(float averageZ);
	bool GetCulled(void) const;
	void SetCulled(bool);
	Vector3D GetNormal(void) const;
	void SetNormal(Vector3D);
	Gdiplus::Color GetColor(void) const;
	void SetColor(Gdiplus::Color);

	const bool operator<(const Polygon3D &other) const;

private:
	int _indices[3];
	Texel _texels[3];
	bool _culled;
	float _averageZ;
	Gdiplus::Color _colour;
	Vector3D _normal;
};

