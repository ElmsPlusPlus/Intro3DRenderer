#pragma once
#include "vertex.h"
#include <string>

class Matrix3D
{
public:
	// Constructors

	Matrix3D(void);
	Matrix3D(float matrix[][4]);
	Matrix3D(const Matrix3D& v);
	~Matrix3D(void);

	// Accessor and Mutator for the matrix, allows any value of the matrix to be accessed

	float GetM(int, int) const;
	void SetM(int, int, float);

	// Operators to allow matrices to be assigned to each other i.e matrix1 = matrix2 so matrix1 now == matrix2
	// and multiplacation operators for multiplying matrices by other matrices and by vertices

	Matrix3D& operator= (const Matrix3D &rhs);

	const Matrix3D operator*(const Matrix3D &other) const;
	const Vertex operator*(const Vertex &p) const;

	// Static Methods for generating matrices to be used 

	static Matrix3D TranslationMatrix(float x, float y, float z);
	static Matrix3D ScalingMatrix(float x, float y, float z);
	static Matrix3D XRotationMatrix(float angle);
	static Matrix3D YRotationMatrix(float angle);
	static Matrix3D ZRotationMatrix(float angle);
	static Matrix3D XYZRotationMatrix(float angleX, float angleY, float angleZ);
	static Matrix3D ViewingMatrix(Vertex &position, float angleX, float angleY, float angleZ);
	static Matrix3D PerspectiveMatrix();
	static Matrix3D ScreenMatrix(unsigned int width, unsigned int height);

	private:
		// 2 Dimensional Array storing the floats that make up the matrix

		float _matrix[4][4];

		// Private Methods

		void Init(float matrix[][4]);
		void Copy(const Matrix3D& v);

		
};

