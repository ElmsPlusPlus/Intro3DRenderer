#include "StdAfx.h"
#include "Matrix3D.h"
#include "Vertex.h"


//Constructors

Matrix3D::Matrix3D(void)
{
	float tempMatrix[4][4];

	for (unsigned int i = 0; i < 4; i++) 
	{
		for (unsigned int j = 0; j < 4; j++) 
		{
			tempMatrix[i][j] = 0.0f;
		}
	}

	Init(tempMatrix);
}

Matrix3D::Matrix3D(float matrix[][4])
{
	Init(matrix);
}

Matrix3D::Matrix3D(const Matrix3D& v)
{
	Copy(v);
}

Matrix3D::~Matrix3D(void)
{
	
}

//Accessors and Mutators

float Matrix3D::GetM(int x, int y) const
{
	return _matrix[x][y];
}

void Matrix3D::SetM(int x, int y, float f)
{
	_matrix[x][y] = f;
}


// Operators for =, multiplacation by vertex and by another matrix

Matrix3D& Matrix3D::operator= (const Matrix3D &rhs)
{
	if (this != &rhs)
	{
		Copy(rhs);
	}
	return *this;
}

const Matrix3D Matrix3D::operator*(const Matrix3D &other) const
{
	Matrix3D result;
	int i,j,k;
	float sum;
	
	// Formula to calculate the multiplacation of two 4x4 matrices

	for (i = 0; i < 4; i++)
	{
		for (j = 0; j < 4; j++)
		{
			sum = 0.0f;
			for (k = 0; k < 4; k++)
			{ 
				sum = sum + _matrix[i][k] * other.GetM(k, j);
			}

			result.SetM(i, j, sum);
		}
	}
	return result;
}

const Vertex Matrix3D::operator*(const Vertex &p) const
{
	Vertex result;
	result.SetX((_matrix[0][0] * p.GetX()) + (_matrix[1][0] * p.GetY()) + (_matrix[2][0] * p.GetZ()) + (_matrix[3][0] * p.GetW()));
	result.SetY((_matrix[0][1] * p.GetX()) + (_matrix[1][1] * p.GetY()) + (_matrix[2][1] * p.GetZ()) + (_matrix[3][1] * p.GetW()));
	result.SetZ((_matrix[0][2] * p.GetX()) + (_matrix[1][2] * p.GetY()) + (_matrix[2][2] * p.GetZ()) + (_matrix[3][2] * p.GetW()));
	result.SetW((_matrix[0][3] * p.GetX()) + (_matrix[1][3] * p.GetY()) + (_matrix[2][3] * p.GetZ()) + (_matrix[3][3] * p.GetW()));
	result.SetColour(p.GetColour());
	return result;
}

// Private Methods

void Matrix3D::Init(float matrix[][4])
{
	// Initialises a new matrix based on a 2d array of values beign passed in

	for (unsigned int i = 0; i < 4; i++)
	{
		for (unsigned int j = 0; j < 4; j++)
		{
			_matrix[i][j] = matrix[i][j];
		}
	}
}
 
void Matrix3D::Copy(const Matrix3D& v)
{
	for (unsigned int i = 0; i < 4; i++)
	{
		for (unsigned int j = 0; j < 4; j++)
		{
			_matrix[i][j] = v.GetM(i,j);
		}
	}
}

//Static Matrix Multiplacation Methods

Matrix3D Matrix3D::TranslationMatrix(float x, float y, float z)
{
	// Generates a translation matrix, when mutliplied by another vertex it will translate the vertex by the x, y and z
	// co-ordinates passed in

	Matrix3D transMatrix;

	transMatrix.SetM(0, 0, 1.0f);	transMatrix.SetM(1, 0, 0.0f);	transMatrix.SetM(2, 0, 0.0f);	transMatrix.SetM(3, 0, -x);
	transMatrix.SetM(0, 1, 0.0f);	transMatrix.SetM(1, 1, 1.0f);	transMatrix.SetM(2, 1, 0.0f);	transMatrix.SetM(3, 1, -y);
	transMatrix.SetM(0, 2, 0.0f);	transMatrix.SetM(1, 2, 0.0f);	transMatrix.SetM(2, 2, 1.0f);	transMatrix.SetM(3, 2, -z);
	transMatrix.SetM(0, 3, 0.0f);	transMatrix.SetM(1, 3, 0.0f);	transMatrix.SetM(2, 3, 0.0f);	transMatrix.SetM(3, 3, 1.0f);

	return transMatrix;
}

