#pragma once
class Vector3D
{
public:
	Vector3D(void);
	Vector3D(float x, float y, float z);
	~Vector3D(void);

	float GetX(void) const;
	void SetX(float x);
	float GetY(void) const;
	void SetY(float y);
	float GetZ(void) const;
	void SetZ(float z);

	float GetLength(void);
	void SetLength();

	Vector3D& operator= (const Vector3D &rhs);
	const Vector3D operator+(const Vector3D &other) const;

	static Vector3D Normalise(Vector3D &vector);
	static float DotProduct(Vector3D &vect1, Vector3D &vect2);
	static Vector3D CrossProduct(Vector3D &vect1, Vector3D &vect2);

private:
	float _x;
	float _y;
	float _z;
	float _Length;

	void Init(float x, float y, float z);
	void Copy(const Vector3D& v);
};

