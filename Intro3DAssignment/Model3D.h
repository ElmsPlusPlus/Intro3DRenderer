#pragma once

#include "Polygon3D.h"
#include "stdafx.h"

class Model3D
{
public:
	// Constructors

	Model3D(void);
	~Model3D(void);

	// Accessor and Mutator for vertices in the model (from the transformed vertices list as these are the ones that are used for calculations

	Vertex GetVertex(int index, int vertex);
	void SetVertex(int index, int vertex, Vertex tempVertex);

	// Adds vertices and polygons to the lists in the model

	void AddVertex(Vertex&);
	void AddPolygon(Polygon3D&);

	// Accessor for a polygon in the model

	Polygon3D GetPolygon(int) const;
	
	// Accessors and mutators for the reflection coefficients

	float GetKaRed(void) const;
	void SetKaRed(float);
	float GetKaGreen(void) const;
	void SetKaGreen(float);
	float GetKaBlue(void) const;
	void SetKaBlue(float);
	float GetKdRed(void) const;
	void SetKdRed(float);
	float GetKdGreen(void) const;
	void SetKdGreen(float);
	float GetKdBlue(void) const;
	void SetKdBlue(float);
	float GetKsRed(void) const;
	void SetKsRed(float);
	float GetKsGreen(void) const;
	void SetKsGreen(float);
	float GetKsBlue(void) const;
	void SetKsBlue(float);

	// Accessors and mutators for texture information

	BYTE* GetTexture(void) const;
	void SetTexture(BYTE*);
	Gdiplus::Color* GetPalette(BYTE) const;
	void SetPalette(Gdiplus::Color*);
	int GetTextureWidth(void) const;
	void SetTextureWidth(int);

	// Public methods, described in more detail in the .cpp

	int GetNumOfPolys(void) const;
	void ApplyTransformToLocalVertices(const Matrix3D& transform);
	void ApplyTransformToTransformedVertices(const Matrix3D& transform);
	void Deheomogenize();
	bool IsPolygonCulled(int polyNum);
	void CalculateBackfaces(const Vertex& camPosition);
	void Sort(void);
	void CalculateLightingDirectionalFlat(std::vector<DirectionalLight>&);
	void CalculateLightingDirectional(std::vector<DirectionalLight>&);
	void CalculateLightingAmbientFlat(const AmbientLight& ambientLight);
	void CalculateLightingAmbient(const AmbientLight& ambientLight);
	void CalculateLightingPointFlat(std::vector<PointLight>&);
	void CalculateLightingPoint(std::vector<PointLight>&);
	void CalculateVertexNormalVectors();



private:
	// Lists of the polygons of the model, the orignal vertices of the model and the vertices as they
	// are transformed by various functions

	std::vector<Vertex> _vertices;
	std::vector<Polygon3D> _polygons;
	std::vector<Vertex> _transformedVertices;

	// Reflections coefficients

	float _kaRed;
	float _kaGreen;
	float _kaBlue;
	float _kdRed;
	float _kdGreen;
	float _kdBlue;
	float _ksRed;
	float _ksGreen;
	float _ksBlue;

	// Texture variables

	BYTE* _texture;
	Gdiplus::Color* _palette;
	int _textureWidth;
};

