#include "StdAfx.h"
#include "Rasterizer.h"
#include "Utils.h"

Rasterizer::Rasterizer(void)
{
	// Declared private so should not be called.
	_bitmap = NULL;
	_graphics = NULL;
}

Rasterizer::Rasterizer(unsigned int width, unsigned int height)
{
	// Create a bitmap of the specified size and get a reference to the graphics object
	// for that bitmap. Note that these calls could theoretically fail so we really should
	// handle that, but we will leave that for now. 
	_width = width;
	_height = height;
	_bitmap = new Bitmap(_width, _height, PixelFormat32bppARGB);
	_graphics = new Graphics(_bitmap);
}

Rasterizer::~Rasterizer(void)
{
	// Clean up all dynamically created objects
	if (_graphics)
	{
		delete _graphics;
		_graphics = NULL;
	}
	if (_bitmap)
	{
		delete _bitmap;
		_bitmap = NULL;
	}
}

// Accessors

unsigned int Rasterizer::GetWidth() const
{
	return _width;
}

unsigned int Rasterizer::GetHeight() const
{
	return _height;
}

Bitmap * Rasterizer::GetBitmap() const
{
	return _bitmap;
}

// Clear the bitmap using the specified colour

void Rasterizer::Clear(const Color& color)
{
	if (_graphics)
	{
		_graphics->Clear(color);
	}
}

// Draw the wireframe of a model

void Rasterizer::DrawWireFrame(Model3D& model)
{
	Pen pen(Color::White);

	int polyNum = model.GetNumOfPolys();
	for (int i = 0; i < polyNum; i++)
	{
		Vertex vert1 = model.GetVertex(i, 0);
		Vertex vert2 = model.GetVertex(i, 1);
		Vertex vert3 = model.GetVertex(i, 2);

		if (!model.IsPolygonCulled(i))
		{
			_graphics->DrawLine(&pen, vert1.GetX(), vert1.GetY(), vert2.GetX(), vert2.GetY());
			_graphics->DrawLine(&pen, vert2.GetX(), vert2.GetY(), vert3.GetX(), vert3.GetY());
			_graphics->DrawLine(&pen, vert3.GetX(), vert3.GetY(), vert1.GetX(), vert1.GetY());
		}	
	}
}

void Rasterizer::DrawSolidGDIFlat(Model3D& model)
{
	int polyNum = model.GetNumOfPolys();
	for (int i = 0; i < polyNum; i++)
	{
		PointF points[3];
		points[0].X = model.GetVertex(i, 0).GetX();
		points[0].Y = model.GetVertex(i, 0).GetY();
		points[1].X = model.GetVertex(i, 1).GetX();
		points[1].Y = model.GetVertex(i, 1).GetY();
		points[2].X = model.GetVertex(i, 2).GetX();
		points[2].Y = model.GetVertex(i, 2).GetY();

		if (!model.IsPolygonCulled(i))
		{
			Polygon3D tempPoly = model.GetPolygon(i);
			SolidBrush solBrush(tempPoly.GetColor());
			_graphics->FillPolygon(&solBrush, points, 3);
		}	
	}
}

void Rasterizer::DrawSolidFlat(Model3D& model)
{
	int polyNum = model.GetNumOfPolys();
	for (int i = 0; i < polyNum; i++)
	{
		Vertex tempVerts[3];
		tempVerts[0] = model.GetVertex(i, 0);
		tempVerts[1] = model.GetVertex(i, 1);
		tempVerts[2] = model.GetVertex(i, 2);
		if (!model.IsPolygonCulled(i))
		{
			Rasterizer::FillPolygon(tempVerts, model.GetPolygon(i).GetColor());
		}	
	}
}

