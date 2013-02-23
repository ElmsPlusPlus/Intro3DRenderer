#include "StdAfx.h"
#include "Model3D.h"
#include <algorithm>
#include <functional>


// Constructors

Model3D::Model3D(void)
{
	_texture = NULL;
	_palette = NULL;
	_textureWidth = NULL;
}

Model3D::~Model3D(void)
{
	delete _texture;
	delete _palette;
}

// Adds a vertex to the model

void Model3D::AddVertex(Vertex& verty)
{
	_vertices.push_back(verty);
}

// Adds a polygon to the model

void Model3D::AddPolygon(Polygon3D& poly)
{
	_polygons.push_back(poly);
}

// Accessor for the polygons list

Polygon3D Model3D::GetPolygon(int index) const
{
	return _polygons[index];
}

// Accessor and Mutator for the vertices in the transformed vertices list

Vertex Model3D::GetVertex(int index, int vertex)
{
	return _transformedVertices[_polygons[index].GetI(vertex)];
}

void Model3D::SetVertex(int index, int vertex, Vertex tempVertex)
{
	_transformedVertices[_polygons[index].GetI(vertex)] = tempVertex;
}

// Gets the number of polygons the model is made of

int Model3D::GetNumOfPolys(void) const
{
	return _polygons.size();
}

// Accessor and Mutators for the reflection coefficients

float Model3D::GetKaRed(void) const
{
	return _kaRed;
}

void Model3D::SetKaRed(float kaRed)
{
	_kaRed = kaRed;
}

float Model3D::GetKaGreen(void) const
{
	return _kaGreen;
}

void Model3D::SetKaGreen(float kaGreen)
{
	_kaGreen = kaGreen;
}

float Model3D::GetKaBlue(void) const
{
	return _kaBlue;
}

void Model3D::SetKaBlue(float kaBlue)
{
	_kaBlue = kaBlue;
}

float Model3D::GetKdRed(void) const
{
	return _kdRed;
}

void Model3D::SetKdRed(float kdRed)
{
	_kdRed = kdRed;
}

float Model3D::GetKdGreen(void) const
{
	return _kdGreen;
}

void Model3D::SetKdGreen(float kdGreen)
{
	_kdGreen = kdGreen;
}

float Model3D::GetKdBlue(void) const
{
	return _kdBlue;
}

void Model3D::SetKdBlue(float kdBlue)
{
	_kdBlue = kdBlue;
}

float Model3D::GetKsRed(void) const
{
	return _ksRed;
}

void Model3D::SetKsRed(float ksRed)
{
	_ksRed = ksRed;
}

float Model3D::GetKsGreen(void) const
{
	return _ksGreen;
}

void Model3D::SetKsGreen(float ksGreen)
{
	_ksGreen = ksGreen;
}

float Model3D::GetKsBlue(void) const
{
	return _ksBlue;
}

void Model3D::SetKsBlue(float ksBlue)
{
	_ksBlue = ksBlue;
}

// Accessors and Mutators for the texture variables

BYTE* Model3D::GetTexture(void) const
{
	return _texture;
}

void Model3D::SetTexture(BYTE* texture)
{
	_texture = texture;
}

Gdiplus::Color* Model3D::GetPalette(BYTE index) const
{
	return _palette + index;
}

void Model3D::SetPalette(Gdiplus::Color* palette)
{
	_palette = palette;
}

int Model3D::GetTextureWidth(void) const
{
	return _textureWidth;
}

void Model3D::SetTextureWidth(int textureWidth)
{
	_textureWidth = textureWidth;
}

// Public Methods

void Model3D::ApplyTransformToLocalVertices(const Matrix3D& transform)
{
	// Clears the transformed vertices list (as this will be the first transform)

	_transformedVertices.clear();

	// Loops through each vertex, transforms it by the matrix passed in, then stores it in the 
	// transformed vertices list

	for (unsigned int i = 0; i < _vertices.size(); i++)
	{
		Vertex tempVert;

		tempVert = transform * _vertices[i];

		_transformedVertices.push_back(tempVert);
	}
}

void Model3D::ApplyTransformToTransformedVertices(const Matrix3D& transform)
{ 
	// Loops through all the transformed vertices, transforms it by the matrix passed in
	// then stores it back in the transformed vertices list

	for (unsigned int i = 0; i < _transformedVertices.size(); i++)
	{
		_transformedVertices[i] = transform * _transformedVertices[i];
	}
}

