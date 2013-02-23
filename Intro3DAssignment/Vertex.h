#pragma once
#include "Vector3D.h"

// Class representing a 2D vertex (i.e a point in two dimensional space)
 
class Vertex
{
	public:
		Vertex(void);
		Vertex(float x, float y, float z, float w);
		Vertex(const Vertex& v);
		~Vertex(void);

		float GetX(void) const;
		void SetX(float x);
		inline float GetY(void) const;
		void SetY(float y);
		float GetZ(void) const;
		void SetZ(float z);
		float GetW(void) const;
		void SetW(float w);
		inline Gdiplus::Color GetColour(void) const;
		void SetColour(Gdiplus::Color);
		Vector3D GetNormal(void) const;
		void SetNormal(Vector3D);
		int GetCount(void) const;
		void SetCount(int);
		void IncrementCount();
		int GetUVIndex(void) const;
		void SetUVIndex(int);

		Vertex& operator= (const Vertex &rhs);

		const Vertex operator+(const Vertex &other) const;
		const Vertex operator-(const Vertex &other) const;

		void Dehomogenize();

		static Vector3D ReturnVector(Vertex vert1, Vertex vert2);

	private:
		float _x;
		float _y;
		float _z;
		float _w;
		Gdiplus::Color _colour;
		Vector3D _normal;
		int _contributingCount;
		int _UVIndex;

		void Init(float x, float y, float z, float w);
		void Copy(const Vertex& v);
};