void Rasterizer::FillPolygon(Vertex verts[3], Gdiplus::Color colour)
{
	ScanLine* scanLines = new ScanLine[_height];

	while(verts[0].GetY() > verts[1].GetY() || verts[1].GetY() > verts[2].GetY())
	{
		Vertex tempVert;

		if(verts[0].GetY() > verts[1].GetY())
		{
			/*tempVert = verts[0];
			verts[0] = verts[1];
			verts[1] = tempVert;*/

			std::swap(verts[0], verts[1]);
		}

		if(verts[1].GetY() > verts[2].GetY())
		{
			/*tempVert = verts[1];
			verts[1] = verts[2];
			verts[2] = tempVert;*/
			std::swap(verts[1], verts[2]);
		}

		if(verts[0].GetY() > verts[1].GetY())
		{
			/*tempVert = verts[0];
			verts[0] = verts[1];
			verts[1] = tempVert;*/
			std::swap(verts[0], verts[1]);
		}
	}

	// Loop 1: AC[0][2] and AB[0][1]

	int yStart = (int)verts[0].GetY();
	int yEnd = (int)verts[1].GetY();
	float currentY = verts[0].GetY();

	for(int i = yStart; i < yEnd; i++)
	{
		/*scanLines->xStart = verts[0].GetX() + ((currentY - verts[0].GetY()) * ((verts[2].GetX() - verts[0].GetX()) / 
			(verts[2].GetY() - verts[0].GetY())));

		scanLines->xEnd = verts[0].GetX() + ((currentY - verts[0].GetY()) * ((verts[1].GetX() - verts[0].GetX()) / 
			(verts[1].GetY() - verts[0].GetY())));*/

		scanLines->xStart = Interpolate<float>(verts[0].GetX(), verts[2].GetX(), verts[0].GetY(), verts[2].GetY(), currentY);

		scanLines->xEnd = Interpolate<float>(verts[0].GetX(), verts[1].GetX(), verts[0].GetY(), verts[1].GetY(), currentY);

		if (scanLines->xStart > scanLines->xEnd)
		{
			/*float temp = scanLines->xEnd;
			scanLines->xEnd = scanLines->xStart;
			scanLines->xStart = temp;*/
			std::swap(scanLines->xStart, scanLines->xEnd);
		}

		currentY+=1;
		scanLines++;
	}

	// Loop 2: AC[0][2] and BC[1][2]

	yStart = (int)verts[1].GetY();
	yEnd = (int)verts[2].GetY();
	currentY = verts[1].GetY();

	for(int i = yStart; i < yEnd; i++)
	{
		/*scanLines->xStart = verts[0].GetX() + ((currentY - verts[0].GetY()) * ((verts[2].GetX() - verts[0].GetX()) / 
			(verts[2].GetY() - verts[0].GetY())));

		scanLines->xEnd = verts[1].GetX() + ((currentY - verts[1].GetY()) * ((verts[2].GetX() - verts[1].GetX()) / 
			(verts[2].GetY() - verts[1].GetY())));*/

		scanLines->xStart = Interpolate<float>(verts[0].GetX(), verts[2].GetX(), verts[0].GetY(), verts[2].GetY(), currentY);
		scanLines->xEnd = Interpolate<float>(verts[1].GetX(), verts[2].GetX(), verts[1].GetY(), verts[2].GetY(), currentY);

		if (scanLines->xStart > scanLines->xEnd)
		{
			/*float temp = scanLines->xEnd;
			scanLines->xEnd = scanLines->xStart;
			scanLines->xStart = temp;*/

			std::swap(scanLines->xStart, scanLines->xEnd);
		}

		currentY+=1;
		scanLines++;
	}

	yStart = (int) verts[0].GetY();
	yEnd = (int)verts[2].GetY();
	currentY = verts[2].GetY();

	for(int i = yStart; i < yEnd; i++)
	{
		--scanLines;
		currentY-=1;
		Rasterizer::DrawScanLine(scanLines->xStart, scanLines->xEnd, currentY, colour);
	}

	delete scanLines;
}

void Rasterizer::DrawScanLine(float xStart, float xEnd, float y, Gdiplus::Color colour)
{
	for(xStart; xStart < xEnd; xStart+=1)
	{
		_bitmap->SetPixel((int)xStart, (int)y, colour);
	}
}