void Model3D::Deheomogenize()
{
	// Deheomogenizes all the transformed vertices by looping through them and calling the vertex method for deheomogenizing

	for (unsigned int i = 0; i < _transformedVertices.size(); i++)
	{
		_transformedVertices[i].Dehomogenize();
	}
}

bool Model3D::IsPolygonCulled(int polyNum)
{
	// Checks if a polygon is culled and returns a boolean of true if it is, false if it is not

	bool culled;

	culled = _polygons[polyNum].GetCulled();

	return culled;
}

void Model3D::CalculateBackfaces(const Vertex& camPosition)
{
	// Loops through each polygon, calculates if it needs to be called or not

	for(unsigned int i = 0; i < _polygons.size(); i++)
	{
		// Create an array of local vertex variables (3 as each polygon is a triangle)

		Vertex vertices[3];
		vertices[0] = Model3D::GetVertex(i, 0);
		vertices[1] = Model3D::GetVertex(i, 1);
		vertices[2] = Model3D::GetVertex(i, 2);

		// Calculates the vectors of between vertices 0 and 1, and 0 and 2

		Vector3D vect1 = Vertex::ReturnVector(vertices[0], vertices[1]);
		Vector3D vect2 = Vertex::ReturnVector(vertices[0], vertices[2]);

		// Calculates the cross product of the two vectors (the normal vector of the polygon)

		_polygons[i].SetNormal(Vector3D::CrossProduct(vect2, vect1));

		// Calculates the vector between vertex 0 of the polygon and the camera

		Vector3D eyeVector = Vertex::ReturnVector(vertices[0], camPosition);

		// Calculates the dot product of the normal vector and the eye vector

		float dotProduct = Vector3D::DotProduct(_polygons[i].GetNormal(), eyeVector);

		// If the dot product is less than 0 it is not in the view of the camera it can be culled so the boolean is set to true
		// if not it is set to false so it will be drawn

		if ( dotProduct < 0)
		{
			_polygons[i].SetCulled(true);
		}
		else
		{
			_polygons[i].SetCulled(false);
		}
	}
}

void Model3D::Sort(void)
{
	// Depth sorts a the polygons

	for (unsigned int i = 0; i < _polygons.size(); i++)
	{
		Vertex tempVerts[3];

		tempVerts[0] = Model3D::GetVertex(i,0);
		tempVerts[1] = Model3D::GetVertex(i,1);
		tempVerts[2] = Model3D::GetVertex(i,2);

		// Calculate the average of the Z values of the polygon (their depth)

		float averageZ = ((tempVerts[0].GetZ() + tempVerts[1].GetZ() + tempVerts[2].GetZ()) / 3);
		_polygons[i].SetAverageZ(averageZ);
	}

	// Runs the standard sort method to sort the polygons. A < operator is defined in the polygon class so that they are sorted
	// by their average Z values

	std::sort(_polygons.begin(), _polygons.end());
}

void Model3D::CalculateLightingDirectionalFlat(std::vector<DirectionalLight>& dirLights)
{
	// Calculate the direction lighting for flat shading

	float totalR, totalG, totalB;
	float tempR, tempG, tempB;

	// Loop through all polygons

	for(unsigned int i = 0; i < _polygons.size(); i++)
	{
		// Set the current total RGB values to the values from the polygon

		totalR = _polygons[i].GetColor().GetRed();
		totalG = _polygons[i].GetColor().GetGreen();
		totalB = _polygons[i].GetColor().GetBlue();

		// Loop through all the directional lights 

		for(unsigned int j = 0; j < dirLights.size(); j++)
		{
			// Set the temp RGB variables to the the RGB values of the directional light

			tempR = dirLights[j].GetRedIntensity();
			tempG = dirLights[j].GetGreenIntensity();
			tempB = dirLights[j].GetBlueIntensity();

			// Multiply these values by the diffuse relfection coefficents

			tempR = tempR * _kdRed;
			tempG = tempG * _kdGreen;
			tempB = tempB * _kdBlue;

			// Normalise the vector of the light direction

			Vector3D tempVect1 = Vector3D::Normalise(dirLights[j].GetLightDirection());

			// Get the normal vector of the polygon

			Vector3D tempVect2 = _polygons[i].GetNormal();

			// Calculate the dot product of the two vectors

			float amountOfLight = Vector3D::DotProduct(tempVect1, tempVect2);

			// Clamp the value between 0 & 1 (no illumination and full illumination)

			if(amountOfLight > 1)
			{
				amountOfLight = 1;
			}
			else if(amountOfLight < 0)
			{
				amountOfLight = 0;
			}

			// Multiply the current RGB values by the amount of light

			tempR = tempR * amountOfLight;
			tempG = tempG * amountOfLight;
			tempB = tempB * amountOfLight;


			// Add the current RGB values to the running total RGB values, then clamp between 0 & 255

			totalR += tempR;
			if(totalR > 255)
			{
				totalR = 255;
			}
			else if (totalR < 0)
			{
				totalR = 0;
			}
			totalG += tempG;
			if(totalG > 255)
			{
				totalG = 255;
			}
			else if (totalG < 0)
			{
				totalG = 0;
			}
			totalB += tempB;
			if(totalB > 255)
			{
				totalB = 255;
			}
			else if(totalB < 0)
			{
				totalB = 0;
			}


			// Set the colour variable of the polygon to the colour calculated

			Gdiplus::Color tempColour((BYTE)totalR, (BYTE)totalG, (BYTE)totalB);
			_polygons[i].SetColor(tempColour);
		}
	}
}

