#pragma once
#include "stdafx.h"

class Camera
{
public:
	// Constructors

	Camera(void);
	Camera(float xRotation, float yRotation, float zRotation, const Vertex& position,
		unsigned int viewWidth, unsigned int viewHeight);
	~Camera(void);

	// Accessors and Mutators for Camera variables

	float GetXRot(void) const;
	void SetXRot(float xRot);
	float GetYRot(void) const;
	void SetYRot(float yRot);
	float GetZRot(void) const;
	void SetZRot(float zRot);
	Vertex GetPosition(void) const;
	void SetPosition(Vertex& position);
	unsigned int GetViewWidth(void) const;
	void SetViewWidth(unsigned int viewWidth);
	unsigned int GetViewHeight(void) const;
	void SetViewHeight(unsigned int viewHeight);

	// Accessors and Mutators for Matrices created by the camera variables

	Matrix3D GetViewpointMatrix(void);
	void SetViewpointMatrix();
	Matrix3D GetPerspectiveMatrix(void);
	void SetPerspectiveMatrix();
	Matrix3D GetScreenMatrix(void);
	void SetScreenMatrix();

private:
	// Rotation values of the camera

	float _xRotation;
	float _yRotation;
	float _zRotation;

	// Position of the Camera

	Vertex _Position;

	// Height and Width of the camera view

	unsigned int _ViewWidth;
	unsigned int _ViewHeight;

	// Matrices created by the Camera variables

	Matrix3D _ViewingMatrix;
	Matrix3D _PerspectiveMatrix;
	Matrix3D _ScreenMatrix;

	// Private Methods

	void Init(float xRotation, float yRotation, float zRotation, const Vertex& position,
		unsigned int viewWidth, unsigned int viewHeight);

};