void Rasterizer::DrawSolidGouraud(Model3D& model)
{
	int polyNum = model.GetNumOfPolys();
	for (int i = 0; i < polyNum; i++)
	{
		Vertex tempVerts[3];
		tempVerts[0] = model.GetVertex(i, 0);
		tempVerts[1] = model.GetVertex(i, 1);
		tempVerts[2] = model.GetVertex(i, 2);
		if (!model.IsPolygonCulled(i))
		{
			Rasterizer::FillPolygonGouraud(tempVerts);
		}	
	}
}

void Rasterizer::FillPolygonGouraud(Vertex verts[3])
{
	ScanLine* scanLines = new ScanLine[_height];

	while(verts[0].GetY() > verts[1].GetY() || verts[1].GetY() > verts[2].GetY())
	{
		Vertex tempVert;

		if(verts[0].GetY() > verts[1].GetY())
		{
			/*tempVert = verts[0];
			verts[0] = verts[1];
			verts[1] = tempVert;*/

			std::swap(verts[0], verts[1]);
		}

		if(verts[1].GetY() > verts[2].GetY())
		{
			/*tempVert = verts[1];
			verts[1] = verts[2];
			verts[2] = tempVert;*/

			std::swap(verts[1], verts[2]);
		}

		if(verts[0].GetY() > verts[1].GetY())
		{
			/*tempVert = verts[0];
			verts[0] = verts[1];
			verts[1] = tempVert;*/

			std::swap(verts[0], verts[1]);
		}
	}

	// Loop 1: AC[0][2] and AB[0][1]

	int yStart = (int)verts[0].GetY();
	int yEnd = (int)verts[1].GetY();
	float currentY = verts[0].GetY();

	for(int i = yStart; i < yEnd; i++)
	{
		/*scanLines->xStart = verts[0].GetX() + ((currentY - verts[0].GetY()) * ((verts[2].GetX() - verts[0].GetX()) / 
			(verts[2].GetY() - verts[0].GetY())));

		scanLines->xEnd = verts[0].GetX() + ((currentY - verts[0].GetY()) * ((verts[1].GetX() - verts[0].GetX()) / 
			(verts[1].GetY() - verts[0].GetY())));

		scanLines->rStart = verts[0].GetColour().GetRed() + ((currentY - verts[0].GetY()) * ((verts[2].GetColour().GetRed() - 
			verts[0].GetColour().GetRed()) / (verts[2].GetY() - verts[0].GetY())));

		scanLines->rEnd = verts[0].GetColour().GetRed() + ((currentY - verts[0].GetY()) * ((verts[1].GetColour().GetRed() - 
			verts[0].GetColour().GetRed()) / (verts[1].GetY() - verts[0].GetY())));

		scanLines->gStart = verts[0].GetColour().GetGreen() + ((currentY - verts[0].GetY()) * ((verts[2].GetColour().GetGreen() - 
			verts[0].GetColour().GetGreen()) / (verts[2].GetY() - verts[0].GetY())));

		scanLines->gEnd = verts[0].GetColour().GetGreen() + ((currentY - verts[0].GetY()) * ((verts[1].GetColour().GetGreen() - 
			verts[0].GetColour().GetGreen()) / (verts[1].GetY() - verts[0].GetY())));

		scanLines->bStart = verts[0].GetColour().GetBlue() + ((currentY - verts[0].GetY()) * ((verts[2].GetColour().GetBlue() - 
			verts[0].GetColour().GetBlue()) / (verts[2].GetY() - verts[0].GetY())));

		scanLines->bEnd = verts[0].GetColour().GetBlue() + ((currentY - verts[0].GetY()) * ((verts[1].GetColour().GetBlue() - 
			verts[0].GetColour().GetBlue()) / (verts[1].GetY() - verts[0].GetY())));*/

		scanLines->xStart = Interpolate<float>(verts[0].GetX(), verts[2].GetX(), verts[0].GetY(), verts[2].GetY(), currentY);

		scanLines->xEnd = Interpolate<float>(verts[0].GetX(), verts[1].GetX(), verts[0].GetY(), verts[1].GetY(), currentY);

		scanLines->rStart = Interpolate<float>(verts[0].GetColour().GetRed(), verts[2].GetColour().GetRed(), 
												verts[0].GetY(), verts[2].GetY(), currentY);

		scanLines->rEnd = Interpolate<float>(verts[0].GetColour().GetRed(), verts[1].GetColour().GetRed(),
												verts[0].GetY(), verts[1].GetY(), currentY);

		scanLines->gStart = Interpolate<float>(verts[0].GetColour().GetGreen(), verts[2].GetColour().GetGreen(),
												verts[0].GetY(), verts[2].GetY(), currentY);

		scanLines->gEnd = Interpolate<float>(verts[0].GetColour().GetGreen(), verts[1].GetColour().GetGreen(),
												verts[0].GetY(), verts[1].GetY(), currentY);

		scanLines->bStart = Interpolate<float>(verts[0].GetColour().GetBlue(), verts[2].GetColour().GetBlue(), 
												verts[0].GetY(), verts[2].GetY(), currentY);

		scanLines->bEnd = Interpolate<float>(verts[0].GetColour().GetBlue(), verts[1].GetColour().GetBlue(),
												verts[0].GetY(), verts[1].GetY(), currentY);

		if (scanLines->xStart > scanLines->xEnd)
		{
			/*float temp = scanLines->xEnd;
			scanLines->xEnd = scanLines->xStart;
			scanLines->xStart = temp;

			temp = scanLines->rEnd;
			scanLines->rEnd = scanLines->rStart;
			scanLines->rStart = temp;

			temp = scanLines->gEnd;
			scanLines->gEnd = scanLines->gStart;
			scanLines->gStart = temp;

			temp = scanLines->bEnd;
			scanLines->bEnd = scanLines->bStart;
			scanLines->bStart = temp;*/

			std::swap(scanLines->xStart, scanLines->xEnd);
			std::swap(scanLines->rStart, scanLines->rEnd);
			std::swap(scanLines->gStart, scanLines->gEnd);
			std::swap(scanLines->bStart, scanLines->bEnd);
		}

		currentY+=1;
		scanLines++;
	}
	
	// Loop 2: AC[0][2] and BC[1][2]

	yStart = (int)verts[1].GetY();
	yEnd = (int)verts[2].GetY();
	currentY = verts[1].GetY();

	for(int i = yStart; i < yEnd; i++)
	{

		/*scanLines->xStart = verts[0].GetX() + ((currentY - verts[0].GetY()) * ((verts[2].GetX() - verts[0].GetX()) / 
			(verts[2].GetY() - verts[0].GetY())));

		scanLines->xEnd = verts[1].GetX() + ((currentY - verts[1].GetY()) * ((verts[2].GetX() - verts[1].GetX()) / 
			(verts[2].GetY() - verts[1].GetY())));

		scanLines->rStart = verts[0].GetColour().GetRed() + ((currentY - verts[0].GetY()) * ((verts[2].GetColour().GetRed() - 
			verts[0].GetColour().GetRed()) / (verts[2].GetY() - verts[0].GetY())));

		scanLines->rEnd = verts[1].GetColour().GetRed() + ((currentY - verts[1].GetY()) * ((verts[2].GetColour().GetRed() - 
			verts[1].GetColour().GetRed()) / (verts[2].GetY() - verts[1].GetY())));

		scanLines->gStart = verts[0].GetColour().GetGreen() + ((currentY - verts[0].GetY()) * ((verts[2].GetColour().GetGreen() - 
			verts[0].GetColour().GetGreen()) / (verts[2].GetY() - verts[0].GetY())));

		scanLines->gEnd = verts[1].GetColour().GetGreen() + ((currentY - verts[1].GetY()) * ((verts[2].GetColour().GetGreen() - 
			verts[1].GetColour().GetGreen()) / (verts[2].GetY() - verts[1].GetY())));

		scanLines->bStart = verts[0].GetColour().GetBlue() + ((currentY - verts[0].GetY()) * ((verts[2].GetColour().GetBlue() - 
			verts[0].GetColour().GetBlue()) / (verts[2].GetY() - verts[0].GetY())));

		scanLines->bEnd = verts[1].GetColour().GetBlue() + ((currentY - verts[1].GetY()) * ((verts[2].GetColour().GetBlue() - 
			verts[1].GetColour().GetBlue()) / (verts[2].GetY() - verts[1].GetY())));*/

		scanLines->xStart = Interpolate<float>(verts[0].GetX(), verts[2].GetX(), verts[0].GetY(), verts[2].GetY(), currentY);

		scanLines->xEnd = Interpolate<float>(verts[1].GetX(), verts[2].GetX(), verts[1].GetY(), verts[2].GetY(), currentY);

		scanLines->rStart = Interpolate<float>(verts[0].GetColour().GetRed(), verts[2].GetColour().GetRed(), 
												verts[0].GetY(), verts[2].GetY(), currentY);

		scanLines->rEnd = Interpolate<float>(verts[1].GetColour().GetRed(), verts[2].GetColour().GetRed(),
												verts[1].GetY(), verts[2].GetY(), currentY);

		scanLines->gStart = Interpolate<float>(verts[0].GetColour().GetGreen(), verts[2].GetColour().GetGreen(),
												verts[0].GetY(), verts[2].GetY(), currentY);

		scanLines->gEnd = Interpolate<float>(verts[1].GetColour().GetGreen(), verts[2].GetColour().GetGreen(),
												verts[1].GetY(), verts[2].GetY(), currentY);

		scanLines->bStart = Interpolate<float>(verts[0].GetColour().GetBlue(), verts[2].GetColour().GetBlue(), 
												verts[0].GetY(), verts[2].GetY(), currentY);

		scanLines->bEnd = Interpolate<float>(verts[1].GetColour().GetBlue(), verts[2].GetColour().GetBlue(),
												verts[1].GetY(), verts[2].GetY(), currentY);

		if (scanLines->xStart > scanLines->xEnd)
		{
			/*float temp = scanLines->xEnd;
			scanLines->xEnd = scanLines->xStart;
			scanLines->xStart = temp;

			temp = scanLines->rEnd;
			scanLines->rEnd = scanLines->rStart;
			scanLines->rStart = temp;

			temp = scanLines->gEnd;
			scanLines->gEnd = scanLines->gStart;
			scanLines->gStart = temp;

			temp = scanLines->bEnd;
			scanLines->bEnd = scanLines->bStart;
			scanLines->bStart = temp;*/

			std::swap(scanLines->xStart, scanLines->xEnd);
			std::swap(scanLines->rStart, scanLines->rEnd);
			std::swap(scanLines->gStart, scanLines->gEnd);
			std::swap(scanLines->bStart, scanLines->bEnd);
		}

		currentY+=1;
		scanLines++;
	}

	yStart = (int) verts[0].GetY();
	yEnd = (int)verts[2].GetY();
	currentY = verts[2].GetY();

	for(int i = yStart; i < yEnd; i++)
	{
		--scanLines;
		currentY-=1;
		Rasterizer::DrawGouraudScanLine(scanLines->xStart, scanLines->xEnd, currentY, scanLines->rStart, scanLines->rEnd, scanLines->gStart,
			scanLines->gEnd, scanLines->bStart, scanLines->bEnd);
		//Rasterizer::DrawGouraudScanLine(scanLines[i].xStart, scanLines[i].xEnd, currentY, scanLines[i].rStart, scanLines[i].rEnd,
			//scanLines[i].gStart, scanLines[i].gEnd, scanLines[i].bStart, scanLines[i].bEnd);
		//--scanLines;
		
	}
	
	delete scanLines;
}