void Model3D::CalculateLightingDirectional(std::vector<DirectionalLight>& dirLights)
{
	// Calculate the direction lighting per vertex

	float totalR, totalG, totalB;
	float tempR, tempG, tempB;

	// Loop through all vertices

	for(unsigned int i = 0; i < _transformedVertices.size(); i++)
	{
		// Get the RGB values of the vertex and store it in temp variables

		totalR = _transformedVertices[i].GetColour().GetRed();
		totalG = _transformedVertices[i].GetColour().GetGreen();
		totalB = _transformedVertices[i].GetColour().GetBlue();

		// Loop through each directional light

		for(unsigned int j = 0; j < dirLights.size(); j++)
		{
			// Store the RGB value of the directional light in temp variables

			tempR = dirLights[j].GetRedIntensity();
			tempG = dirLights[j].GetGreenIntensity();
			tempB = dirLights[j].GetBlueIntensity();

			// Multiply the running RGB values by the diffuse reflection coefficients

			tempR = tempR * _kdRed;
			tempG = tempG * _kdGreen;
			tempB = tempB * _kdBlue;

			// Normalise the direction vector of the light

			Vector3D tempVect1 = Vector3D::Normalise(dirLights[j].GetLightDirection());

			// Get the normal vector of the vertices

			Vector3D tempVect2 = _transformedVertices[i].GetNormal();

			// Calculate the dot product of the two vertices

			float amountOfLight = Vector3D::DotProduct(tempVect1, tempVect2);

			// Clamp the value between 0 & 1 (no illumination and full illumination)

			if(amountOfLight > 1)
			{
				amountOfLight = 1;
			}
			else if(amountOfLight < 0)
			{
				amountOfLight = 0;
			}

			// Multiply the running RGB values by the amount of light

			tempR = tempR * amountOfLight;
			tempG = tempG * amountOfLight;
			tempB = tempB * amountOfLight;

			// Add the running RGB values to the RGB values of the vertex, them clamp between 0 & 255

			totalR += tempR;
			if(totalR > 255)
			{
				totalR = 255;
			}
			else if (totalR < 0)
			{
				totalR = 0;
			}
			totalG += tempG;
			if(totalG > 255)
			{
				totalG = 255;
			}
			else if (totalG < 0)
			{
				totalG = 0;
			}
			totalB += tempB;
			if(totalB > 255)
			{
				totalB = 255;
			}
			else if(totalB < 0)
			{
				totalB = 0;
			}

			// Store the colour calculates back into the vertex

			Gdiplus::Color tempColour((BYTE)totalR, (BYTE)totalG, (BYTE)totalB);
			_transformedVertices[i].SetColour(tempColour);
		}
	}
}

void Model3D::CalculateLightingAmbientFlat(const AmbientLight& ambientLight)
{
	// Calculate the ambient lighting per polygon

	// Loop through all polygons

	for(unsigned int i = 0; i < _polygons.size(); i++)
	{
		// Set the RGB values to the values of the ambient light, as this is the base colour

		float totalR = ambientLight.GetRedIntensity();
		float totalG = ambientLight.GetGreenIntensity();
		float totalB = ambientLight.GetBlueIntensity();

		// Multiply by the ambient reflection coefficient

		totalR = totalR * _kaRed;
		totalG = totalG * _kaGreen;
		totalB = totalB * _kaBlue;

		// Clamp between 0 and 255

		if(totalR > 255)
		{
			totalR = 255;
		}
		else if (totalR < 0)
		{
			totalR = 0;
		}
		if(totalG > 255)
		{
			totalG = 255;
		}
		else if (totalG < 0)
		{
			totalG = 0;
		}
		if(totalB > 255)
		{
			totalB = 255;
		}
		else if(totalB < 0)
		{
			totalB = 0;
		}

		// Store the colour in the polygon

		Gdiplus::Color totalColour((BYTE)totalR, (BYTE)totalG, (BYTE)totalB);
		_polygons[i].SetColor(totalColour);
	}
}

