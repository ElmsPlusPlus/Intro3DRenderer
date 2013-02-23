#include "StdAfx.h"
#include "Camera.h"

// Constructors

Camera::Camera(void)
{
}

Camera::Camera(float xRotation, float yRotation, float zRotation, const Vertex& position,
	unsigned int viewWidth, unsigned int viewHeight)
{
	Init(xRotation, yRotation, zRotation, position, viewWidth, viewHeight);
	Camera::SetViewpointMatrix();
	Camera::SetPerspectiveMatrix();
	Camera::SetScreenMatrix();
}

Camera::~Camera(void)
{
}

// Accessors and Mutators Camera variables

float Camera::GetXRot(void) const
{
	return _xRotation;
}

void Camera::SetXRot(float xRot)
{
	_xRotation = xRot;
	// Create a new viewpoint matrix as values have changed
	Camera::SetViewpointMatrix();
}

float Camera::GetYRot(void) const
{
	return _yRotation;
}

void Camera::SetYRot(float yRot)
{
	_yRotation = yRot;
	// Create a new viewpoint matrix as values have changed
	Camera::SetViewpointMatrix();
}

float Camera::GetZRot(void) const
{
	return _zRotation;
}

void Camera::SetZRot(float zRot)
{
	_zRotation = zRot;
	// Create a new viewpoint matrix as values have changed
	Camera::SetViewpointMatrix();
}

Vertex Camera::GetPosition(void) const
{
	return _Position;
}

void Camera::SetPosition(Vertex& position)
{
	_Position = position;
	// Create a new viewpoint matrix as values have changed
	Camera::SetViewpointMatrix();
}

unsigned int Camera::GetViewWidth(void) const
{
	return _ViewWidth;
}

void Camera::SetViewWidth(unsigned int viewWidth)
{
	_ViewWidth = viewWidth;
}

unsigned int Camera::GetViewHeight(void) const
{
	return _ViewHeight;
}

void Camera::SetViewHeight(unsigned int viewHeight)
{
	_ViewHeight = viewHeight;
}

// Accessors and Mutators for Matrices created by the camera variables

Matrix3D Camera::GetViewpointMatrix(void)
{
	return _ViewingMatrix;
}

void Camera::SetViewpointMatrix()
{
	_ViewingMatrix = Matrix3D::ViewingMatrix(_Position, _xRotation, _yRotation, _zRotation);
}

Matrix3D Camera::GetPerspectiveMatrix(void)
{
	return _PerspectiveMatrix;
}

void Camera::SetPerspectiveMatrix()
{
	_PerspectiveMatrix = Matrix3D::PerspectiveMatrix();
}

Matrix3D Camera::GetScreenMatrix(void)
{
	return _ScreenMatrix;
}

void Camera::SetScreenMatrix()
{
	_ScreenMatrix = Matrix3D::ScreenMatrix(_ViewWidth, _ViewHeight);
}

// Private Methods

void Camera::Init(float xRotation, float yRotation, float zRotation, const Vertex& position,
		unsigned int viewWidth, unsigned int viewHeight)
{
	_xRotation = xRotation;
	_yRotation = yRotation;
	_zRotation = zRotation;
	_Position = position;
	_ViewWidth = viewWidth;
	_ViewHeight = viewHeight;
}