void Rasterizer::DrawGouraudScanLine(float xStart, float xEnd, float y, float rStart, float rEnd, float gStart, 
	float gEnd, float bStart, float bEnd)
{
	for(int i = (int)xStart; i <= xEnd; ++i)
	{
		float currentR = rStart + ((i - xStart) * ((rEnd - rStart) / (xEnd - xStart)));
		float currentG = gStart + ((i - xStart) * ((gEnd - gStart) / (xEnd - xStart)));
		float currentB = bStart + ((i - xStart) * ((bEnd - bStart) / (xEnd - xStart)));

		if(currentR > 255)
		{
			currentR = 255;
		}
		else if (currentR < 0)
		{
			currentR = 0;
		}
		if(currentG > 255)
		{
			currentG = 255;
		}
		else if (currentG < 0)
		{
			currentG = 0;
		}
		if(currentB > 255)
		{
			currentB = 255;
		}
		else if(currentB < 0)
		{
			currentB = 0;
		}

		_bitmap->SetPixel(i, (int)y, Gdiplus::Color((BYTE)currentR, (BYTE)currentG, (BYTE)currentB));
	}			
}

void Rasterizer::DrawSolidTextured(Model3D& model)
{
	int polyNum = model.GetNumOfPolys();
	for (int i = 0; i < polyNum; i++)
	{
		Vertex tempVerts[3];
		Texel tempTexels[3];
		tempVerts[0] = model.GetVertex(i, 0);
		tempVerts[1] = model.GetVertex(i, 1);
		tempVerts[2] = model.GetVertex(i, 2);
		tempTexels[0] = model.GetPolygon(i).GetTexel(0);
		tempTexels[1] = model.GetPolygon(i).GetTexel(1);
		tempTexels[2] = model.GetPolygon(i).GetTexel(2);

		if (model.IsPolygonCulled(i) == false)
		{
			Rasterizer::FillPolygonTextured(tempVerts, tempTexels, model);
		}	
	}
}