Matrix3D Matrix3D::ScalingMatrix(float x, float y, float z)
{
	// Generates a scaling matrix, when multiplied by a vertex it will scale the vertex by the x, y and z co-ordinate passed in

	Matrix3D scalMatrix;

	scalMatrix.SetM(0, 0, x);		scalMatrix.SetM(1, 0, 0.0f);	scalMatrix.SetM(2, 0, 0.0f);	scalMatrix.SetM(3, 0, 0.0f);
	scalMatrix.SetM(0, 1, 0.0f);	scalMatrix.SetM(1, 1, y);		scalMatrix.SetM(2, 1, 0.0f);	scalMatrix.SetM(3, 1, 0.0f);
	scalMatrix.SetM(0, 2, 0.0f);	scalMatrix.SetM(1, 2, 0.0f);	scalMatrix.SetM(2, 2, z);		scalMatrix.SetM(3, 2, 0.0f);
	scalMatrix.SetM(0, 3, 0.0f);	scalMatrix.SetM(1, 3, 0.0f);	scalMatrix.SetM(2, 3, 0.0f);	scalMatrix.SetM(3, 3, 1.0f);

	return scalMatrix;
}

Matrix3D Matrix3D::XRotationMatrix(float angle)
{
	// Generates a rotation matrix, will rotate a vertex around the x axis by the angle passed in

	Matrix3D xRotMatrix;

	xRotMatrix.SetM(0, 0, 1.0f);	xRotMatrix.SetM(1, 0, 0.0f);			xRotMatrix.SetM(2, 0, 0.0f);			xRotMatrix.SetM(3, 0, 0.0f);
	xRotMatrix.SetM(0, 1, 0.0f);	xRotMatrix.SetM(1, 1, (cos(angle)));	xRotMatrix.SetM(2, 1, (-sin(angle)));	xRotMatrix.SetM(3, 1, 0.0f);
	xRotMatrix.SetM(0, 2, 0.0f);	xRotMatrix.SetM(1, 2, (sin(angle)));	xRotMatrix.SetM(2, 2, (cos(angle)));	xRotMatrix.SetM(3, 2, 0.0f);
	xRotMatrix.SetM(0, 3, 0.0f);	xRotMatrix.SetM(1, 3, 0.0f);			xRotMatrix.SetM(2, 3, 0.0f);			xRotMatrix.SetM(3, 3, 1.0f);

	return xRotMatrix;
}

Matrix3D Matrix3D::YRotationMatrix(float angle)
{
	// Generates a rotation matrix, will rotate a vertex around the y axis by the angle passed in

	Matrix3D yRotMatrix;

	yRotMatrix.SetM(0, 0, (cos(angle)));	yRotMatrix.SetM(1, 0, 0.0f);	yRotMatrix.SetM(2, 0, (sin(angle)));	yRotMatrix.SetM(3, 0, 0.0f);
	yRotMatrix.SetM(0, 1, 0.0f);			yRotMatrix.SetM(1, 1, 1.0f);	yRotMatrix.SetM(2, 1, 0);				yRotMatrix.SetM(3, 1, 0.0f);
	yRotMatrix.SetM(0, 2, (-sin(angle)));	yRotMatrix.SetM(1, 2, 0.0f);	yRotMatrix.SetM(2, 2, (cos(angle)));	yRotMatrix.SetM(3, 2, 0.0f);
	yRotMatrix.SetM(0, 3, 0.0f);			yRotMatrix.SetM(1, 3, 0.0f);	yRotMatrix.SetM(2, 3, 0.0f);			yRotMatrix.SetM(3, 3, 1.0f);

	return yRotMatrix;
}