void Model3D::CalculateLightingAmbient(const AmbientLight& ambientLight)
{
	// Calculate the ambient light per vertex

	// Loop through all vertices

	for(unsigned int i = 0; i < _transformedVertices.size(); i++)
	{
		// Set the RGB values to the values of the ambient light, as this is the base colour

		float totalR = ambientLight.GetRedIntensity();
		float totalG = ambientLight.GetGreenIntensity();
		float totalB = ambientLight.GetBlueIntensity();

		// Multiply by the ambient reflection coefficient

		totalR = totalR * _kaRed;
		totalG = totalG * _kaGreen;
		totalB = totalB * _kaBlue;

		// Clamp the values between 0 & 255

		if(totalR > 255)
		{
			totalR = 255;
		}
		else if (totalR < 0)
		{
			totalR = 0;
		}
		if(totalG > 255)
		{
			totalG = 255;
		}
		else if (totalG < 0)
		{
			totalG = 0;
		}
		if(totalB > 255)
		{
			totalB = 255;
		}
		else if(totalB < 0)
		{
			totalB = 0;
		}

		// Store the colour in the vertex

		Gdiplus::Color totalColour((BYTE)totalR, (BYTE)totalG, (BYTE)totalB);
		_transformedVertices[i].SetColour(totalColour);
	}
}

void Model3D::CalculateLightingPointFlat(std::vector<PointLight>& pointLights)
{
	float totalR;
	float totalG;
	float totalB;
	float tempR;
	float tempG;
	float tempB;
	float a;
	float b;
	float c;
	float d;

	for(unsigned int i = 0; i < _polygons.size(); i++)
	{
		totalR = _polygons[i].GetColor().GetRed();
		totalG = _polygons[i].GetColor().GetGreen();
		totalB = _polygons[i].GetColor().GetBlue();

		for(unsigned int j = 0; j < pointLights.size(); j++)
		{
			tempR = pointLights[j].GetRedIntensity();
			tempG = pointLights[j].GetGreenIntensity();
			tempB = pointLights[j].GetBlueIntensity();
			a = pointLights[j].GetAttenuationA();
			b = pointLights[j].GetAttenuationB();
			c = pointLights[j].GetAttenuationC();

			tempR = tempR * _kdRed;
			tempG = tempG * _kdGreen;
			tempB = tempB * _kdBlue;

			Vector3D tempVect1 = Vertex::ReturnVector(Model3D::GetVertex(i, 0), pointLights[j].GetLightLocation());
			d = tempVect1.GetLength();
			tempVect1 = Vector3D::Normalise(tempVect1);
			Vector3D tempVect2 = _polygons[i].GetNormal();
			float amountOfLight = Vector3D::DotProduct(tempVect1, tempVect2);
			float totalAttenuation = 1 / (a + (b * d) + (c * (d * d)));
			amountOfLight *= totalAttenuation;
			if(amountOfLight > 1)
			{
				amountOfLight = 1;
			}
			else if(amountOfLight < 0)
			{
				amountOfLight = 0;
			}

			tempR = tempR * amountOfLight;
			tempG = tempG * amountOfLight;
			tempB = tempB * amountOfLight;

			

			totalR += tempR;
			if(totalR > 255)
			{
				totalR = 255;
			}
			else if (totalR < 0)
			{
				totalR = 0;
			}
			totalG += tempG;
			if(totalG > 255)
			{
				totalG = 255;
			}
			else if (totalG < 0)
			{
				totalG = 0;
			}
			totalB += tempB;
			if(totalB > 255)
			{
				totalB = 255;
			}
			else if(totalB < 0)
			{
				totalB = 0;
			}

			Gdiplus::Color tempColour((BYTE)totalR, (BYTE)totalG, (BYTE)totalB);
			_polygons[i].SetColor(tempColour);
		}
	}
}