void Rasterizer::FillPolygonTextured(Vertex verts[3], Texel texels[3], Model3D& model)
{
	ScanLine* scanLines = new ScanLine[_height];

	while(verts[0].GetY() > verts[1].GetY() || verts[1].GetY() > verts[2].GetY())
	{
		Vertex tempVert;
		Texel tempTexel;

		if(verts[0].GetY() > verts[1].GetY())
		{
			tempVert = verts[0];
			verts[0] = verts[1];
			verts[1] = tempVert;
			tempTexel = texels[0];
			texels[0] = texels[1];
			texels[1] = tempTexel;
		}

		if(verts[1].GetY() > verts[2].GetY())
		{
			tempVert = verts[1];
			verts[1] = verts[2];
			verts[2] = tempVert;
			tempTexel = texels[1];
			texels[1] = texels[2];
			texels[2] = tempTexel;
		}

		if(verts[0].GetY() > verts[1].GetY())
		{
			tempVert = verts[0];
			verts[0] = verts[1];
			verts[1] = tempVert;
			tempTexel = texels[0];
			texels[0] = texels[1];
			texels[1] = tempTexel;
		}
	}

	// Loop 1: AC[0][2] and AB[0][1]

	int yStart = (int)verts[0].GetY();
	int yEnd = (int)verts[1].GetY();
	float currentY = verts[0].GetY();

	for(int i = yStart; i < yEnd; i++)
	{
		scanLines->xStart = verts[0].GetX() + ((currentY - verts[0].GetY()) * ((verts[2].GetX() - verts[0].GetX()) / 
			(verts[2].GetY() - verts[0].GetY())));

		scanLines->xEnd = verts[0].GetX() + ((currentY - verts[0].GetY()) * ((verts[1].GetX() - verts[0].GetX()) / 
			(verts[1].GetY() - verts[0].GetY())));

		scanLines->uStart = texels[0].GetU() + ((currentY - verts[0].GetY()) * ((texels[2].GetU() - texels[0].GetU()) /
			(verts[2].GetY() - verts[0].GetY())));

		scanLines->uEnd = texels[0].GetU() + ((currentY - verts[0].GetY()) * ((texels[1].GetU() - texels[0].GetU()) /
			(verts[1].GetY() - verts[0].GetY())));

		scanLines->rStart = verts[0].GetColour().GetRed() + ((currentY - verts[0].GetY()) * ((verts[2].GetColour().GetRed() - 
			verts[0].GetColour().GetRed()) / (verts[2].GetY() - verts[0].GetY())));

		scanLines->rEnd = verts[0].GetColour().GetRed() + ((currentY - verts[0].GetY()) * ((verts[1].GetColour().GetRed() - 
			verts[0].GetColour().GetRed()) / (verts[1].GetY() - verts[0].GetY())));

		scanLines->gStart = verts[0].GetColour().GetGreen() + ((currentY - verts[0].GetY()) * ((verts[2].GetColour().GetGreen() - 
			verts[0].GetColour().GetGreen()) / (verts[2].GetY() - verts[0].GetY())));

		scanLines->gEnd = verts[0].GetColour().GetGreen() + ((currentY - verts[0].GetY()) * ((verts[1].GetColour().GetGreen() - 
			verts[0].GetColour().GetGreen()) / (verts[1].GetY() - verts[0].GetY())));

		scanLines->bStart = verts[0].GetColour().GetBlue() + ((currentY - verts[0].GetY()) * ((verts[2].GetColour().GetBlue() - 
			verts[0].GetColour().GetBlue()) / (verts[2].GetY() - verts[0].GetY())));

		scanLines->bEnd = verts[0].GetColour().GetBlue() + ((currentY - verts[0].GetY()) * ((verts[1].GetColour().GetBlue() - 
			verts[0].GetColour().GetBlue()) / (verts[1].GetY() - verts[0].GetY())));

		if (scanLines->xStart > scanLines->xEnd)
		{
			float temp = scanLines->xEnd;
			scanLines->xEnd = scanLines->xStart;
			scanLines->xStart = temp;

			temp = scanLines->uEnd;
			scanLines->uEnd = scanLines->uStart;
			scanLines->uStart = temp;

			temp = scanLines->rEnd;
			scanLines->rEnd = scanLines->rStart;
			scanLines->rStart = temp;

			temp = scanLines->gEnd;
			scanLines->gEnd = scanLines->gStart;
			scanLines->gStart = temp;

			temp = scanLines->bEnd;
			scanLines->bEnd = scanLines->bStart;
			scanLines->bStart = temp;
		}

		currentY+=1;
		scanLines++;
	}
	
	// Loop 2: AC[0][2] and BC[1][2]

	yStart = (int)verts[1].GetY();
	yEnd = (int)verts[2].GetY();
	currentY = verts[1].GetY();

	for(int i = yStart; i < yEnd; i++)
	{
		scanLines->xStart = verts[0].GetX() + ((currentY - verts[0].GetY()) * ((verts[2].GetX() - verts[0].GetX()) / 
			(verts[2].GetY() - verts[0].GetY())));

		scanLines->xEnd = verts[1].GetX() + ((currentY - verts[1].GetY()) * ((verts[2].GetX() - verts[1].GetX()) / 
			(verts[2].GetY() - verts[1].GetY())));

		scanLines->uStart = texels[0].GetU() + ((currentY - verts[0].GetY()) * ((texels[2].GetU() - texels[0].GetU()) /
			(verts[2].GetY() - verts[0].GetY())));

		scanLines->uEnd = texels[1].GetU() + ((currentY - verts[1].GetY()) * ((texels[2].GetU() - texels[1].GetU()) /
			(verts[2].GetY() - verts[1].GetY())));

		scanLines->rStart = verts[0].GetColour().GetRed() + ((currentY - verts[0].GetY()) * ((verts[2].GetColour().GetRed() - 
			verts[0].GetColour().GetRed()) / (verts[2].GetY() - verts[0].GetY())));

		scanLines->rEnd = verts[1].GetColour().GetRed() + ((currentY - verts[1].GetY()) * ((verts[2].GetColour().GetRed() - 
			verts[1].GetColour().GetRed()) / (verts[2].GetY() - verts[1].GetY())));

		scanLines->gStart = verts[0].GetColour().GetGreen() + ((currentY - verts[0].GetY()) * ((verts[2].GetColour().GetGreen() - 
			verts[0].GetColour().GetGreen()) / (verts[2].GetY() - verts[0].GetY())));

		scanLines->gEnd = verts[1].GetColour().GetGreen() + ((currentY - verts[1].GetY()) * ((verts[2].GetColour().GetGreen() - 
			verts[1].GetColour().GetGreen()) / (verts[2].GetY() - verts[1].GetY())));

		scanLines->bStart = verts[0].GetColour().GetBlue() + ((currentY - verts[0].GetY()) * ((verts[2].GetColour().GetBlue() - 
			verts[0].GetColour().GetBlue()) / (verts[2].GetY() - verts[0].GetY())));

		scanLines->bEnd = verts[1].GetColour().GetBlue() + ((currentY - verts[1].GetY()) * ((verts[2].GetColour().GetBlue() - 
			verts[1].GetColour().GetBlue()) / (verts[2].GetY() - verts[1].GetY())));

		if (scanLines->xStart > scanLines->xEnd)
		{
			float temp = scanLines->xEnd;
			scanLines->xEnd = scanLines->xStart;
			scanLines->xStart = temp;

			temp = scanLines->uEnd;
			scanLines->uEnd = scanLines->uStart;
			scanLines->uStart = temp;

			temp = scanLines->rEnd;
			scanLines->rEnd = scanLines->rStart;
			scanLines->rStart = temp;

			temp = scanLines->gEnd;
			scanLines->gEnd = scanLines->gStart;
			scanLines->gStart = temp;

			temp = scanLines->bEnd;
			scanLines->bEnd = scanLines->bStart;
			scanLines->bStart = temp;
		}

		currentY+=1;
		scanLines++;
	}

	yStart = (int) verts[0].GetY();
	yEnd = (int)verts[2].GetY();
	currentY = verts[2].GetY();
	float currentV = texels[2].GetV();
	float vDecrementer = (float)(1 / model.GetTextureWidth());

	for(int i = yStart; i < yEnd; i++)
	{
		scanLines--;
		currentY-=1;
		currentV -= vDecrementer;
		Rasterizer::DrawTexturedLine(model, scanLines->xStart, scanLines->xEnd, currentY, scanLines->rStart, scanLines->rEnd, scanLines->gStart,
			scanLines->gEnd, scanLines->bStart, scanLines->bEnd, scanLines->uStart, scanLines->uEnd, currentV);
	}
	
	delete scanLines;
}