Matrix3D Matrix3D::ZRotationMatrix(float angle)
{
	// Generates a rotation matrix, will rotate a vertex around the z axis by the angle passed in

	Matrix3D zRotMatrix;

	zRotMatrix.SetM(0, 0, (cos(angle)));	zRotMatrix.SetM(1, 0, (-sin(angle)));	zRotMatrix.SetM(2, 0, 0.0f);	zRotMatrix.SetM(3, 0, 0.0f);
	zRotMatrix.SetM(0, 1, (sin(angle)));	zRotMatrix.SetM(1, 1, (cos(angle)));	zRotMatrix.SetM(2, 1, 0.0f);	zRotMatrix.SetM(3, 1, 0.0f);
	zRotMatrix.SetM(0, 2, 0.0f);			zRotMatrix.SetM(1, 2, 0.0f);			zRotMatrix.SetM(2, 2, 1.0f);	zRotMatrix.SetM(3, 2, 0.0f);
	zRotMatrix.SetM(0, 3, 0.0f);			zRotMatrix.SetM(1, 3, 0.0f);			zRotMatrix.SetM(2, 3, 0.0f);	zRotMatrix.SetM(3, 3, 1.0f);

	return zRotMatrix;
}

Matrix3D Matrix3D::XYZRotationMatrix(float angleX, float angleY, float angleZ)
{
	// Generates a rotation matrix, will rotate a vertex and around the x, y and z axes, calculated by calling the static fuctions
	// to generate a rotation matrix of each axis, them multiplying them together

	Matrix3D xyzRotMatrix;

	xyzRotMatrix = Matrix3D::XRotationMatrix(angleX) * Matrix3D::YRotationMatrix(angleY) * Matrix3D::ZRotationMatrix(angleZ); 

	return xyzRotMatrix;
}

Matrix3D Matrix3D::ViewingMatrix(Vertex &position, float angleX, float angleY, float angleZ)
{
	// Generates a viewing matrix of the camera based on it position and rotation. Calls the static methods to calculate a translation matrix
	// and rotation and multiplies them

	Matrix3D viewingMatrix;

	viewingMatrix = Matrix3D::XYZRotationMatrix(angleX, angleY, angleZ) *  
		Matrix3D::TranslationMatrix(position.GetX(), position.GetY(), position.GetZ());

	return viewingMatrix;
}

Matrix3D Matrix3D::PerspectiveMatrix()
{
	// Generates a perspective matrix, d is currently hard-coded to 1

	Matrix3D perMatrix;

	perMatrix.SetM(0, 0, 1.0f);		perMatrix.SetM(1, 0, 0.0f);		perMatrix.SetM(2, 0, 0.0f);		perMatrix.SetM(3, 0, 0.0f);
	perMatrix.SetM(0, 1, 0.0f);		perMatrix.SetM(1, 1, 1.0f);		perMatrix.SetM(2, 1, 0.0f);		perMatrix.SetM(3, 1, 0.0f);
	perMatrix.SetM(0, 2, 0.0f);		perMatrix.SetM(1, 2, 0.0f);		perMatrix.SetM(2, 2, 1.0f);		perMatrix.SetM(3, 2, 0.0f);
	perMatrix.SetM(0, 3, 0.0f);		perMatrix.SetM(1, 3, 0.0f);		perMatrix.SetM(2, 3, 1.0f);		perMatrix.SetM(3, 3, 0.0f);

	return perMatrix;
}

Matrix3D Matrix3D::ScreenMatrix(unsigned int width, unsigned int height)
{
	// Generates a screen matrix based on the height and width of the viewing area given by the camera

	Matrix3D screenMatrix;

	float scalVar1 = (float)(min(width, height) / 2);
	float scalVar2 = (- 1.0f *(float)(min(width, height) / 2));

	screenMatrix.SetM(0, 0, scalVar1);	screenMatrix.SetM(1, 0, 0.0f);		screenMatrix.SetM(2, 0, 0.0f);		screenMatrix.SetM(3, 0, (float)(width / 2));
	screenMatrix.SetM(0, 1, 0.0f);		screenMatrix.SetM(1, 1, scalVar2);	screenMatrix.SetM(2, 1, 0.0f);		screenMatrix.SetM(3, 1, (float)(height / 2));
	screenMatrix.SetM(0, 2, 0.0f);		screenMatrix.SetM(1, 2, 0.0f);		screenMatrix.SetM(2, 2, 1.0f);		screenMatrix.SetM(3, 2, 0.0f);
	screenMatrix.SetM(0, 3, 0.0f);		screenMatrix.SetM(1, 3, 0.0f);		screenMatrix.SetM(2, 3, 0.0f);		screenMatrix.SetM(3, 3, 1.0f);

	return screenMatrix;
}




