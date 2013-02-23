#pragma once
#include "Vertex.h"
#include "Model3D.h"

using namespace Gdiplus;

struct ScanLine
{
	float xStart;
	float xEnd;
	float rStart;
	float rEnd;
	float gStart;
	float gEnd;
	float bStart;
	float bEnd;
	float uStart;
	float uEnd;
};

class Rasterizer
{
	public:
		Rasterizer(unsigned int width, unsigned int height);
		~Rasterizer(void);

		unsigned int GetWidth() const;
		unsigned int GetHeight() const;
		Bitmap * GetBitmap() const;

		void Clear(const Color& color);

		void DrawWireFrame(Model3D&);
		void DrawSolidGDIFlat(Model3D&);
		void DrawSolidFlat(Model3D&);
		void FillPolygon(Vertex verts[3], Gdiplus::Color);
		void DrawSolidGouraud(Model3D&);
		void FillPolygonGouraud(Vertex verts[3]);
		void DrawSolidTextured(Model3D&);
		void FillPolygonTextured(Vertex verts[3], Texel texels[3], Model3D&);
		void DrawScanLine(float, float, float, Gdiplus::Color);
		void DrawGouraudScanLine(float, float, float, float, float, float, float, float, float);
		void DrawTexturedLine(Model3D&, float, float, float, float, float, float, float, float, float, float, float, float);
		void DrawString(WCHAR*);
		template<class T> T Interpolate(T x0,T x1,T y0,T y1,T Y);

	private:
		unsigned int _width;
		unsigned int _height;
		Bitmap * _bitmap;
		Graphics * _graphics;

		// Private constructor. Should not be used directly.
		Rasterizer(void);
};