void Model3D::CalculateLightingPoint(std::vector<PointLight>& pointLights)
{
	for(unsigned int i = 0; i < _transformedVertices.size(); i++)
	{
		float totalR = _transformedVertices[i].GetColour().GetRed();
		float totalG = _transformedVertices[i].GetColour().GetGreen();
		float totalB = _transformedVertices[i].GetColour().GetBlue();
		//totalR = 0;
		//totalG = 0;
		//totalB = 0;

		for(unsigned int j = 0; j < pointLights.size(); j++)
		{
			float tempR = pointLights[j].GetRedIntensity();
			float tempG = pointLights[j].GetGreenIntensity();
			float tempB = pointLights[j].GetBlueIntensity();
			float a = pointLights[j].GetAttenuationA();
			float b = pointLights[j].GetAttenuationB();
			float c = pointLights[j].GetAttenuationC();

			tempR = tempR * _kdRed;
			tempG = tempG * _kdGreen;
			tempB = tempB * _kdBlue;

			Vector3D tempVect1 = Vertex::ReturnVector(_transformedVertices[i], pointLights[j].GetLightLocation());
			float d = tempVect1.GetLength();
			Vector3D tempVect2 = _transformedVertices[i].GetNormal();
			float amountOfLight = Vector3D::DotProduct(tempVect1, tempVect2);
			float totalAttenuation = 1 / (a + (b * d) + (c * (d * d)));
			amountOfLight *= totalAttenuation;
			if(amountOfLight > 1)
			{
				amountOfLight = 1;
			}
			else if(amountOfLight < 0)
			{
				amountOfLight = 0;
			}

			tempR = tempR * amountOfLight;
			tempG = tempG * amountOfLight;
			tempB = tempB * amountOfLight;

			

			totalR += tempR;
			if(totalR > 255)
			{
				totalR = 255;
			}
			else if (totalR < 0)
			{
				totalR = 0;
			}
			totalG += tempG;
			if(totalG > 255)
			{
				totalG = 255;
			}
			else if (totalG < 0)
			{
				totalG = 0;
			}
			totalB += tempB;
			if(totalB > 255)
			{
				totalB = 255;
			}
			else if(totalB < 0)
			{
				totalB = 0;
			}

			Gdiplus::Color tempColour((BYTE)totalR, (BYTE)totalG, (BYTE)totalB);
			_transformedVertices[i].SetColour(tempColour);
		}
	}
}

void Model3D::CalculateVertexNormalVectors()
{
	for(unsigned int i = 0; i < _transformedVertices.size(); i++)
	{
		_transformedVertices[i].SetNormal(Vector3D(0,0,0));
		_transformedVertices[i].SetCount(0);
	}

	for(unsigned int i = 0; i < _polygons.size(); i++)
	{
		Vertex vertices[3];
		vertices[0] = Model3D::GetVertex(i, 0);
		vertices[1] = Model3D::GetVertex(i, 1);
		vertices[2] = Model3D::GetVertex(i, 2);

		Vector3D vect1 = Vertex::ReturnVector(vertices[0], vertices[1]);
		Vector3D vect2 = Vertex::ReturnVector(vertices[0], vertices[2]);
		_polygons[i].SetNormal(Vector3D::CrossProduct(vect2, vect1));

		vertices[0].IncrementCount();
		vertices[1].IncrementCount();
		vertices[2].IncrementCount();
		vertices[0].SetNormal(vertices[0].GetNormal() + _polygons[i].GetNormal());
		vertices[1].SetNormal(vertices[1].GetNormal() + _polygons[i].GetNormal());
		vertices[2].SetNormal(vertices[2].GetNormal() + _polygons[i].GetNormal());
		Model3D::SetVertex(i, 0, vertices[0]);
		Model3D::SetVertex(i, 1, vertices[1]);
		Model3D::SetVertex(i, 2, vertices[2]);
	}

	for(unsigned int i = 0; i < _transformedVertices.size(); i++)
	{
		Vector3D tempVector; 
		Vector3D tempVector2 = _transformedVertices[i].GetNormal();
		int count = _transformedVertices[i].GetCount();

		if(count > 0)
		{
			tempVector.SetX(tempVector2.GetX() / count);
			tempVector.SetY(tempVector2.GetY() / count);
			tempVector.SetZ(tempVector2.GetZ() / count);
			tempVector.SetLength();
		}
		
		//tempVector = Vector3D::Normalise(tempVector);
		_transformedVertices[i].SetNormal(Vector3D::Normalise(tempVector));
	}
}