void Rasterizer::DrawTexturedLine(Model3D& model, float xStart, float xEnd, float y, float rStart, float rEnd, float gStart, 
	float gEnd, float bStart, float bEnd, float uStart, float uEnd, float v)
{
	for(int i = (int)xStart; i < xEnd; i++)
	{
		float currentR = rStart + ((i - xStart) * ((rEnd - rStart) / (xEnd - xStart)));
		float currentG = gStart + ((i - xStart) * ((gEnd - gStart) / (xEnd - xStart)));
		float currentB = bStart + ((i - xStart) * ((bEnd - bStart) / (xEnd - xStart)));

		if(currentR > 255)
		{
			currentR = 255;
		}
		else if (currentR < 0)
		{
			currentR = 0;
		}
		if(currentG > 255)
		{
			currentG = 255;
		}
		else if (currentG < 0)
		{
			currentG = 0;
		}
		if(currentB > 255)
		{
			currentB = 255;
		}
		else if(currentB < 0)
		{
			currentB = 0;
		}

		Gdiplus::Color* textureColor = model.GetPalette((BYTE)(v * model.GetTextureWidth() + uStart));

		//currentR = ((textureColor->GetRed() * currentR) / 255);
		//currentG = ((textureColor->GetGreen() * currentG) / 255);
		//currentB = ((textureColor->GetBlue() * currentB) / 255);

		//_bitmap->SetPixel(i, (int)y, Gdiplus::Color((BYTE)currentR, (BYTE)currentG, (BYTE)currentB));
		_bitmap->SetPixel(i, (int)y, *textureColor);
		uStart++;
	}
}

void Rasterizer::DrawString(WCHAR* text)
{
	Font font(L"Arial", 12);
	SolidBrush brush(Gdiplus::Color(255, 255,255));
	PointF origin(1.0f, 1.0f);

	_graphics->DrawString(text, -1, &font, origin, &brush);
}

/*template<class T> T Rasterizer::Interpolate(T x0,T x1,T y0,T y1,T Y)
{
	return (x0 + ((Y - y0) * ((x1 - x0) / (y1 - y0))));
}*/
